#ifndef TUNNEL_FRAGMENT_H_
#define TUNNEL_FRAGMENT_H_

#include <string>

const std::string tunnelFragmentShaderSrc = R"(
#version 430 core

layout(location = 0) uniform sampler2D u_WallTextureSampler;

in vec2 v_GridPos;              // Revieced from vertex shader (0, 1) in range
in vec2 v_tCoords;              // Texture coordinates recieved from vertex shader
in vec4 v_fragPos;              // INPUT: Position from vertex shader (world space)
in vec4 v_normal;               // INPUT: Normal position from vertex shader

out vec4 fragColor;

// Uniforms
uniform vec2 u_GridSize;        // Grid size of the wall (5x10 for sides, 5x5 for back)
uniform int u_textureEnabled;   // Flag for if the textures should be shown
uniform float u_ambientStrength;
uniform vec3 u_lightSourcePosition;
uniform vec3 u_lightSourcePosition2;
uniform float u_diffuseStr;
uniform vec3 u_cameraPosition;
uniform float u_specularStr;

void main()
{
    float borderWidth = 0.05;
    vec4 textureColor = texture(u_WallTextureSampler, v_tCoords);
    vec3 borderColor = vec3(0.2, 0.8, 0.2);

    // Diffuse light calculations from block
    vec3 lightDirection = normalize(vec3(u_lightSourcePosition - v_fragPos.xyz));
    float diffuseStrength = max(dot(lightDirection, v_normal.xyz), 0.0) * u_diffuseStr;
    // Diffuse light calculations from sun
    vec3 lightDirection2 = normalize(vec3(u_lightSourcePosition2 - v_fragPos.xyz));
    float diffuseStrength2 = max(dot(lightDirection2, v_normal.xyz), 0.0) * u_diffuseStr / 4.0;

    // Specualr illumination
    vec3 reflectedLight = normalize(reflect(-lightDirection, v_normal.xyz));
    vec3 observerDirection = normalize(u_cameraPosition - v_fragPos.xyz);
    float specFactor = pow(max(dot(observerDirection, reflectedLight), 0.0), 64);
    float specular = specFactor * u_specularStr;

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
        // If texture flag is off we use the border color and the alpha (0.0 or 1.0 when close to edge)
        // This will draw borders / grid for our tunnel
        fragColor = vec4((borderColor * (u_ambientStrength + diffuseStrength + diffuseStrength2 + specular)), alpha);
    } else {
        // First we blend the texture and a blue color
        vec4 textureBlend = mix(textureColor, vec4(0.27, 0.5, 1.0, 1.0), 0.3);
        // Then we blend with the border alpha to maintain visible borders
        // alpha is dynamic so we either show the blended texture OR a black border
        vec4 finalColor = mix(textureBlend, vec4(0.0, 0.0, 0.0, 1.0), alpha);
        // Only apply lighting to the rgb channels
        fragColor = vec4(finalColor.rgb * (u_ambientStrength + diffuseStrength + diffuseStrength2 + specular), finalColor.a);
    }
}
)";

#endif // TUNNEL_FRAGMENT_H_
