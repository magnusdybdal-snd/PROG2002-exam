#ifndef TUNNEL_FRAGMENT_H_
#define TUNNEL_FRAGMENT_H_

#include <string>

const std::string tunnelFragmentShaderSrc = R"(
#version 430 core

in vec2 v_GridPos;              // Revieced from vertex shader (0, 1) in range
out vec4 fragColor;

// Uniforms
uniform vec2 u_GridSize;

void main()
{
    float borderWidth = 0.04;

    // Scale position to grid coordinates
    vec2 gridCoord = v_GridPos * u_GridSize;

    // Get position within a tile
    vec2 tilePos = fract(gridCoord);

    // Distance to nearest edge
    float distanceToEdge = min(
        min(tilePos.x, 1.0 - tilePos.x),
        min(tilePos.y, 1.0 - tilePos.y)
    );

    // step returns 0.0 or 1.0, 1.0 when we are on an edge, else 0.0
    float alpha = step(distanceToEdge, borderWidth);

    vec3 color = vec3(0.0, 1.0, 0.0);
    fragColor = vec4(color, alpha);
}
)";

#endif // TUNNEL_FRAGMENT_H_
