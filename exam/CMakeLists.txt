cmake_minimum_required(VERSION 3.15)

project(exam)

find_package(OpenGL REQUIRED)

add_executable(exam
main.cpp
ExamApplication.cpp
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/wall_texture.jpeg
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/wall_texture.jpeg)

#  add_custom_command(
#      TARGET ${PROJECT_NAME} POST_BUILD
#      COMMAND ${CMAKE_COMMAND} -E copy
#     ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/chesspiece_texture.jpg
#      ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/chesspiece_texture.jpg)

 target_compile_definitions(${PROJECT_NAME} PRIVATE
     TEXTURES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/")

# glfw, glad and OpenGL::GL are linked trough GLFWApplication
target_link_libraries(exam 
PRIVATE
Framework::GeometricTools
Framework::GLFWApplication
Framework::Rendering
)