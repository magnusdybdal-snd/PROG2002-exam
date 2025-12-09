#include "ExamApplication.h"

#include <iostream>
#include <algorithm>

#include "shaders/tunnel_vertex.h"
#include "shaders/tunnel_fragment.h"
#include "shaders/active_cube_vertex.h"
#include "shaders/active_cube_fragment.h"
#include "shaders/solid_blocks_vertex.h"
#include "shaders/solid_blocks_fragment.h"

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
        glm::vec3(0.0f, 0.0f, CAMERA_DISTANCE), // camera position - 
        glm::vec3(0.0f, 0.0f, 0.0f), // look at vector - camera looks at origin.
        glm::vec3(0.0f, 1.0f, 0.0f)  // up-direction.
    );

    glEnable(GL_DEPTH_TEST);
    // Enable blending for transparent tiles where no border
    // https://learnopengl.com/Advanced-OpenGL/Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    InitializeTunnel();
    InitializeCube();
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
        RenderSolidBlocks();
        RenderActiveCube();
        MoveActiveCube();
        HandleInput();

        glfwSwapBuffers(window);
    }

    std::cout << "Exiting..." << std::endl;
    return EXIT_SUCCESS;
}

void ExamApplication::InitializeTunnel()
{
    auto bufferLayout = BufferLayout({
        { ShaderDataType::Float2, "position" }
    });

    // Create 5x5 grid for the back wall
    auto backWallVertices = GeometricTools::UnitGridGeometry2D<5,5>();
    auto backWallIndices = GeometricTools::UnitGridTopologyTriangles<5,5>();
    
    auto backWallVertexBuffer = std::make_shared<VertexBuffer>(backWallVertices.data(), backWallVertices.size() * sizeof(float));
    auto backWallIndexBuffer = std::make_shared<IndexBuffer>(backWallIndices.data(), backWallIndices.size());

    backWallVertexBuffer->SetLayout(bufferLayout);

    m_backWallVAO = std::make_shared<VertexArray>();
    m_backWallVAO->AddVertexBuffer(backWallVertexBuffer);
    m_backWallVAO->SetIndexBuffer(backWallIndexBuffer);
    m_backWallVAO->Unbind();

    // Create 5x10 grid for the side walls
    auto tunnelVertices = GeometricTools::UnitGridGeometry2D<5,10>();
    auto tunnelIndices = GeometricTools::UnitGridTopologyTriangles<5,10>();
    
    auto tunnelVertexBuffer = std::make_shared<VertexBuffer>(tunnelVertices.data(), tunnelVertices.size() * sizeof(float));
    auto tunnelIndexBuffer = std::make_shared<IndexBuffer>(tunnelIndices.data(), tunnelIndices.size());

    tunnelVertexBuffer->SetLayout(bufferLayout);

    m_tunnelVAO = std::make_shared<VertexArray>();
    m_tunnelVAO->AddVertexBuffer(tunnelVertexBuffer);
    m_tunnelVAO->SetIndexBuffer(tunnelIndexBuffer);
    m_tunnelVAO->Unbind();

    // Model matrices for all walls

    float tunnelWidth = 5.0f;
    float tunnelHeight = 5.0f;
    float tunnelDepth = 10.0f;

    // Back wall
    m_backWallModelMatrix = glm::mat4(1.0f);
    m_backWallModelMatrix = glm::translate(m_backWallModelMatrix, glm::vec3(0.0f, 0.0f, -tunnelDepth));
    m_backWallModelMatrix = glm::scale(m_backWallModelMatrix, glm::vec3(tunnelWidth, tunnelHeight, 1.0f));
    // Top wall
    m_topWallModelMatrix = glm::mat4(1.0f);
    m_topWallModelMatrix = glm::translate(m_topWallModelMatrix, glm::vec3(0.0f, tunnelHeight/2.0f, -tunnelDepth/2.0f));
    m_topWallModelMatrix = glm::rotate(m_topWallModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    m_topWallModelMatrix = glm::scale(m_topWallModelMatrix, glm::vec3(tunnelWidth, tunnelDepth, 1.0f));
    // Left wall
    m_leftWallModelMatrix = glm::mat4(1.0f);
    m_leftWallModelMatrix = glm::translate(m_leftWallModelMatrix, glm::vec3(-tunnelWidth/2, 0.0f, -tunnelDepth/2.0f));
    m_leftWallModelMatrix = glm::rotate(m_leftWallModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_leftWallModelMatrix = glm::rotate(m_leftWallModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    m_leftWallModelMatrix = glm::scale(m_leftWallModelMatrix, glm::vec3(tunnelWidth, tunnelDepth, 1.0f));
    // Right wall
    m_rightWallModelMatrix = glm::mat4(1.0f);
    m_rightWallModelMatrix = glm::translate(m_rightWallModelMatrix, glm::vec3(tunnelWidth/2, 0.0f, -tunnelDepth/2.0f));
    m_rightWallModelMatrix = glm::rotate(m_rightWallModelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_rightWallModelMatrix = glm::rotate(m_rightWallModelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    m_rightWallModelMatrix = glm::scale(m_rightWallModelMatrix, glm::vec3(tunnelWidth, tunnelDepth, 1.0f));
    // Bottom wall
    m_bottomWallModelMatrix = glm::mat4(1.0f);
    m_bottomWallModelMatrix = glm::translate(m_bottomWallModelMatrix, glm::vec3(0.0f, -tunnelHeight/2.0f, -tunnelDepth/2.0f));
    m_bottomWallModelMatrix = glm::rotate(m_bottomWallModelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    m_bottomWallModelMatrix = glm::scale(m_bottomWallModelMatrix, glm::vec3(tunnelWidth, tunnelDepth, 1.0f));

}

void ExamApplication::InitializeCube()
{
    auto cubeVertices = GeometricTools::UnitCubeGeometry3D;
    auto cubeIndices = GeometricTools::UnitCubeTopologyTriangles;

    auto cubeVertexBuffer = std::make_shared<VertexBuffer>(cubeVertices.data(), cubeVertices.size() * sizeof(float));
    auto cubeIndexBuffer = std::make_shared<IndexBuffer>(cubeIndices.data(), cubeIndices.size());
    auto cubeBufferLayout = BufferLayout(
        {
            { ShaderDataType::Float3, "cube_position" }
        }
    );
    cubeVertexBuffer->SetLayout(cubeBufferLayout);

    m_activeCubeVAO = std::make_shared<VertexArray>();
    m_activeCubeVAO->AddVertexBuffer(cubeVertexBuffer);
    m_activeCubeVAO->SetIndexBuffer(cubeIndexBuffer);
    m_activeCubeVAO->Unbind();

    // Initialize solid blocks vao with the same geometry
    m_solidBlocksVAO = std::make_shared<VertexArray>();
    m_solidBlocksVAO->AddVertexBuffer(cubeVertexBuffer);
    m_solidBlocksVAO->SetIndexBuffer(cubeIndexBuffer);
    m_solidBlocksVAO->Unbind();

    m_cubeModelMatrix = glm::mat4(1.0f);
    m_cubeModelMatrix = glm::translate(m_cubeModelMatrix, glm::vec3(0.0f, -1.0f, 2.0f));
    m_cubeModelMatrix = glm::scale(m_cubeModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
}

/**
 * Initializes the shader programs
 */
void ExamApplication::InitializeShaders()
{   
    m_tunnelShaderProgram = std::make_unique<Shader>(
        tunnelVertexShaderSrc.c_str(), tunnelFragmentShaderSrc.c_str()
    );
    m_activeCubeShaderProgram = std::make_unique<Shader>(
        activeCubeVertexShaderSrc.c_str(), activeCubeFragmentShaderSrc.c_str()
    );
    m_solidBlocksShaderProgram = std::make_unique<Shader>(
        solidBlocksVertexShaderSrc.c_str(), solidBlocksFragmentShaderSrc.c_str()
    );
}

void ExamApplication::HandleInput()
{
    GLFWwindow* window = GetWindow();

    InputHandleBlockMovement(window);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void ExamApplication::InputHandleBlockMovement(GLFWwindow * window)
{
    static bool keyWasPressed = false;
    bool keyIsPressed = false;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (!keyWasPressed && m_activeCubeGridPos[0] < 4) {
            m_activeCubeGridPos[0]++;
            m_cubeModelMatrix = glm::translate(m_cubeModelMatrix, glm::vec3(1.0f, 0.0f, 0.0f));
        }
        keyIsPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (!keyWasPressed && m_activeCubeGridPos[0] > 0) {
            m_activeCubeGridPos[0]--;
            m_cubeModelMatrix = glm::translate(m_cubeModelMatrix, glm::vec3(-1.0f, 0.0f, 0.0f));
        }
        keyIsPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (!keyWasPressed && m_activeCubeGridPos[1] < 4) {
            m_activeCubeGridPos[1]++;
            m_cubeModelMatrix = glm::translate(m_cubeModelMatrix, glm::vec3(0.0f, 1.0f, 0.0f));
        }
        keyIsPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (!keyWasPressed && m_activeCubeGridPos[1] > 0) {
            m_activeCubeGridPos[1]--;
            m_cubeModelMatrix = glm::translate(m_cubeModelMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
        }
        keyIsPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        if (!keyWasPressed && !ShouldBecomeSolid(m_activeCubeGridPos)) {
            m_activeCubeGridPos[2] ++;
            m_activeCubeLastMoveTime = glfwGetTime();
            m_cubeModelMatrix = glm::translate(m_cubeModelMatrix, glm::vec3(0.0f, 0.0f, -1.0f));
        } else {
            if(!keyWasPressed && ShouldBecomeSolid(m_activeCubeGridPos))
                MakeActiveCubeSolid();
        }
        keyIsPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!keyWasPressed) {
            auto distance = 10 - m_activeCubeGridPos[2];
            for (int i = 0; i < distance; i++) {
                auto should = ShouldBecomeSolid(m_activeCubeGridPos + glm::ivec3(0, 0, i));
                if (should) {
                    m_cubeModelMatrix = glm::translate(m_cubeModelMatrix, glm::vec3(0.0f, 0.0f, -static_cast<float>(i)));
                    m_activeCubeGridPos[2] += i;
                    MakeActiveCubeSolid();
                    break;
                }
            }
        }
        keyIsPressed = true;
    }
    keyWasPressed = keyIsPressed;
}

void ExamApplication::RenderTunnel()
{
    m_tunnelShaderProgram->Bind();
    m_backWallVAO->Bind();

    m_tunnelShaderProgram->UploadUniformMat4("u_ViewProjectionMatrix", m_camera->GetViewProjectionMatrix());
    
    m_tunnelShaderProgram->UploadUniformMat4("u_tunnelModelMatrix", m_backWallModelMatrix);
    m_tunnelShaderProgram->UploadUniformFloat2("u_GridSize", {5.0f, 5.0f});
    RenderCommands::DrawIndex(m_backWallVAO, GL_TRIANGLES);

    // Top wall
    m_tunnelVAO->Bind();
    m_tunnelShaderProgram->UploadUniformFloat2("u_GridSize", {5.0f, 10.0f});
    m_tunnelShaderProgram->UploadUniformMat4("u_tunnelModelMatrix", m_topWallModelMatrix);
    RenderCommands::DrawIndex(m_tunnelVAO, GL_TRIANGLES);

    // Left wall
    m_tunnelShaderProgram->UploadUniformMat4("u_tunnelModelMatrix", m_leftWallModelMatrix);
    RenderCommands::DrawIndex(m_tunnelVAO, GL_TRIANGLES);

    // Right wall
    m_tunnelShaderProgram->UploadUniformMat4("u_tunnelModelMatrix", m_rightWallModelMatrix);
    RenderCommands::DrawIndex(m_tunnelVAO, GL_TRIANGLES);

    // Bottom wall
    m_tunnelShaderProgram->UploadUniformMat4("u_tunnelModelMatrix", m_bottomWallModelMatrix);
    RenderCommands::DrawIndex(m_tunnelVAO, GL_TRIANGLES);
}

void ExamApplication::RenderActiveCube()
{
    m_activeCubeShaderProgram->Bind();
    m_activeCubeVAO->Bind();

    m_activeCubeShaderProgram->UploadUniformMat4("u_ViewProjectionMatrix", m_camera->GetViewProjectionMatrix());
    m_activeCubeShaderProgram->UploadUniformMat4("u_activeCubeModelMatrix", m_cubeModelMatrix);
    RenderCommands::DrawIndex(m_activeCubeVAO, GL_TRIANGLES);
}

void ExamApplication::RenderSolidBlocks()
{
    if (m_solidBlocks.size() == 0)
        return;

    m_solidBlocksShaderProgram->Bind();
    m_solidBlocksVAO->Bind();
    
    m_solidBlocksShaderProgram->UploadUniformMat4("u_ViewProjectionMatrix", m_camera->GetViewProjectionMatrix());

    for (const auto& block : m_solidBlocks){
        // Temporary draw call for each one
        glm::mat4 solidBlockModelMatrix = glm::mat4(1.0f);
        solidBlockModelMatrix = glm::translate(solidBlockModelMatrix, block.worldCoordinate);
        solidBlockModelMatrix = glm::scale(solidBlockModelMatrix, glm::vec3(0.5f, 0.5, 0.5f));
        m_solidBlocksShaderProgram->UploadUniformMat4("u_solidBlockModelMatrix", solidBlockModelMatrix);
        m_solidBlocksShaderProgram->UploadUniformFloat3("u_blockColor", block.color);
        RenderCommands::DrawIndex(m_solidBlocksVAO, GL_TRIANGLES);
    } 
}

void ExamApplication::MoveActiveCube()
{
    int time = glfwGetTime();
    if (time - m_activeCubeLastMoveTime >= 2.0f && !ShouldBecomeSolid(m_activeCubeGridPos)) {
        m_activeCubeGridPos[2] ++;
        m_cubeModelMatrix = glm::translate(m_cubeModelMatrix, glm::vec3(0.0f, 0.0f, -1.0f));
        m_activeCubeLastMoveTime = time;
    }
    if (time - m_activeCubeLastMoveTime >= 2.0f && ShouldBecomeSolid(m_activeCubeGridPos)){
        MakeActiveCubeSolid();
    }
}

void ExamApplication::RespawnActiveBlock()
{
    // Reset blocks grid position to start
    m_activeCubeGridPos = glm::ivec3(2, 0, 0);
    // Rebuild the model matrix to init state
    m_cubeModelMatrix = glm::mat4(1.0f);
    m_cubeModelMatrix = glm::translate(m_cubeModelMatrix, glm::vec3(0.0f, -1.0f, 2.0f));
    m_cubeModelMatrix = glm::scale(m_cubeModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
}

bool ExamApplication::ShouldBecomeSolid(glm::ivec3 position)
{
    // We only check the position ahead for becoming solid
    auto positionToCheck = position + glm::ivec3(0, 0, 1);
    // If we are at end of tunnel, return true
    if (positionToCheck[2] > 9) {
        return true;
    }
    // Go trough all solid blocks and check if we will collide on next z move
    for (const auto& block : m_solidBlocks) {
        if (positionToCheck == block.gridCoordinate) {
            return true;
        }
    }
    // No collision ahead of us
    return false;
}

void ExamApplication::MakeActiveCubeSolid()
{
    SolidBlock solidBlock;
    // Copy the grid coordinate from the active cube
    solidBlock.gridCoordinate = m_activeCubeGridPos;
    // Get the world coordinate to the new solid block by extracting it from the model matrix
    solidBlock.worldCoordinate = glm::vec3(m_cubeModelMatrix[3]);
    // Get color for block based on z position
    solidBlock.color = GetColorForSolidBlock(solidBlock.gridCoordinate[2]);
    // Add the solid block to the vector of solid blocks
    m_solidBlocks.push_back(solidBlock);
    // Reset the position of the active block
    RespawnActiveBlock();

}

glm::vec3 ExamApplication::GetColorForSolidBlock(int zPos)
{
    auto colorInt = zPos % 5;
    glm::vec3 color;
    switch (colorInt)
    {
    case 0:
        color = glm::vec3(0.8, 0.1, 0.1);
        break;
    case 1:
        color = glm::vec3(0.1, 0.8, 0.1);
        break;
    case 2:
        color = glm::vec3(0.1, 0.1, 0.8);
        break;
    case 3:
        color = glm::vec3(0.8, 0.8, 0.1); // Yellow
        break;
    case 4:
        color = glm::vec3(0.1, 0.8, 0.8); // Cyan
        break;
    default:
        color = glm::vec3(1.0, 1.0, 1.0); // Fallback, white for now
        break;
    }
    return color;
}