#version 330 core
layout (location = 0) in vec2 position;
layout (location = 2) in vec2 texCoord;

out vec2 outTexCoord;

void main() {
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
    outTexCoord = texCoord;
}
