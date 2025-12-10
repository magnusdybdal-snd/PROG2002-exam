#ifndef TUNNEL_VERTEX_H_
#define TUNNEL_VERTEX_H_

#include <string>

const std::string tunnelVertexShaderSrc = R"(
#version 430 core

layout(location = 0) in vec2 position;  // Input 2D position coords from VBO
layout(location = 1) in vec2 tCoords;   // Input 2D texture coords from VBO

// Uniforms
uniform mat4 u_tunnelModelMatrix;       // Matrix for back wall
uniform mat4 u_tunnelModelMatrices[4];  // Array of model matrices
uniform mat4 u_ViewProjectionMatrix;
uniform int u_usingInstancing;   

out vec2 v_GridPos;                     // Output pass grid position to fragment shader
out vec2 v_tCoords;                     // Output pass texture coordinates to fragment shader
out vec4 v_fragPos;                     // Output pass world position to fragment shader
out vec4 v_normal;                      // Output pass normals to fragment shader for illumination

void main()
{
    // Get the current model matrix, dependent if we are using instancing.
    mat4 modelMatrix;
    if (u_usingInstancing == 1) {
        modelMatrix = u_tunnelModelMatrices[gl_InstanceID];
    } else {
        modelMatrix = u_tunnelModelMatrix;
    }

    gl_Position = u_ViewProjectionMatrix * modelMatrix * vec4(position, 0.0, 1.0);

    v_fragPos = modelMatrix * vec4(position, 0.0, 1.0);

    // Convert from (-0.5, 0.5) to (0.0, 1.0)
    v_GridPos = position + 0.5;
    // Pass texture coordinates
    v_tCoords = tCoords;

    v_normal = normalize(modelMatrix * vec4(0.0, 0.0, 1.0, 0.0));
}

)";

#endif // TUNNEL_VERTEX_H_
