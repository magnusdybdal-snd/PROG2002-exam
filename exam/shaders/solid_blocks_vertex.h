#ifndef SOLID_BLOCKS_VERTEX_H_
#define SOLID_BLOCKS_VERTEX_H_

#include <string>

const std::string solidBlocksVertexShaderSrc = R"(
#version 430 core

layout(location = 0) in vec3 i_Position;    // INPUT: 3D position from VBO
layout(location = 1) in vec3 i_normal;      // INPUT: Normal vertex attribute
//layout(location = 2) in mat4 instanceMatrix // INPUT: Array with model matrices

// Uniforms
uniform int u_blockCount;
uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_solidBlockModelMatrix;

out vec3 vs_position;                       // Pass position to fragment shader
out vec4 vs_normal;                         // Pass the normal position to fragment shader
out vec4 vs_fragPosition;                   // Pass the world position to fragment shader

void main()
{
    gl_Position = u_ViewProjectionMatrix * u_solidBlockModelMatrix * vec4(i_Position, 1.0);

    vs_position = i_Position;
    vs_fragPosition = u_solidBlockModelMatrix * vec4(i_Position, 1.0);

    vs_normal = normalize(u_solidBlockModelMatrix * vec4(i_normal, 0.0));
}
)";

#endif // SOLID_BLOCKS_VERTEX_H_