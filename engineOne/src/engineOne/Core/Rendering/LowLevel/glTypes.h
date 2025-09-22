#pragma once
#include<glad/gl.h>
#include "engineOne/utils/utils.h"

enum class GLType : GLenum
{
	UnsignedByte = GL_UNSIGNED_BYTE,
	Byte = GL_BYTE,
	UnsignedShort = GL_UNSIGNED_SHORT,
	Short = GL_SHORT,
	UnsignedInt = GL_UNSIGNED_INT,
	Int = GL_INT,
	Float = GL_FLOAT,
	Double = GL_DOUBLE,
};


inline constexpr GLenum GLTypeToGLenum(GLType type) noexcept
{
	return enumValue(type);
}



inline constexpr GLboolean boolToGLboolean(bool value) noexcept
{
	return static_cast<GLboolean>(value);
}

inline constexpr bool GLbooleanTobool(GLboolean value) noexcept
{
	return  static_cast<bool>(value);
}

template<typename T = size_t>
inline constexpr T getSizeofGLType(GLType type) noexcept
{
    switch (type)
    {
    case GLType::UnsignedByte: return static_cast<T>(sizeof(GLubyte));
    case GLType::Byte:         return static_cast<T>(sizeof(GLbyte));
    case GLType::UnsignedShort:return static_cast<T>(sizeof(GLushort));
    case GLType::Short:        return static_cast<T>(sizeof(GLshort));
    case GLType::UnsignedInt:  return static_cast<T>(sizeof(GLuint));
    case GLType::Int:          return static_cast<T>(sizeof(GLint));
    case GLType::Float:        return static_cast<T>(sizeof(GLfloat));
    case GLType::Double:       return static_cast<T>(sizeof(GLdouble));
    default:                   return static_cast<T>(0); // or static_assert(false)
    }
}

