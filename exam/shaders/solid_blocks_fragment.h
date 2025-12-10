#ifndef SOLID_BLOCKS_FRAGMENT_H_
#define SOLID_BLOCKS_FRAGMENT_H_

#include <string>

const std::string solidBlocksFragmentShaderSrc = R"(
#version 430 core

layout(binding = 1) uniform samplerCube u_CubeTextureSampler;

in vec3 vs_position;
in vec4 vs_fragPosition;
in vec4 vs_normal;          // INPUT: Normal position from vertex shader

out vec4 fragColor;

// Uniform
uniform vec3 u_blockColor;      // The color of the solid block based on z pos
uniform int u_textureEnabled;   // Flag for if the textures should be shown
uniform float u_ambientStrength;
uniform vec3 u_lightSourcePosition;
uniform float u_diffuseStr;


void main()
{
    vec4 textureColor = texture(u_CubeTextureSampler, vs_position);

    // Diffuse illumination
    vec3 lightDirection = normalize(vec3(u_lightSourcePosition - vs_fragPosition.xyz));
    float diffuseStrength = max(dot(lightDirection, vs_normal.xyz), 0.0) * u_diffuseStr;

    if(u_textureEnabled == 0) {
        // Flag toggled off, just use block color
        fragColor = vec4((u_blockColor * (u_ambientStrength + diffuseStrength)), 1.0);
    } else {
        // Texture flag on: We mix the block color with the texture
        vec4 textureBlend = mix(vec4(u_blockColor, 1.0), textureColor, 0.3);
        // Only apply lighting to the rgb, not alpha
        fragColor = vec4((textureBlend.rgb * (u_ambientStrength + diffuseStrength)), 1.0);
    }
}
)";

#endif // SOLID_BLOCKS_FRAGMENT_H_
