#version 330 core

in vec3 fragmentColor;

out vec4 fragColor;

void main(){
    fragColor = vec4(fragmentColor, 1);
    //    fragColor = vec4(1.0, 1.0, 0.0, 1.0);
}