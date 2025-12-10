#ifndef ACTIVE_CUBE_VERTEX_H_
#define ACTIVE_CUBE_VERTEX_H_

#include <string>

const std::string activeCubeVertexShaderSrc = R"(
#version 430 core

layout(location = 0) in vec3 i_Position;    // INPUT: 3D position from VBO

// Uniforms
uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_activeCubeModelMatrices[4];

out vec3 vs_Position; // Pass position to fragment shader

void main()
{
    mat4 modelMatrix = u_activeCubeModelMatrices[gl_InstanceID];

    gl_Position = u_ViewProjectionMatrix * modelMatrix * vec4(i_Position, 1.0);

    vs_Position = i_Position;
}

)";

#endif // ACTIVE_CUBE_VERTEX_H_