#ifndef SOLID_BLOCKS_FRAGMENT_H_
#define SOLID_BLOCKS_FRAGMENT_H_

#include <string>

const std::string solidBlocksFragmentShaderSrc = R"(
#version 430 core

in vec3 vs_Position;           
out vec4 fragColor;

// Uniform
uniform vec3 u_blockColor;

void main()
{
    fragColor = vec4(u_blockColor, 1.0);
}
)";

#endif // SOLID_BLOCKS_FRAGMENT_H_
