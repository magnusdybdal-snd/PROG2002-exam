#ifndef GLFWAPPLICATION_H_
#define GLFWAPPLICATION_H_


#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GLFWApplication
{
    public:
    GLFWApplication(
        const std::string &name, 
        const std::string &version,
        int windowWidth = 1280,
        int windowHeight = 720
    );
    virtual ~GLFWApplication();

    // Initialize GLFW, create window, initialize GLAD
    virtual unsigned Init();

    // Pure virtual function - must be implemented in subclass
    virtual unsigned Run() = 0;
    
    protected:

    GLFWwindow* GetWindow() const { return m_window; }

    private:
    std::string m_name;
    std::string m_version;
    int m_windowWidth = 1280;
    int m_windowHeight = 720;
    std::string m_windowName;
    GLFWwindow* m_window = nullptr;

    #ifndef __APPLE__
    // Error callbacks
    static void GLFWErrorCallback(int code, const char* description);
    static void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam);
    #endif // __APPLE__
};
#endif // GLFWAPPLICATION_H_