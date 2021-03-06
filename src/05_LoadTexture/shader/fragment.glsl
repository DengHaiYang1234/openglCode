#version 330 core
out vec4 FragColor;
in vec3 vColor;
in vec2 texCoord;

uniform sampler2D tex;
uniform sampler2D tex1;

uniform float t;

void main() {
    FragColor = mix(texture(tex, texCoord), texture(tex1, texCoord), abs(sin(t) * 0.2f));
}