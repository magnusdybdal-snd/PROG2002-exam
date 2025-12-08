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
#include "PerspectiveCamera.h"

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

    // ===== CAMERA CONSTANTS =====
    static constexpr float CAMERA_FOV = 45.0f;              // degrees
    static constexpr float CAMERA_WIDTH = 1024.0f;
    static constexpr float CAMERA_HEIGHT = 768.0f;
    static constexpr float CAMERA_NEAR_PLANE = 0.1f;
    static constexpr float CAMERA_FAR_PLANE = 15.0f;
    static constexpr float CAMERA_DISTANCE = 3.0f;          // Z distance from origin

    // ===== DYNAMIC MEMBER VARIABLES =====
    bool m_textureEnabled = false;

    // ===== SMART POINTERS =====
    std::shared_ptr<VertexArray> m_backWallVAO;
    std::shared_ptr<VertexArray> m_tunnelVAO;
    std::unique_ptr<Shader> m_tunnelShaderProgram;
    std::unique_ptr<PerspectiveCamera> m_camera;

    // ===== MODEL MATRICES =====
    glm::mat4 m_backWallModelMatrix;
    glm::mat4 m_topWallModelMatrix;


    // ===== INITIALIZATOIN =====
    void InitializeTunnel();
    void InitializeShaders();

    // ===== RENDERING =====
    void RenderTunnel();

};
#endif // AssignmentApplication_H_