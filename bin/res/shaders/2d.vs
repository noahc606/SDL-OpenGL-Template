#version 330 core
layout(location = 0) in vec2 position; // Vertex position
layout(location = 1) in vec2 texCoord; // Texture coordinates

out vec2 TexCoord;

uniform mat4 projection; // Projection matrix

void main() {
    gl_Position = projection * vec4(position, 0.0, 1.0);
    TexCoord = texCoord;
}
