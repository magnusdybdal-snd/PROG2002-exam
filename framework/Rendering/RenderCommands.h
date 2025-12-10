#ifndef RENDERCOMMANDS_H_
#define RENDERCOMMANDS_H_

#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "VertexArray.h"

namespace RenderCommands
{
    inline void Clear(GLuint mode = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    {
        glClear(mode);
    }

    inline void SetPolygonMode(GLenum face, GLenum mode)
    {
        glPolygonMode(face, mode);
    }

    inline void DrawIndex(const std::shared_ptr<VertexArray>& vao, GLenum primitive)
    {
        glDrawElements(primitive, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    // https://learnopengl.com/Advanced-OpenGL/Instancing
    inline void DrawIndexInstanced(const std::shared_ptr<VertexArray>& vao, GLenum primitive, GL_UNSIGNED_INT instanceCount)
    {
        glDrawElementsInstanced(primitive, vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr, instanceCount);
    }

    inline void SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.x, color.y, color.z, color.w);
    }

    inline void SetWireframeMode(GLenum face = GL_FRONT_AND_BACK)
    {
        glPolygonMode(face, GL_LINE);
    }

    inline void SetSolidMode(GLenum face = GL_FRONT_AND_BACK)
    {
        glPolygonMode(face, GL_FILL);
    }

    inline void SetLineWidth(GLfloat width) 
    {
        glLineWidth(width);
    }
}

#endif // RENDERCOMMANDS_H_