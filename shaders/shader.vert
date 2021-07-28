#version 450

layout(binding = 0) uniform UniformBufferObject {
<<<<<<< HEAD
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
=======
    mat3 translation;
} ubo;

layout(location = 0) in vec2 inPosition;
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
layout(location = 1) in vec3 inColor;

layout(location = 2) out vec3 fragColor;

void main() {
<<<<<<< HEAD
    gl_Position =  ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
=======
    vec3 new_pos = ubo.translation * vec3(inPosition, 1.0);
    gl_Position = vec4(vec2(new_pos), 0.0, 1.0);
>>>>>>> 287851c9b52ae4d54f326ba993a64f7c3be3d4c9
    fragColor = inColor;
}
