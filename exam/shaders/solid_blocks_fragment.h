#ifndef SOLID_BLOCKS_FRAGMENT_H_
#define SOLID_BLOCKS_FRAGMENT_H_

#include <string>

const std::string solidBlocksFragmentShaderSrc = R"(
#version 430 core

layout(binding = 1) uniform samplerCube u_CubeTextureSampler;

in vec3 vs_Position;        // INPUT: Position from vertex shader
out vec4 fragColor;

// Uniform
uniform vec3 u_blockColor;      // The color of the solid block based on z pos
uniform int u_textureEnabled;   // Flag for if the textures should be shown
uniform float u_ambientStrength;


void main()
{
    vec4 textureColor = texture(u_CubeTextureSampler, vs_Position);

    if(u_textureEnabled == 0) {
        // Flag toggled off, just use block color
        fragColor = vec4(u_blockColor, 1.0) * u_ambientStrength;
    } else {
        // Texture flag on: We mix the block color with the texture
        fragColor = mix(vec4(u_blockColor, 1.0), textureColor, 0.3) * u_ambientStrength; 
    }
}
)";

#endif // SOLID_BLOCKS_FRAGMENT_H_
