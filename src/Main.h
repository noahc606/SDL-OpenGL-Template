#pragma once
#include <cstdint>
#include <SDL2/SDL.h>
#include "Resources.h"

class Main {
public:
    Main();
    ~Main();

    static int getWidth();
    static int getHeight();
private:

    bool init();
    static void tick();
    static void draw2D();
    static void draw3D();
    static void draw();
    static void events(SDL_Event& e);

    static SDL_Window* sdlWindow;
    static SDL_GLContext sdlGlCtx;
    static Resources* resources;
    static uint64_t lastSec;
};