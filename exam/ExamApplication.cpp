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

    InitializeTextures();
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
        RenderCommands::SetClearColor(glm::vec4(1.0f) * m_globalIllumination);
        RenderCommands::Clear();

        // Process events
        glfwPollEvents();
        MoveActiveCube();
        HandleInput();

        m_lightSourcePos = glm::vec3(m_cubeModelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

        RenderTunnel();
        RenderSolidBlocks();
        RenderActiveCube();

        glfwSwapBuffers(window);
    }

    std::cout << "Exiting..." << std::endl;
    return EXIT_SUCCESS;
}

void ExamApplication::InitializeTunnel()
{
    auto bufferLayout = BufferLayout({
        { ShaderDataType::Float2, "position" },
        { ShaderDataType::Float2, "tCoords" }
    });

    // Create 5x5 grid for the back wall
    auto backWallVertices = GeometricTools::UnitGridGeometry2DWTCoords<5,5>();
    auto backWallIndices = GeometricTools::UnitGridTopologyTriangles<5,5>();
    
    auto backWallVertexBuffer = std::make_shared<VertexBuffer>(backWallVertices.data(), backWallVertices.size() * sizeof(float));
    auto backWallIndexBuffer = std::make_shared<IndexBuffer>(backWallIndices.data(), backWallIndices.size());

    backWallVertexBuffer->SetLayout(bufferLayout);

    m_backWallVAO = std::make_shared<VertexArray>();
    m_backWallVAO->AddVertexBuffer(backWallVertexBuffer);
    m_backWallVAO->SetIndexBuffer(backWallIndexBuffer);
    m_backWallVAO->Unbind();

    // Create 5x10 grid for the side walls
    auto tunnelVertices = GeometricTools::UnitGridGeometry2DWTCoords<5,10>();
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
    m_topWallModelMatrix = glm::rotate(m_topWallModelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
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
    m_bottomWallModelMatrix = glm::rotate(m_bottomWallModelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    m_bottomWallModelMatrix = glm::scale(m_bottomWallModelMatrix, glm::vec3(tunnelWidth, tunnelDepth, 1.0f));

}

void ExamApplication::InitializeCube()
{
    auto cubeVertices = GeometricTools::UnitCube3D24WNormals;
    auto cubeIndices = GeometricTools::UnitCube3D24WNormalsTopologyTriangles;

    auto cubeVertexBuffer = std::make_shared<VertexBuffer>(cubeVertices.data(), cubeVertices.size() * sizeof(float));
    auto cubeIndexBuffer = std::make_shared<IndexBuffer>(cubeIndices.data(), cubeIndices.size());
    auto cubeBufferLayout = BufferLayout(
        {
            { ShaderDataType::Float3, "cube_position" },
            { ShaderDataType::Float3, "cube_normal" }

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

    MakeLPiece();
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

void ExamApplication::InitializeTextures()
{
    auto textureManager = TextureManager::GetInstance();
    textureManager->LoadTexture2D("wallTexture", std::string(TEXTURES_DIR) + "wall_texture.jpeg", 0);
    textureManager->LoadCubeMap("cubeTexture", std::string(TEXTURES_DIR) + "block_texture.png", 1);
}

void ExamApplication::HandleInput()
{
    GLFWwindow* window = GetWindow();

    InputHandleBlockMovement(window);
    InputHandleTextureToggle(window);
    InputHandleRotation(window);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void ExamApplication::InputHandleTextureToggle(GLFWwindow *window)
{
    static bool tWasPressed = false;
    bool tPressed = (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS);
       if (tPressed && !tWasPressed) {
        m_textureEnabled = !m_textureEnabled;
    } 
    tWasPressed = tPressed;
}

void ExamApplication::InputHandleBlockMovement(GLFWwindow *window)
{
    static bool keyWasPressed = false;
    bool keyIsPressed = false;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (!keyWasPressed) {
            bool canMove = true;
            for (const auto& block : m_activePiece) {
                if (IsOccupied(block.gridCoordinate + glm::ivec3(1, 0, 0))) {
                    canMove = false;
                    break;
                }
            }
            if (canMove) {
                for (auto& block: m_activePiece) {
                    block.gridCoordinate[0]++;
                    block.worldCoordinate += glm::vec3(0.5f, 0.0f, 0.0f);
                }
            }
        }
        keyIsPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (!keyWasPressed) {
            bool canMove = true;
            for (const auto& block : m_activePiece) {
                if (IsOccupied(block.gridCoordinate + glm::ivec3(-1, 0, 0))) {
                    canMove = false;
                    break;
                }
            }
            if (canMove) {
                for (auto& block: m_activePiece) {
                    block.gridCoordinate[0]--;
                    block.worldCoordinate += glm::vec3(-0.5f, 0.0f, 0.0f);
                }
            }
        }
        keyIsPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (!keyWasPressed) {
            bool canMove = true;
            for (const auto& block : m_activePiece) {
                if (IsOccupied(block.gridCoordinate + glm::ivec3(0, 1, 0))) {
                    canMove = false;
                    break;
                }
            }
            if (canMove) {
                for (auto& block: m_activePiece) {
                    block.gridCoordinate[1]++;
                    block.worldCoordinate += glm::vec3(0.0f, 0.5f, 0.0f);
                }
            }
        }
        keyIsPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (!keyWasPressed) {
            bool canMove = true;
            for (const auto& block : m_activePiece) {
                if (IsOccupied(block.gridCoordinate + glm::ivec3(0, -1, 0))) {
                    canMove = false;
                    break;
                }
            }
            if (canMove) {
                for (auto& block: m_activePiece) {
                    block.gridCoordinate[1]--;
                    block.worldCoordinate += glm::vec3(0.0f, -0.5f, 0.0f);
                }
            }
        }
        keyIsPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        if (!keyWasPressed) {
            bool canMove = true;
            for (const auto& block : m_activePiece) {
                if (ShouldBecomeSolid(block.gridCoordinate)) {
                    canMove = false;
                    break;
                }
            }
            if (canMove) {
                m_activeCubeLastMoveTime = glfwGetTime();
                for (auto& block: m_activePiece) {
                    block.gridCoordinate[2]++;
                    block.worldCoordinate += glm::vec3(0.0f, 0.0f, -0.5f);
                }
            } else {
                MakeActiveCubeSolid();
            }
        }
        keyIsPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!keyWasPressed) {

            m_activeCubeLastMoveTime = glfwGetTime();
            int distance = 0;
            bool collided = false;

            // Count up how far we can move
            while(!collided) {
                for (const auto& block : m_activePiece){
                    if (IsOccupied(block.gridCoordinate + glm::ivec3(0, 0, distance + 1))) {
                        collided = true;
                        break;
                    }
                }
                if(!collided)
                    distance ++;
            }

            // Apply the movement
            for (auto& block : m_activePiece) {
                block.gridCoordinate[2] += distance;
                block.worldCoordinate += glm::vec3(0.0f, 0.0f, -0.5f * distance);
            }
            MakeActiveCubeSolid();
        }
        keyIsPressed = true;
    }
    keyWasPressed = keyIsPressed;
}

void ExamApplication::RenderTunnel()
{
    m_tunnelShaderProgram->Bind();
    m_backWallVAO->Bind();

    // Common uniforms for all wals
    m_tunnelShaderProgram->UploadUniformMat4("u_ViewProjectionMatrix", m_camera->GetViewProjectionMatrix());
    m_tunnelShaderProgram->UploadUniformBool("u_textureEnabled", m_textureEnabled);
    m_tunnelShaderProgram->UploadUniformFloat1("u_ambientStrength", glm::vec1(m_globalIllumination));
    m_tunnelShaderProgram->UploadUniformFloat3("u_lightSourcePosition", m_lightSourcePos); // Light follow the active cube
    m_tunnelShaderProgram->UploadUniformFloat1("u_diffuseStr", glm::vec1(0.75f));
    m_tunnelShaderProgram->UploadUniformFloat3("u_cameraPosition", m_camera->GetPosition());
    m_tunnelShaderProgram->UploadUniformFloat1("u_specularStr", glm::vec1(0.5f));

    // Draw the back wall
    m_tunnelShaderProgram->UploadUniformMat4("u_tunnelModelMatrix", m_backWallModelMatrix);
    m_tunnelShaderProgram->UploadUniformBool("u_usingInstancing", false);
    m_tunnelShaderProgram->UploadUniformFloat2("u_GridSize", {5.0f, 5.0f});
    RenderCommands::DrawIndex(m_backWallVAO, GL_TRIANGLES);

    // Use instanced rendering to draw all side walls in one call
    m_tunnelVAO->Bind();
    m_tunnelShaderProgram->UploadUniformFloat2("u_GridSize", {5.0f, 10.0f});
    m_tunnelShaderProgram->UploadUniformMat4("u_tunnelModelMatrices[0]", m_topWallModelMatrix);
    m_tunnelShaderProgram->UploadUniformMat4("u_tunnelModelMatrices[1]", m_leftWallModelMatrix);
    m_tunnelShaderProgram->UploadUniformMat4("u_tunnelModelMatrices[2]", m_rightWallModelMatrix);
    m_tunnelShaderProgram->UploadUniformMat4("u_tunnelModelMatrices[3]", m_bottomWallModelMatrix);
    m_tunnelShaderProgram->UploadUniformBool("u_usingInstancing", true);
    RenderCommands::DrawIndexInstanced(m_tunnelVAO, GL_TRIANGLES, 4);
}

void ExamApplication::RenderActiveCube()
{
    if (m_activePiece.size() == 0)
        return;

    m_activeCubeShaderProgram->Bind();
    m_activeCubeVAO->Bind();

    // Common uniforms for active piece
    m_activeCubeShaderProgram->UploadUniformMat4("u_ViewProjectionMatrix", m_camera->GetViewProjectionMatrix());
    m_activeCubeShaderProgram->UploadUniformFloat1("u_ambientStrength", glm::vec1(m_globalIllumination));

    // Upload all model matrices to the array
    for (int i = 0; i < m_activePiece.size(); i++) {
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, m_activePiece[i].worldCoordinate);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
        m_activeCubeShaderProgram->UploadUniformMat4("u_activeCubeModelMatrices[" + std::to_string(i) + "]", modelMatrix);
    }
    // Draw the whole array (4 pieces) at once
    RenderCommands::DrawIndexInstanced(m_solidBlocksVAO, GL_TRIANGLES, m_activePiece.size());
}

void ExamApplication::RenderSolidBlocks()
{
    if (m_solidBlocks.size() == 0)
        return;

    m_solidBlocksShaderProgram->Bind();
    m_solidBlocksVAO->Bind();
    
    m_solidBlocksShaderProgram->UploadUniformMat4("u_ViewProjectionMatrix", m_camera->GetViewProjectionMatrix());
    m_solidBlocksShaderProgram->UploadUniformInt("u_textureEnabled", (int)m_textureEnabled);
    m_solidBlocksShaderProgram->UploadUniformFloat1("u_ambientStrength", glm::vec1(m_globalIllumination));
    m_solidBlocksShaderProgram->UploadUniformFloat3("u_lightSourcePosition", m_cubeModelMatrix[3]); // Light follow the active cube
    m_solidBlocksShaderProgram->UploadUniformFloat1("u_diffuseStr", glm::vec1(0.5f)); // Hard coded, make var if want to change
    m_solidBlocksShaderProgram->UploadUniformFloat3("u_cameraPosition", m_camera->GetPosition());
    m_solidBlocksShaderProgram->UploadUniformFloat1("u_specularStr", glm::vec1(1.0f));

    // // OpenGL uses column - major ordering : http://www.theamazingking.com/ogl-matrix.php
    // // Upload all matrices and colors to array used for instanced drawing
    // std::vector<float> data;

    // for (const auto& block : m_solidBlocks) {
    //     glm::mat4 modelMatrix = glm::mat4(1.0f);
    //     modelMatrix = glm::translate(modelMatrix, block.worldCoordinate);
    //     modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));

    //     // Extract model matrix values in COLUMN - MAJOR order
    //     for (int col = 0; col < 4; col++) {
    //         for (int row = 0; row < 4; row++) {
    //             data.push_back(modelMatrix[col][row]);
    //         }
    //     }
    //     // Then we add all the color values
    //     data.push_back(block.color.x);
    //     data.push_back(block.color.y);
    //     data.push_back(block.color.z);

    //     m_solidBlocksShaderProgram->UploadUniformMat4("u_solidBlockModelMatrices[" + std::to_string(i) + "]", modelMatrix);
    //     m_solidBlocksShaderProgram->UploadUniformFloat3("u_blockColors[" + std::to_string(i) + "]", m_solidBlocks[i].color); 
    // }
    // RenderCommands::DrawIndexInstanced(m_solidBlocksVAO, GL_TRIANGLES, m_solidBlocks.size());

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
    double time = glfwGetTime();
    if (time - m_activeCubeLastMoveTime >= 2.0f) {
        bool canMove = true;
        for (const auto& block : m_activePiece) {
            if(ShouldBecomeSolid(block.gridCoordinate)) {
                canMove = false;
                break;
            }
        }
        if (canMove) {
            for (auto& block : m_activePiece){
                block.gridCoordinate[2]++;
                block.worldCoordinate += glm::vec3(0.0f, 0.0f, -0.5f);
            } 
            m_activeCubeLastMoveTime = time;
        } else {
            MakeActiveCubeSolid();
        }
    }
}

void ExamApplication::RespawnActiveBlock()
{
    // Clear the vector with pieces
    m_activePiece.clear();

    // Pick a random piece
    int randomNum = rand() % 3;
    switch (randomNum)
    {
    case 0:
        MakeLPiece();
        break;
    case 1:
        MakeTPiece();
        break;
    case 2:
        MakeZPiece();
        break;
    default:
        MakeLPiece();
    }
}

bool ExamApplication::ShouldBecomeSolid(glm::ivec3 gridCoordinate)
{
    // If we are at end of tunnel, return true
    if (gridCoordinate[2] >= 9) {
        return true;
    }
    // Check one gridspace ahead of us
    return IsOccupied(gridCoordinate + glm::ivec3(0, 0, 1));
}

void ExamApplication::MakeActiveCubeSolid()
{
    for (const auto& block : m_activePiece) {
        SolidBlock solidBlock;
        // Copy the grid coordinate from the active cube
        solidBlock.gridCoordinate = block.gridCoordinate;
        // Get the world coordinate to the new solid block by extracting it from the model matrix
        solidBlock.worldCoordinate = block.worldCoordinate;
        // Get color for block based on z position
        solidBlock.color = GetColorForSolidBlock(solidBlock.gridCoordinate[2]);
        // Add the solid block to the vector of solid blocks
        m_solidBlocks.push_back(solidBlock);
        // Reset the position of the active block
    }
    RespawnActiveBlock();
}

bool ExamApplication::IsOccupied(glm::ivec3 gridCoordinate)
{
    // Check edges of tunnel
    if (gridCoordinate[0] < 0 || gridCoordinate[0] > 4 || 
        gridCoordinate[1] < 0 || gridCoordinate[1] > 4 ||
        gridCoordinate[2] > 9) {
            return true;
        }
    // Go trough all solid blocks and check for collision
    for (const auto& block : m_solidBlocks) {
        if (block.gridCoordinate == gridCoordinate) {
            return true;
        }
    }
    // No collision
    return false;
}

glm::vec3 ExamApplication::GetColorForSolidBlock(int zPos)
{
    auto colorInt = zPos % 5;
    glm::vec3 color;
    switch (colorInt)
    {
    case 0:
        color = glm::vec3(0.8, 0.2, 0.2); // Red
        break;
    case 1:
        color = glm::vec3(0.2, 0.8, 0.2); // Green
        break;
    case 2:
        color = glm::vec3(0.8, 0.2, 0.8); // Magenta
        break;
    case 3:
        color = glm::vec3(0.8, 0.8, 0.2); // Yellow
        break;
    case 4:
        color = glm::vec3(0.2, 0.8, 0.8); // Cyan
        break;
    default:
        color = glm::vec3(1.0, 1.0, 1.0); // Fallback, white for now
        break;
    }
    return color;
}

void ExamApplication::MakeLPiece()
{
    ActiveBlock block;
    block.gridCoordinate = glm::ivec3(1, 2, 0);
    block.worldCoordinate = glm::vec3(-0.5f, 0.0f, 2.0f);
    m_activePiece.push_back(block);

    block.gridCoordinate = glm::ivec3(2, 2, 0);
    block.worldCoordinate = glm::vec3(0.0f, 0.0f, 2.0f);
    m_activePiece.push_back(block);

    block.gridCoordinate = glm::ivec3(2, 2, 1);
    block.worldCoordinate = glm::vec3(0.0f, 0.0f, 1.5f);
    m_activePiece.push_back(block);

    block.gridCoordinate = glm::ivec3(2, 2, 2);
    block.worldCoordinate = glm::vec3(0.0f, 0.0f, 1.0f);
    m_activePiece.push_back(block);
}

void ExamApplication::MakeTPiece()
{
    ActiveBlock block;
    block.gridCoordinate = glm::ivec3(2, 2, 0);
    block.worldCoordinate = glm::vec3(0.0f, 0.0f, 2.0f);
    m_activePiece.push_back(block);

    block.gridCoordinate = glm::ivec3(1, 2, 0);
    block.worldCoordinate = glm::vec3(-0.5f, 0.0f, 2.0f);
    m_activePiece.push_back(block);

    block.gridCoordinate = glm::ivec3(3, 2, 0);
    block.worldCoordinate = glm::vec3(0.5f, 0.0f, 2.0f);
    m_activePiece.push_back(block);

    block.gridCoordinate = glm::ivec3(2, 1, 0);
    block.worldCoordinate = glm::vec3(0.0f, -0.5f, 2.0f);
    m_activePiece.push_back(block);
}

void ExamApplication::MakeZPiece()
{
    ActiveBlock block;
    block.gridCoordinate = glm::ivec3(1, 2, 0);
    block.worldCoordinate = glm::vec3(-0.5f, 0.0f, 2.0f);
    m_activePiece.push_back(block);

    block.gridCoordinate = glm::ivec3(2, 2, 0);
    block.worldCoordinate = glm::vec3(0.0f, 0.0f, 2.0f);
    m_activePiece.push_back(block);

    block.gridCoordinate = glm::ivec3(2, 3, 0);
    block.worldCoordinate = glm::vec3(0.0f, 0.5f, 2.0f);
    m_activePiece.push_back(block);

    block.gridCoordinate = glm::ivec3(3, 3, 0);
    block.worldCoordinate = glm::vec3(0.5f, 0.5f, 2.0f);
    m_activePiece.push_back(block);
}

void ExamApplication::InputHandleRotation(GLFWwindow *window)
{
    static bool keyWasPressed = false;
    bool keyIsPressed = false;  

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        if (!keyWasPressed) {
            PitchActivePiece(true);
        }
        keyIsPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        if (!keyWasPressed) {
            PitchActivePiece(false);
        }
        keyIsPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        if (!keyWasPressed) {
            RollActivePiece(true);
        }
        keyIsPressed = true;
    }
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        if (!keyWasPressed) {
            RollActivePiece(false);
        }
        keyIsPressed = true;
    }
        else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        if (!keyWasPressed) {
            YawActivePiece(true);
        }
        keyIsPressed = true;
    }
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        if (!keyWasPressed) {
            YawActivePiece(false);
        }
        keyIsPressed = true;
    }
    
    keyWasPressed = keyIsPressed;
}

