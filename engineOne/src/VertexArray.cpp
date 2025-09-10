#include "VertexArray.h"
#include<glad/gl.h>
#include "glUtils.h"


VertexArray::VertexArray() noexcept
	:
	m_ID(0)
{
	glGenVertexArrays(1, &m_ID);
}

VertexArray::VertexArray(VertexArray&& other) noexcept
	: m_ID(other.m_ID)
{
	other.m_ID = 0;
}

VertexArray::~VertexArray()
{
	SafeDeleteGLVertexArray(m_ID);
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
	if(this != &other)
	{
		SafeDeleteGLVertexArray(m_ID);
		m_ID = other.m_ID;
		other.m_ID = 0;
	}
	return *this;
}

void VertexArray::Bind() const noexcept
{
	glBindVertexArray(m_ID);
}

void VertexArray::Unbind() const noexcept
{
	glBindVertexArray(0);
}

void VertexArray::addAttribute(unsigned int index, int size, GLType type, bool normalized, int stride, const void* pointer) const noexcept
{
	glBindVertexArray(m_ID);
	glVertexAttribPointer(index, size, GLTypeToGLenum(type), bool(normalized), stride, pointer);
	glEnableVertexAttribArray(index);
}
