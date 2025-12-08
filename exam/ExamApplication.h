#ifndef EXAMAPPLICATION_H
#define EXAMAPPLICATION_H

#include "GLFWApplication.h"
#include "RenderCommands.h"
#include "GeometricTools.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ExamApplication : public GLFWApplication
{
public:

    ExamApplication(const std::string& name, const std::string& version);
    ~ExamApplication();

    // Override base class methods
    unsigned Init() override;
    unsigned Run() override;

private:

    // ===== SMART POINTERS =====
    std::shared_ptr<VertexArray> m_tunnelVAO;
    std::unique_ptr<Shader> m_tunnelShaderProgram;

    // ===== MODEL MATRICES =====
    glm::mat4 m_bottomWallModelMatrix;

    // ===== INITIALIZATOIN =====
    void InitializeTunnel();

    // ===== RENDERING =====
    void RenderTunnel();

};
#endif // AssignmentApplication_H_