#version 330 core
out vec4 FragColor;
in vec3 vColor;
in vec2 texCoord;

uniform sampler2D tex;
uniform sampler2D tex1;


void main() {
    FragColor = texture(tex, texCoord);
}