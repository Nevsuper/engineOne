#pragma once
#include<glad/gl.h>
#include "utils.h"

enum class GLType : GLenum
{
	Float = GL_FLOAT,
	Double = GL_DOUBLE,
	Int = GL_INT,
	UnsignedInt = GL_UNSIGNED_INT,
	Short = GL_SHORT,
	UnsignedShort = GL_UNSIGNED_SHORT,
	Byte = GL_BYTE,
	UnsignedByte = GL_UNSIGNED_BYTE
};


constexpr GLenum GLTypeToGLenum(GLType type) noexcept
{
	return enumValue(type);
}

inline GLboolean boolToGLboolean(bool value) noexcept
{
	return static_cast<GLboolean>(value);
}

inline bool GLbooleanTobool(GLboolean value) noexcept
{
	return  static_cast<bool>(value);
}
