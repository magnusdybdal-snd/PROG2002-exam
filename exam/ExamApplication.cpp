#include "ExamApplication.h"

#include <iostream>
#include <algorithm>

/**
 * Constructor for ExamApplication
 * Initializes the application with the given name and version, and sets up the window dimensions (1024x768)
 */
ExamApplication::ExamApplication(const std::string &name, const std::string &version)
    : GLFWApplication(name, version, 1024, 768)
{
}

/**
 * Destructor for ExamApplication
 * Resources are cleaned up by pointers and class destructors in VAO, VBO, EBO and Shaders
 */
ExamApplication::~ExamApplication()
{
}

/**
 * Initializes the application
 * 
 * @return EXIT_SUCCESS if initialization succeeds, EXIT_FAILURE otherwise
 */
unsigned ExamApplication::Init()
{
    // Call parent Init first to setup GLFW, window, OpenGL context)
    if (GLFWApplication::Init() != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    InitializeTunnel();

    return EXIT_SUCCESS;
}

/**
 * Main application loop
 * Continuously renders the scene, processes input, and swaps buffers until the window is closed
 * 
 * @return EXIT_SUCCESS when the application exits normally
 */
unsigned ExamApplication::Run()
{
    // Get the window
    GLFWwindow* window = GetWindow();
    
    // Main rendering loop
    while (!glfwWindowShouldClose(window)) 
    {
        // clear screen
        RenderCommands::SetClearColor(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));
        RenderCommands::Clear();

        // Process events
        glfwPollEvents();

        RenderTunnel();

        glfwSwapBuffers(window);
    }

    std::cout << "Exiting..." << std::endl;
    return EXIT_SUCCESS;
}

void ExamApplication::InitializeTunnel()
{
    auto vertices = GeometricTools::UnitGridGeometry2DWTCoords<5, 10>();
    auto indices = GeometricTools::UnitGridTopologyTriangles<5, 10>();

    m_bottomWallModelMatrix = glm::mat4(1.0f);
    m_bottomWallModelMatrix = glm::scale(
        m_bottomWallModelMatrix,
        glm::vec3(3.0f, 3.0f, 1.0f));
    m_bottomWallModelMatrix = glm::rotate(
        m_bottomWallModelMatrix,
        glm::radians(45.0f),
        glm::vec3(1.0f, 0.0f, 0.0f));
    m_bottomWallModelMatrix = glm::translate(
        m_bottomWallModelMatrix,
        glm::vec3(0.0f, 0.0f, 0.0f));
    
    auto wallVertexBuffer = std::make_shared<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));
    auto wallIndexBuffer = std::make_shared<IndexBuffer>(indices.data(), indices.size());

    auto chessboardBufferLayout = BufferLayout({
        { ShaderDataType::Float2, "position" },
        { ShaderDataType::Float2, "tCoords" }
    });
    wallVertexBuffer->SetLayout(chessboardBufferLayout);

    m_tunnelVAO = std::make_shared<VertexArray>();
    m_tunnelVAO->AddVertexBuffer(wallVertexBuffer);
    m_tunnelVAO->SetIndexBuffer(wallIndexBuffer);
    m_tunnelVAO->Unbind();
}

void ExamApplication::RenderTunnel()
{
    m_tunnelShaderProgram->Bind();
    m_tunnelVAO->Bind();

    RenderCommands::DrawIndex(m_tunnelVAO, GL_TRIANGLES);
}