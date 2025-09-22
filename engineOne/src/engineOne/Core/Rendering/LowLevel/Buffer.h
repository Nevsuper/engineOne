#pragma once
#include <glad/gl.h>
#include "engineOne/utils/utils.h"
#include "glTypes.h"
#include "glUtils.h"
enum class BufferUsage : GLenum
{
	Static = GL_STATIC_DRAW,
	Dynamic = GL_DYNAMIC_DRAW,
	Stream = GL_STREAM_DRAW
};

//set default value to Static

constexpr BufferUsage DefaultBufferUsage = BufferUsage::Static;


constexpr GLenum BufferUsageToGLenum(BufferUsage usage) noexcept
{
	return enumValue(usage);
}

enum class BufferTargetType : GLenum
{
	Array = GL_ARRAY_BUFFER,//	Vertex attributes
	AtomicCounter = GL_ATOMIC_COUNTER_BUFFER,//	Atomic counter storage
	CopyRead = GL_COPY_READ_BUFFER,//	Buffer copy source
	CopyWrite = GL_COPY_WRITE_BUFFER,//	Buffer copy destination
	DispatchIndiretc = GL_DISPATCH_INDIRECT_BUFFER,//	Indirect compute dispatch commands
	DrawIndirect = GL_DRAW_INDIRECT_BUFFER,//	Indirect command arguments
	ElementArray = GL_ELEMENT_ARRAY_BUFFER,//	Vertex array indices
	PixelPack = GL_PIXEL_PACK_BUFFER,//	Pixel read target
	PixelUnpack = GL_PIXEL_UNPACK_BUFFER,//	Texture data source
	Query = GL_QUERY_BUFFER,//	Query result buffer
	ShaderStorage = GL_SHADER_STORAGE_BUFFER,//	Read - write storage for shaders
	Texture = GL_TEXTURE_BUFFER,//	Texture data buffer
	TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,//	Transform feedback buffer
	Unform = GL_UNIFORM_BUFFER,//
};

constexpr GLenum BufferTargetTypeToGLenum(BufferTargetType type) noexcept
{
	return enumValue(type);
}

template<class T>
class BufferBase
{
protected:
	BufferBase(const void* data, unsigned int size, BufferUsage usage = DefaultBufferUsage)
	{
		glGenBuffers(1, &m_ID);
		Bind();
		glBufferData(BufferTargetTypeToGLenum(T::getType()), size, data, BufferUsageToGLenum(usage));
	}

	~BufferBase(){	SafeDeleteGLBuffer(m_ID); }

	BufferBase(const BufferBase&) = delete;
	BufferBase& operator=(const BufferBase&) = delete;

	BufferBase(BufferBase&& other)
		: m_ID(other.m_ID)
	{
		other.m_ID = 0;
	}

	BufferBase& operator=(BufferBase&& other)
	{
		if (this != &other)
		{
			SafeDeleteGLBuffer(m_ID);
			m_ID = other.m_ID;
			other.m_ID = 0;
		}
		return *this;
	}


public:
	void Bind() const noexcept { glBindBuffer(BufferTargetTypeToGLenum(T::getType()), m_ID); }
	void Unbind() const noexcept { glBindBuffer(BufferTargetTypeToGLenum(T::getType()), 0); }

	unsigned int getID() const noexcept { return m_ID; }
protected:
	unsigned int m_ID;
};


#define DECLARE_BUFFER_TARGET_TYPE(type) \
static constexpr BufferTargetType getType() {return type;}

class VertexBuffer : public BufferBase<VertexBuffer>
{
public:
	DECLARE_BUFFER_TARGET_TYPE(BufferTargetType::Array);
	VertexBuffer(const void* data, unsigned int size, BufferUsage usage = DefaultBufferUsage) noexcept;
	//move constructor
	VertexBuffer(VertexBuffer&& other) noexcept;
	~VertexBuffer() = default;

	//move assignment operator
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;
	// delete copy	constructor and copy assignment operator
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;
};


class IndexBuffer : public BufferBase<IndexBuffer>
{
public:
	DECLARE_BUFFER_TARGET_TYPE(BufferTargetType::ElementArray);
	IndexBuffer(const void* data, unsigned int size, GLType type, BufferUsage usage = DefaultBufferUsage) noexcept;
	//move constructor
	IndexBuffer(IndexBuffer&& other) noexcept;
	~IndexBuffer() = default;

	//move assignment operator
	IndexBuffer& operator=(IndexBuffer&& other) noexcept;
	// delete copy	constructor and copy assignment operator
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	unsigned int getCount() const noexcept { return m_Count; }

private:
	unsigned int m_Count;
	GLType m_Type;
};

