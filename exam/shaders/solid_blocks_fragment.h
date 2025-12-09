#ifndef SOLID_BLOCKS_FRAGMENT_H_
#define SOLID_BLOCKS_FRAGMENT_H_

#include <string>

const std::string solidBlocksFragmentShaderSrc = R"(
#version 430 core

layout(binding = 1) uniform samplerCube u_CubeTextureSampler;

in vec3 vs_Position;        // INPUT: Position from vertex shader
out vec4 fragColor;

// Uniform
uniform vec3 u_blockColor;
uniform int u_textureEnabled;

void main()
{
    vec4 textureColor = texture(u_CubeTextureSampler, vs_Position);

    if(u_textureEnabled == 0) {
        fragColor = vec4(u_blockColor, 1.0);
    } else {
        fragColor = mix(vec4(u_blockColor, 1.0), textureColor, 0.4); 
    }



}
)";

#endif // SOLID_BLOCKS_FRAGMENT_H_
