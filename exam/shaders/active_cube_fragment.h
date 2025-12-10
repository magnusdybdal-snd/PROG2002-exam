#ifndef ACTIVE_CUBE_FRAGMENT_H_
#define ACTIVE_CUBE_FRAGMENT_H_

#include <string>

const std::string activeCubeFragmentShaderSrc = R"(
#version 430 core

in vec3 vs_Position;           
out vec4 fragColor;

// Uniforms
uniform float u_ambientStrength;

void main()
{
    vec3 blockColor = vec3(0.0, 1.0, 1.0);

    // Only apply lighting to the rgb values
    fragColor = vec4((blockColor * u_ambientStrength), 0.3);
}
)";

#endif // ACTIVE_CUBE_FRAGMENT_H_
