#ifndef SOLID_CUBE_VERTEX_H_
#define SOLID_CUBE_VERTEX_H_

#include <string>

const std::string solidCubeVertexShaderSrc = R"(
#version 430 core

layout(location = 0) in vec3 i_Position;    // INPUT: 3D position from VBO

// Uniforms
uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_solidCubeModelMatrix;

out vec3 vs_Position; // Pass position to fragment shader

void main()
{
    gl_Position = u_ViewProjectionMatrix * u_solidCubeModelMatrix * vec4(i_Position, 1.0);

    vs_Position = i_Position;
}

)";

#endif // SOLID_CUBE_VERTEX_H_