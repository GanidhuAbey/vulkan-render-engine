#version 450

layout(binding = 0) uniform UniformBufferObject  {
    mat4 modelToWorld;
    mat4 worldToCamera;
    mat4 projection;
};

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 2) out vec4 fragColor;

void main() {
    gl_Position =  projection * worldToCamera * modelToWorld * vec4(inPosition, 2.0); //opengl automatically divids the components of the vector by 'w'
    fragColor = vec4(inColor, 1.0);
}