#version 450

layout(location=0) out vec4 outColor;
layout(location=1) out vec4 fragColor;

void main() {
    outColor = fragColor;
}