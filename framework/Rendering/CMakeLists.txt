add_library(Rendering 
    VertexBuffer.cpp
    IndexBuffer.cpp
    VertexArray.cpp
    Shader.cpp
    TextureManager.cpp
    PerspectiveCamera.cpp
    OrthographicCamera.cpp
)

add_library(Framework::Rendering ALIAS Rendering)

target_include_directories(Rendering PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Rendering PUBLIC glad glm OpenGL::GL stb)
