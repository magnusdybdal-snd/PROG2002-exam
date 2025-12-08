#include "ExamApplication.h"
#include "RenderCommands.h"

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

        glfwSwapBuffers(window);
    }

    std::cout << "Exiting..." << std::endl;
    return EXIT_SUCCESS;
}
