#version 330 core

in vec2 uv;

out vec4 fragColor;

uniform sampler2D myTextureSampler;

void main(){
    fragColor = texture(myTextureSampler, uv).rgba;
}