void ExamApplication::PitchActivePiece(bool positive)
{
    // Set direction based on bool flag
    int direction = positive ? 1 : -1;

    // Choose a pivot point. 2nd block in the piece, should be middle
    glm::ivec3 pivotPointGridCoordinate = m_activePiece[1].gridCoordinate;

    // Coordinates to rotate to for each block
    std::vector<glm::ivec3> rotatedGridCoordinates;

    // Go trough each block and find their relative position compared to pivot point
    for (const auto& block : m_activePiece) {
        glm::ivec3 relativeGridCoordinate = block.gridCoordinate - pivotPointGridCoordinate;

        /* Get the new values. The axis we rotate around does not change for any block
         * Relative position on the other two axis swaps */
        int newY = (-relativeGridCoordinate[2] * direction);
        int newZ = ( relativeGridCoordinate[1] * direction); 

        // Add the new relative position to the pivot point coordinate to place it back on the grid
        glm::ivec3 rotated = pivotPointGridCoordinate + glm::ivec3(relativeGridCoordinate[0], newY, newZ); 
        
        // Check if new position is occupied
        if (IsOccupied(rotated)){
            return;
        }
        // Add coordinate to vector
        rotatedGridCoordinates.push_back(rotated);
    }

    // Apply the rotation
    for (int i = 0; i < m_activePiece.size(); i++) {
        m_activePiece[i].gridCoordinate = rotatedGridCoordinates[i];
        // World coordinates are half the size of grid coordinates
        // They also have an offset from origin
        m_activePiece[i].worldCoordinate = glm::vec3(
            (rotatedGridCoordinates[i].x - 2) / 2.0f,
            (rotatedGridCoordinates[i].y - 2) / 2.0f,
            (2 - rotatedGridCoordinates[i].z) / 2.0f
        );
    }
}

void ExamApplication::RollActivePiece(bool positive)
{
    
}

void ExamApplication::YawActivePiece(bool positive)
{
    
}