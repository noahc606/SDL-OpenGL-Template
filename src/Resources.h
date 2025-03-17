#pragma once
#include <GL/glew.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

class Resources {
public:
    enum TexID {
    BEGIN_TEX,
        TEX_TILE_metal,
        TEX_TILE_rock,
    END_TEX,
    BEGIN_FONT,
        FONT_backToEarth,
    END_FONT,
    };

    Resources();
    ~Resources();

    static GLuint getGLPrgmID(int dex);
    static GLuint getGLTextureID(int dex);
    static TTF_Font* getTTF(int dex);

private:
    static void loadTex(std::string path);
    static void loadFont(std::string path);

    static GLuint glPrgm2D;
    static GLuint glPrgm3D;
    static std::vector<GLuint> glTextures;
    static std::vector<TTF_Font*> fonts;
    
    static bool initialized;
};