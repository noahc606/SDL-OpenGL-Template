#include "Draw.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Resources.h"

using namespace nch;

void Draw::texturedTri(const Vec3f& p0, const Vec3f& p1, const Vec3f& p2, const Vec2f& t1, const Vec2f& t2, const Vec2f& t3, GLuint& glVAO, GLuint& glVBO, GLuint& glEBO)
{
	
	/* Vertices and indices */
    float vertices[] = {
        // positions          // colors           // texture coords
		p0.x, p0.y, p0.z, 1.0f, 1.0f, 1.0f, t1.x, t1.y,
		p1.x, p1.y, p1.z, 1.0f, 1.0f, 1.0f, t2.x, t2.y,
		p2.x, p2.y, p2.z, 1.0f, 1.0f, 1.0f, t3.x, t3.y,
    };
    GLuint indices[] = { 0, 1, 2 };
	
	//Vertex array object
	glBindVertexArray(glVAO);
	//Vertex buffer objects
	glBindBuffer(GL_ARRAY_BUFFER, glVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//Vertex element object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //1st arg 0 = Position attribute
	unsigned long sf = sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sf, (void*)0);
    glEnableVertexAttribArray(0);
    //1st arg 1 = Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sf, (void*)(3*sf));
    glEnableVertexAttribArray(1);
    //1st arg 2 = Texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sf, (void*)(6*sf));
    glEnableVertexAttribArray(2);

	//Draw
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

void Draw::texturedTri(const Vec3f& p0, const Vec3f& p1, const Vec3f& p2, GLuint& glVAO, GLuint& glVBO, GLuint& glEBO)
{
	texturedTri(
		Vec3f(p0.x, p0.y, p0.z),
		Vec3f(p1.x, p1.y, p1.z),
		Vec3f(p2.x, p2.y, p2.z),
		Vec2f(0, 0),
		Vec2f(1, 0),
		Vec2f(1, 1),
		glVAO, glVBO, glEBO);
}

void Draw::texturedTri(const Vec2f& p0, const Vec2f& p1, const Vec2f& p2, GLuint& glVAO, GLuint& glVBO, GLuint& glEBO)
{
	texturedTri(
		Vec3f(p0.x, p0.y, 0),
		Vec3f(p1.x, p1.y, 0),
		Vec3f(p2.x, p2.y, 0),
		glVAO, glVBO, glEBO);
}

void Draw::texturedQuad(const Vec3f& p0, const Vec3f& p1, const Vec3f& p2, const Vec3f& p3, GLuint& glVAO, GLuint& glVBO, GLuint& glEBO)
{
	texturedTri( p0, p1, p2, Vec2f(0, 0), Vec2f(0, 1), Vec2f(1, 1), glVAO, glVBO, glEBO );
	texturedTri( p0, p2, p3, Vec2f(0, 0), Vec2f(1, 1), Vec2f(1, 0), glVAO, glVBO, glEBO );
}

void Draw::streamText(const std::string& text, TTF_Font* font, const Color& color, float x, float y, GLuint projectionUniform, glm::mat4 projectionMatrix) {
    
    //Render text to an SDL_Surface
    SDL_Surface* ttfRender = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), {color.r, color.g, color.b, color.a}, 1024);
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(ttfRender, SDL_PIXELFORMAT_ABGR8888, 0); SDL_FreeSurface(ttfRender);
    if (!surface) {
        printf("TTF_RenderText_Blended failed: %s\n", TTF_GetError());
        return;
    }

    //Generate OpenGL texture w/ params
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Vertices for quad (positions(x, y), texture coords(u, v))
    float vertices[] = {
        x, y,                           0.0f, 1.0f,
        x + surface->w, y,              1.0f, 1.0f, // Top-right
        x + surface->w, y + surface->h, 1.0f, 0.0f, // Bottom-right
        x, y + surface->h,              0.0f, 0.0f  // Bottom-left
    };
    //Indices
    GLuint indices[] = { 0, 1, 2, 0, 2, 3 };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Use 2D shader, pass projection matrix
    glUseProgram(Resources::getGLPrgmID(2));
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    glBindTexture(GL_TEXTURE_2D, texture);

    // Draw the textured quad
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Cleanup
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    SDL_FreeSurface(surface);
    glDeleteTextures(1, &texture);
}