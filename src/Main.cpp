#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nch/cpp-utils/log.h>
#include <nch/cpp-utils/timer.h>
#include <nch/math-utils/vec3.h>
#include <nch/sdl-utils/main-loop-driver.h>
#include <stdlib.h>
#include "Draw.h"
#include "Main.h"
#include "Shaders.h"

using namespace nch;

SDL_Window* Main::sdlWindow = nullptr;
SDL_GLContext Main::sdlGlCtx;
Resources* Main::resources = nullptr;
uint64_t Main::lastSec = 0;

GLuint glVAO, glVBO, glEBO;
float x = 0;

int main(int argc, char** argv) { Main m; return 0; }
Main::Main()
{
    //Initialization
    if(!init()) Log::throwException(__PRETTY_FUNCTION__, "Initialization failed");

    //Main loop driver
    MainLoopDriver mld(&tick, 60, &draw, 1000, &events);
}

Main::~Main()
{
    /* Cleanup memory */
    //SDL_* quit
    SDL_Quit();
    TTF_Quit();
}

int Main::getWidth() {
    if(sdlWindow==nullptr) return -1;
	int w; SDL_GetWindowSize(sdlWindow, &w, NULL); return w;
}
int Main::getHeight() {
    if(sdlWindow==nullptr) return -1;
	int h; SDL_GetWindowSize(sdlWindow, NULL, &h); return h;
}

bool Main::init()
{    
    /* SDL2 */
    //Init
    if(SDL_Init(SDL_INIT_VIDEO)<0) {
        Log::errorv(__PRETTY_FUNCTION__, "SDL error", SDL_GetError());
        return false;
    }
    //Create window and GL context
	sdlWindow = SDL_CreateWindow("2D & 3D rendering", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    sdlGlCtx = SDL_GL_CreateContext(sdlWindow);
    //SDL-OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetSwapInterval(0);

    /* SDL_TTF */
    if(TTF_Init()==-1) {
        Log::errorv(__PRETTY_FUNCTION__, "TTF error", TTF_GetError());
        return false;
    }
    
    /* GLEW */
    //Init
    glewExperimental = GL_TRUE;  // Force GLEW to get exported functions instead of checking via extension string
    if(glewInit()!=GLEW_OK) {
        nch::Log::errorv(__PRETTY_FUNCTION__, "glewInit()", "Failed to inititalize GLEW");
        return false;
    }

    /* Misc. setup */
    //Set depth func, enable blending, etc.
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Generate VAO, VBO, and VEO buffers
	glGenVertexArrays(1, &glVAO);
	glGenBuffers(1, &glVBO);
	glGenBuffers(1, &glEBO);
    //Load resources
    resources = new Resources();
    //Successful return by this point
    return true;
}

void Main::tick()
{
    //Run this block every second
    if(Timer::getTicks()/1000>lastSec) {
        lastSec = Timer::getTicks()/1000;
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
            std::cout << "OpenGL Error: " << gluErrorString(err) << std::endl;
    }
    x+=0.4;
}

void Main::draw2D()
{
    /* Orthographic projection */
    glm::mat4 projection = glm::ortho(0.0f, (float)getWidth(), 0.0f, (float)getHeight(), -1.0f, 1.0f);
    auto projectionUniform = glGetUniformLocation(Resources::getGLPrgmID(2), "projection");
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

    /* Debug text */
    //Build debug text
    std::stringstream dbgTxt;
    dbgTxt << MainLoopDriver::getPerformanceInfo();
    //Draw debug text
    Color cc; cc.setFromHSV( (double)((SDL_GetTicks()/2)%360), 30, 30); 
    Draw::streamText(dbgTxt.str(),
        Resources::getTTF(Resources::FONT_backToEarth),
        Color((255-cc.r)/8, (255-cc.g)/8, (255-cc.b)/8, 255),
        4, getHeight()-26,
        projectionUniform, projection
    );
}

void Main::draw3D()
{
    glActiveTexture(GL_TEXTURE0);
    
    glBindTexture(GL_TEXTURE_2D, Resources::getGLTextureID(Resources::TEX_TILE_rock));
    Draw::texturedQuad(Vec3f(x/200, 0, 0), Vec3f(x/200, 0.5, 0), Vec3f(0.5+x/200, 0.5, 0), Vec3f(0.5+x/200, 0, 0), glVAO,  glVBO, glEBO);
    glBindTexture(GL_TEXTURE_2D, Resources::getGLTextureID(Resources::TEX_TILE_metal));
    Draw::texturedTri(Vec3f(-1, -1, 0.5), Vec3f(1, -1, 0.5), Vec3f(1, 1, -0.3), glVAO,  glVBO, glEBO);
}

void Main::draw()
{
    /* Clear screen */
    //Set clear color and clear
    Color cc; cc.setFromHSV( (double)((SDL_GetTicks()/2)%360), 30, 30); 
    glClearColor(cc.r/255.0f, cc.g/255.0f, cc.b/255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /* Draw 3D stuff */
    glEnable(GL_DEPTH_TEST);
    glUseProgram(Resources::getGLPrgmID(3));
    draw3D();

    /* Draw 2D stuff */
    glDisable(GL_DEPTH_TEST);
    glUseProgram(Resources::getGLPrgmID(2));
    draw2D();
    
    /* Send objects to window */
    SDL_GL_SwapWindow(sdlWindow);
}

void Main::events(SDL_Event& e)
{
	switch(e.window.event) {
		case SDL_WINDOWEVENT_SIZE_CHANGED: {
            glViewport(0, 0, getWidth(), getHeight());
		} break;
	}
}