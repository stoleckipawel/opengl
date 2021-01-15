#include "GL/glew.h"

#include "VertexBuffer.h"
#include "Assert.h"


VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererID));// generate name for the buffer
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));//bind buffer, set the type
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));//creates storage wrzuca dane, usuwa poprzednie
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));//bind buffer, set the type
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));//unbinds
}
