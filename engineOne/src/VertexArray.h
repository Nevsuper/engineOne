#pragma once
#include "glTypes.h"
class VertexArray
{
public:
	VertexArray() noexcept;
	//move constructor
	VertexArray(VertexArray&& other) noexcept;

	~VertexArray();

	//move assignment operator
	VertexArray& operator=(VertexArray&& other) noexcept;

	// delete copy	constructor and copy assignment operator
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;

	void Bind() const noexcept;
	void Unbind() const noexcept;

	void addAttribute(unsigned int index, int size, GLType type, bool normalized, int stride, const void* pointer) const noexcept;

	inline unsigned int getID() const noexcept { return m_ID; }
private:
	unsigned int m_ID;
};

