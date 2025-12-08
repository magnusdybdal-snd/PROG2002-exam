#ifndef TUNNEL_VERTEX_H_
#define TUNNEL_VERTEX_H_

#include <string>

const std::string tunnelVertexShaderSrc = R"(
#version 430 core

layout(location = 0) in vec2 position;

// Uniforms
uniform mat4 u_tunnelModelMatrix;
uniform mat4 u_ViewProjectionMatrix;

out vec2 v_GridPos; // Pas grid position to fragment shader

void main()
{
    gl_Position = u_ViewProjectionMatrix * u_tunnelModelMatrix * vec4(position, 0.0, 1.0);

    // Convert from (-0.5, 0.5) to (0.0, 1.0)
    v_GridPos = position + 0.5;
}

)";

#endif // TUNNEL_VERTEX_H_
