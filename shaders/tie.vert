#version 330 core

uniform mat4 al_ModelViewMatrix;
uniform mat4 al_ProjectionMatrix;

layout (location = 0) in vec3 position;

out vec3 vPos; // Send to frag
out vec2 vUV;

void main() {
    vPos = position;
    gl_Position = al_ProjectionMatrix * al_ModelViewMatrix * vec4(position, 1.0);
}
