#pragma once
#include <GL/glew.h>
#include <SDL2/SDL_ttf.h>
#include <glm/fwd.hpp>
#include <nch/cpp-utils/color.h>
#include <nch/math-utils/vec2.h>
#include <nch/math-utils/vec3.h>
#include <string>

class Draw {
public:
    static void texturedTri(const nch::Vec3f& p0, const nch::Vec3f& p1, const nch::Vec3f& p2, const nch::Vec2f& t1, const nch::Vec2f& t2, const nch::Vec2f& t3, GLuint& glVAO, GLuint& glVBO, GLuint& glEBO);
    static void texturedTri(const nch::Vec3f& p0, const nch::Vec3f& p1, const nch::Vec3f& p2, GLuint& glVAO, GLuint& glVBO, GLuint& glEBO);
    static void texturedTri(const nch::Vec2f& p0, const nch::Vec2f& p1, const nch::Vec2f& p2, GLuint& glVAO, GLuint& glVBO, GLuint& glEBO);
    static void texturedQuad(const nch::Vec3f& p0, const nch::Vec3f& p1, const nch::Vec3f& p2, const nch::Vec3f& p3, GLuint& glVAO, GLuint& glVBO, GLuint& glEBO);

    //static void streamText(const std::string& text, TTF_Font* font, const nch::Color& color, int x, int y);
    static void streamText(const std::string& text, TTF_Font* font, const nch::Color& color, float x, float y, GLuint projectionUniform, glm::mat4 projectionMatrix);
private:

};