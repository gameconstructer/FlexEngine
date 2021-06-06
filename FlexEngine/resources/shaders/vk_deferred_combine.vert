#version 450

// Deferred PBR Combine

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec2 in_Position2D;
layout (location = 1) in vec2 in_TexCoord;

layout (location = 0) out vec2 ex_TexCoord;

void main()
{
    ex_TexCoord = in_TexCoord;
    gl_Position = vec4(in_Position2D, 0.0f, 1.0);
}
