#include "pch.h"
#include "Buffer.h"
#include "glUtils.h"


VertexBuffer::VertexBuffer(const void* data, unsigned int size, BufferUsage usage) noexcept
	:
	BufferBase(data, size, usage)
{}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
	: BufferBase(std::move(other))
{}



VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
	BufferBase::operator=(std::move(other));
	return *this;
}



IndexBuffer::IndexBuffer(const void* data, unsigned int size, GLType type, BufferUsage usage) noexcept
	:
	 m_Count(size/getSizeofGLType(type)),m_Type(type),
	BufferBase(data, size, usage)
{
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
	:
	BufferBase(std::move(other))
{}



IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
	BufferBase::operator=(std::move(other));
	return *this;
}

