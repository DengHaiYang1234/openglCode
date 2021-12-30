#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;
out vec3 vColor;
out vec2 texCoord;

uniform float angle;

uniform mat4 transform;

mat4 rotate3d(float angle)
{
    return mat4(cos(angle),-sin(angle),0.0f,0.0f,
                sin(angle),cos(angle),0.0f,0.0f,
                0.0f,0.0f,1.0f,0.0f,
                0.0f,0.0f,0.0f,1.0f);
}

void main() {
    vColor = aColor;
    gl_Position = vec4(transform * vec4(aPos,1.0f));
    texCoord = aTexCoord;
}