#ifndef ACTIVE_CUBE_FRAGMENT_H_
#define ACTIVE_CUBE_FRAGMENT_H_

#include <string>

const std::string activeCubeFragmentShaderSrc = R"(
#version 430 core

in vec3 vs_Position;           
out vec4 fragColor;

void main()
{
    fragColor = vec4(0.0, 1.0, 1.0, 0.5);
}
)";

#endif // ACTIVE_CUBE_FRAGMENT_H_
