#include "Resources.h"
#include <nch/cpp-utils/log.h>
#include <stb/stb_image.h>
#include "Shaders.h"

using namespace nch;

GLuint Resources::glPrgm2D;
GLuint Resources::glPrgm3D;
std::vector<GLuint> Resources::glTextures;
std::vector<TTF_Font*> Resources::fonts;

bool Resources::initialized = false;

Resources::Resources()
{
	/* Init check */
    if(initialized) {
        Log::warnv(__PRETTY_FUNCTION__, "doing nothing", "Resources have already been initialized");
        return;
    } initialized = true;

	/* Resource loading */
	//Shaders
	glPrgm2D = Shaders::load("res/shaders/2d.vs", "res/shaders/2d.fs");
	glPrgm3D = Shaders::load("res/shaders/3d.vs", "res/shaders/3d.fs");
	if(glPrgm2D==0 || glPrgm3D==0) {
        Log::error(__PRETTY_FUNCTION__, "Failed to load shaders");
		return;
	}

	//STBI
    stbi_set_flip_vertically_on_load(true);

	//Textures
	loadTex("res/textures/tile/metal.png");
	loadTex("res/textures/tile/rock.png");

	//Fonts
	loadFont("res/fonts/BackToEarth.ttf");
}

Resources::~Resources(){}

GLuint Resources::getGLPrgmID(int dex) {
	switch(dex) {
		case 2: return glPrgm2D;
		case 3: return glPrgm3D;
	}

	Log::warnv(__PRETTY_FUNCTION__, "returning 0", "Invalid index '%d'", dex);
	return 0;
}

GLuint Resources::getGLTextureID(int dex) {
    if(dex<=BEGIN_TEX || dex>=END_TEX) {
        Log::warnv(__PRETTY_FUNCTION__, "returning 0", "Invalid texture ID '%d' provided", dex);
        return 0;
    }

    return glTextures.at(dex-BEGIN_TEX-1);
}

TTF_Font* Resources::getTTF(int dex)
{
    if(dex<=BEGIN_FONT || dex>=END_FONT) {
        Log::warnv(__PRETTY_FUNCTION__, "returning 0", "Invalid font index '%d' provided", dex);
        return 0;
    }

    return fonts.at(dex-BEGIN_FONT-1);
}


void Resources::loadTex(std::string path)
{
    //Create texture ID
    GLuint glTexID;

    //Build 'glTexture1'
	glGenTextures(1, &glTexID);
	glBindTexture(GL_TEXTURE_2D, glTexID); {
		//Set wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//Set filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//Load image data onto texture and generate mipmaps
		unsigned char *data; {
			int width, height, nrChannels;
			data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

			if(data) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			} else {
				Log::error(__PRETTY_FUNCTION__, "Failed to load texture");
				glTextures.push_back(-1);
				return;
			}
		}
		stbi_image_free(data);
	}

    //Add texture ID to 'glTextures'
    glTextures.push_back(glTexID);
}

void Resources::loadFont(std::string path)
{
	TTF_Font* res = TTF_OpenFont(path.c_str(), 25);

	if(res!=NULL) {
		fonts.push_back(res);
	} else {
		Log::error(__PRETTY_FUNCTION__, "Failed to load TTF Font at \"%s\"", path.c_str());
		fonts.push_back(nullptr);
	}
}