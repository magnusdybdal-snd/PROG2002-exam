#include "ExamApplication.h"

#include <iostream>
#include <algorithm>

#include "shaders/tunnel_vertex.h"
#include "shaders/tunnel_fragment.h"
#include "shaders/tunnel_top_vertex.h"
#include "shaders/tunnel_top_fragment.h"

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

    // =============== CAMERA SETUP ===============
    m_camera = std::make_unique<PerspectiveCamera>(
        PerspectiveCamera::Frustrum{CAMERA_FOV, CAMERA_WIDTH, CAMERA_HEIGHT, CAMERA_NEAR_PLANE, CAMERA_FAR_PLANE},
        glm::vec3(0.0f, 0.0f, -CAMERA_DISTANCE), // camera position - 
        glm::vec3(0.0f, 0.0f, 0.0f), // look at vector - camera looks at origin.
        glm::vec3(0.0f, 1.0f, 0.0f)  // up-direction.
    );

    glEnable(GL_DEPTH_TEST);

    InitializeTunnel();
    InitializeShaders();

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
    // Create 5x5 grid for the back wall
    auto backWallVertices = GeometricTools::UnitGridGeometry2D<5,5>();
    auto backWallIndices = GeometricTools::UnitGridTopologyTriangles<5,5>();
    
    auto backWallVertexBuffer = std::make_shared<VertexBuffer>(backWallVertices.data(), backWallVertices.size() * sizeof(float));
    auto backWallIndexBuffer = std::make_shared<IndexBuffer>(backWallIndices.data(), backWallIndices.size());

    auto backWallBufferLayout = BufferLayout({
        { ShaderDataType::Float2, "position" }
    });
    backWallVertexBuffer->SetLayout(backWallBufferLayout);

    m_tunnelVAO = std::make_shared<VertexArray>();
    m_tunnelVAO->AddVertexBuffer(backWallVertexBuffer);
    m_tunnelVAO->SetIndexBuffer(backWallIndexBuffer);
    m_tunnelVAO->Unbind();

    // Create 5x10 grid for the side walls
    auto tunnelVertices = GeometricTools::UnitGridGeometry2D<5,10>();
    auto tunnelIndices = GeometricTools::UnitGridTopologyTriangles<5,10>();
    
    auto tunnelVertexBuffer = std::make_shared<VertexBuffer>(tunnelVertices.data(), tunnelVertices.size() * sizeof(float));
    auto tunnelIndexBuffer = std::make_shared<IndexBuffer>(tunnelIndices.data(), tunnelIndices.size());

    auto tunnelBufferLayout = BufferLayout({
        { ShaderDataType::Float2, "position" }
    });

    tunnelVertexBuffer->SetLayout(tunnelBufferLayout);

    m_tunnelVAO = std::make_shared<VertexArray>();
    m_tunnelVAO->AddVertexBuffer(tunnelVertexBuffer);
    m_tunnelVAO->SetIndexBuffer(tunnelIndexBuffer);
    m_tunnelVAO->Unbind();
}

/**
 * Initializes the shader programs
 */
void ExamApplication::InitializeShaders()
{   
    m_tunnelShaderProgram = std::make_unique<Shader>(
        tunnelVertexShaderSrc.c_str(), tunnelFragmentShaderSrc.c_str()
    );
}

void ExamApplication::RenderTunnel()
{
    m_tunnelShaderProgram->Bind();
    m_tunnelVAO->Bind();

    m_tunnelShaderProgram->UploadUniformMat4("u_tunnelModelMatrix", m_backWallModelMatrix);
    m_tunnelShaderProgram->UploadUniformMat4("u_ViewProjectionMatrix", m_camera->GetViewProjectionMatrix());



    RenderCommands::DrawIndex(m_tunnelVAO, GL_TRIANGLES);
}