#ifndef SOLID_CUBE_FRAGMENT_H_
#define SOLID_CUBE_FRAGMENT_H_

#include <string>

const std::string activeCubeFragmentShaderSrc = R"(
#version 430 core

in vec3 vs_Position;           
out vec4 fragColor;

// Uniform
uniform vec3 u_cubeColor;

void main()
{
    fragColor = vec4(u_cubeColor, 1.0);
}
)";

#endif // SOLID_CUBE_FRAGMENT_H_
