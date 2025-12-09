#ifndef TUNNEL_FRAGMENT_H_
#define TUNNEL_FRAGMENT_H_

#include <string>

const std::string tunnelFragmentShaderSrc = R"(
#version 430 core

layout(location = 0) uniform sampler2D u_WallTextureSampler;

in vec2 v_GridPos;              // Revieced from vertex shader (0, 1) in range
in vec2 v_tCoords;

out vec4 fragColor;

// Uniforms
uniform vec2 u_GridSize;
uniform int u_textureEnabled;

void main()
{
    float borderWidth = 0.05;
    vec4 textureColor = texture(u_WallTextureSampler, v_tCoords);
    vec3 borderColor = vec3(0.2, 0.8, 0.2);

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
    
    if (u_textureEnabled == 0) {
        fragColor = vec4(borderColor, alpha);
    } else {
        // First we blend the texture and a blue color
        vec4 textureBlend = mix(textureColor, vec4(0.27, 0.5, 1.0, 1.0), 0.3);
        // Then we blend with the border alpha to maintain visible borders
        fragColor = mix(textureBlend, vec4(0.0, 0.0, 0.0, 1.0), alpha); 
    }
}
)";

#endif // TUNNEL_FRAGMENT_H_
