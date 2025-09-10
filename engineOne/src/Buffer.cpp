#include "Buffer.h"
#include "glUtils.h"

VertexBuffer::VertexBuffer() noexcept
	:
	m_ID(0)
{
	glGenBuffers(1, &m_ID);
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size, BufferUsage usage) noexcept
	:
	m_ID(0)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, BufferUsageToGLenum(usage));
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
	: m_ID(other.m_ID)
{
	other.m_ID = 0;
}

VertexBuffer::~VertexBuffer()
{
	SafeDeleteGLBuffer(m_ID);
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
	if (this != &other)
	{
		SafeDeleteGLBuffer(m_ID);
		m_ID = other.m_ID;
		other.m_ID = 0;
	}
	return *this;
}



void VertexBuffer::Bind() const noexcept
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VertexBuffer::Unbind() const noexcept
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void VertexBuffer::SetData(const void* data, unsigned int size, BufferUsage usage) const noexcept
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, BufferUsageToGLenum(usage));
}

IndexBuffer::IndexBuffer() noexcept
	:
	m_ID(0)
{
	glGenBuffers(1, &m_ID);
}

IndexBuffer::IndexBuffer(const void* data, unsigned int size, BufferUsage usage) noexcept
	:
	m_ID(0)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, BufferUsageToGLenum(usage));
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
	:
	m_ID(other.m_ID)
{
	other.m_ID = 0;
}


IndexBuffer::~IndexBuffer()
{
	SafeDeleteGLBuffer(m_ID);
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
	if (this != &other)
	{
		SafeDeleteGLBuffer(m_ID);
		m_ID = other.m_ID;
		other.m_ID = 0;
	}
	return *this;
}

void IndexBuffer::Bind() const noexcept
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void IndexBuffer::Unbind() const noexcept
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}