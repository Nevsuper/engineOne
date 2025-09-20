#pragma once
#include <glad/gl.h>
#include "engineOne/utils/utils.h"
enum class BufferUsage : GLenum
{
	Static  =  GL_STATIC_DRAW,
	Dynamic = GL_DYNAMIC_DRAW,
	Stream  = GL_STREAM_DRAW
};

//set default value to Static

constexpr BufferUsage DefaultBufferUsage = BufferUsage::Static;


constexpr GLenum BufferUsageToGLenum(BufferUsage usage) noexcept
{
	return enumValue(usage);
}


class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size, BufferUsage usage = DefaultBufferUsage) noexcept;
	//move constructor
	VertexBuffer(VertexBuffer&& other) noexcept;
	~VertexBuffer();

	//move assignment operator
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;
	// delete copy	constructor and copy assignment operator
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

	void Bind() const noexcept;
	void Unbind() const noexcept;

	unsigned int getID() const noexcept { return m_ID; }

private:
	unsigned int m_ID;
};


class IndexBuffer
{
public:
	IndexBuffer(const void* data, unsigned int size, BufferUsage usage = DefaultBufferUsage) noexcept;
	//move constructor
	IndexBuffer(IndexBuffer&& other) noexcept;
	~IndexBuffer();

	//move assignment operator
	IndexBuffer& operator=(IndexBuffer&& other) noexcept;
	// delete copy	constructor and copy assignment operator
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	void Bind() const noexcept;
	void Unbind() const noexcept;

	 unsigned int getID() const noexcept { return m_ID; }
	 unsigned int getCount() const noexcept { return m_Count; }

private:
	unsigned int m_ID;
	unsigned int m_Count;
};

