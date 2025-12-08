#include "Shader.h"

#include <iostream>

Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc)
{
    CompileShader(GL_VERTEX_SHADER, vertexSrc);
    CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    // Create a shader program
    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    // Check linking
    GLint success;
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(ShaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
    }

    // Shader objects can be deleted when linked to a program
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(ShaderProgram);
}

void Shader::Bind() const
{
    glUseProgram(ShaderProgram);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::UploadUniformFloat1(const std::string &name, const glm::vec1 &vector)
{
    glUniform1f(GetUniformLocation(name), vector.x);
}

void Shader::UploadUniformFloat2(const std::string &name, const glm::vec2 &vector)
{
    glUniform2f(GetUniformLocation(name), vector.x, vector.y);
}

void Shader::UploadUniformFloat3(const std::string &name, const glm::vec3 &vector)
{
    glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z);
}

void Shader::UploadUniformFloat4(const std::string &name, const glm::vec4 &vector)
{
    glUniform4f(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
}

void Shader::UploadUniformInt2(const std::string &name, const glm::ivec2 &vector)
{
    glUniform2i(GetUniformLocation(name), vector.x, vector.y);
}

void Shader::UploadUniformMat4(const std::string &name, const glm::mat4 &matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::UploadUniformInt(const std::string &name, const int &value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::UploadUniformBool(const std::string &name, const bool &value)
{
    glUniform1i(GetUniformLocation(name), (int)value);
}

GLint Shader::GetUniformLocation(const std::string &name)
{
    GLint location = glGetUniformLocation(ShaderProgram, name.c_str());
    if (location == -1) {
        std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;
    }
    return location;
}
/**
 * Compiles a shader, checks the compilation and sets the Vertexshader or FragmentShader
 * in the class based on the shaderType that is passed
 */
void Shader::CompileShader(GLenum shaderType, const std::string &shaderSrc)
{
    const char* src = shaderSrc.c_str();

    // Compile vertex shader
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // Check compilation
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        const char* shaderTypeName = (shaderType == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
        std::cerr << shaderTypeName << " shader compilation failed:\n" << infoLog << std::endl;
    }

    if (shaderType == GL_VERTEX_SHADER) {
        VertexShader = shader;
    } else if (shaderType == GL_FRAGMENT_SHADER) {
        FragmentShader = shader;
    }
}
