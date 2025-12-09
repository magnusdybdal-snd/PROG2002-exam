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

    struct SolidBlock {
        glm::vec3 gridPosition;     // Grid coordinates in tunnel
        glm::vec3 color;            // Color (based on z coordinate)
    };

    // ===== CAMERA CONSTANTS =====
    static constexpr float CAMERA_FOV = 60.0f;              // degrees
    static constexpr float CAMERA_WIDTH = 1024.0f;
    static constexpr float CAMERA_HEIGHT = 768.0f;
    static constexpr float CAMERA_NEAR_PLANE = 0.1f;
    static constexpr float CAMERA_FAR_PLANE = 20.0f;
    static constexpr float CAMERA_DISTANCE = 4.5f;          // Z distance from origin

    // ===== DYNAMIC MEMBER VARIABLES =====
    bool m_textureEnabled = false;

    // ===== ACTIVE CUBE DYNAMIC VARIABLES =====
    glm::ivec3 m_activeCubeGridPos = glm::ivec3(2.0f, 0.0f, 0.0f); // Starting position in the tube
    float m_activeCubeLastMoveTime = 0; // Keeps track of when to automatically move inwards

    // ===== SOLID BLOCKS =====
    std::vector<SolidBlock> m_solidBlocks;

    // ===== SMART POINTERS =====
    std::shared_ptr<VertexArray> m_activeCubeVAO;
    std::shared_ptr<VertexArray> m_backWallVAO;
    std::shared_ptr<VertexArray> m_tunnelVAO;
    std::unique_ptr<Shader> m_tunnelShaderProgram;
    std::unique_ptr<Shader> m_activeCubeShaderProgram;
    std::unique_ptr<PerspectiveCamera> m_camera;

    // ===== MODEL MATRICES =====
    glm::mat4 m_backWallModelMatrix;
    glm::mat4 m_topWallModelMatrix;
    glm::mat4 m_bottomWallModelMatrix;
    glm::mat4 m_leftWallModelMatrix;
    glm::mat4 m_rightWallModelMatrix;
    glm::mat4 m_cubeModelMatrix;

    // ===== INITIALIZATOIN =====
    void InitializeTunnel();
    void InitializeCube();
    void InitializeShaders();

    // ===== RENDERING =====
    void RenderTunnel();
    void RenderActiveCube();

    // ===== Input =====
    void HandleInput();
    void InputHandleBlockMovement(GLFWwindow *window);

    void MoveActiveCube();
    void MakeActiveCubeSolid();



};
#endif // AssignmentApplication_H_