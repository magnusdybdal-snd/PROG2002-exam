#include "VertexArray.h"
#include <iostream>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_vertexArrayID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_vertexArrayID);
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_vertexArrayID);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer)
{
    // Safeguard - unsure about this one
    if (vertexBuffer->GetLayout().GetAttributes().size() == 0)
    {
        std::cerr << "Error: VertexBuffer has no layout defined!" << std::endl;
        return;
    }

    // Bind the VAO
    glBindVertexArray(m_vertexArrayID);

    // Bind the vertex buffer
    vertexBuffer->Bind();

    // Get the layout
    const BufferLayout &layout = vertexBuffer->GetLayout();

    // Set up vertex attributes based on the layout
    GLuint index = 0;
    for (const auto &attribute : layout)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index,
            ShaderDataTypeComponentCount(attribute.Type),   // Component count
            ShaderDataTypeToOpenGLBaseType(attribute.Type), // Type
            attribute.Normalized,                           // Normalized
            layout.GetStride(),                             // Stride
            (const void*)(intptr_t)attribute.Offset         // Offset
        );
        index++;
    }

    // Store the vertex buffer
    VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer)
{
    // Bind the VAO
    glBindVertexArray(m_vertexArrayID);

    // Bind the index buffer
    indexBuffer->Bind();

    // Store the index buffer
    IdxBuffer = indexBuffer;
}