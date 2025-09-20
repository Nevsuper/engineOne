#pragma once
#include<glad/gl.h>
#include "glTypes.h"
#include "engineOne/utils/utils.h"

// Simplified format categories
enum class TextureInternalFormat : GLenum
{
	// Basic formats
	R8 = GL_R8,
	RG8 = GL_RG8,
	RGB8 = GL_RGB8,
	RGBA8 = GL_RGBA8,

	// Float formats
	R16F = GL_R16F,
	RG16F = GL_RG16F,
	RGB16F = GL_RGB16F,
	RGBA16F = GL_RGBA16F,

	// Depth formats
	DEPTH24 = GL_DEPTH_COMPONENT24,
	DEPTH32F = GL_DEPTH_COMPONENT32F,
	DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8
};

enum class TextureBaseFormat : GLenum
{
	RED = GL_RED,
	RG = GL_RG,
	RGB = GL_RGB,
	RGBA = GL_RGBA,
	DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
	DEPTH_STENCIL = GL_DEPTH_STENCIL
};

enum class PixelDataType : GLenum
{
	UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
	BYTE = GL_BYTE,
	UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
	SHORT = GL_SHORT,
	UNSIGNED_INT = GL_UNSIGNED_INT,
	INT = GL_INT,
	FLOAT = GL_FLOAT
};

inline constexpr GLenum TextureInternalFormatToGLenum(TextureInternalFormat format) noexcept
{
	return enumValue(format);
}

inline constexpr GLenum TextureBaseFormatToGLenum(TextureBaseFormat format) noexcept
{
	return enumValue(format);
}

enum class pixelDataType : GLenum
{
	UNSIGNEDBYTE = GL_UNSIGNED_BYTE,
	BYTE = GL_BYTE,
	UNSIGNEDSHORT = GL_UNSIGNED_SHORT,
	SHORT = GL_SHORT,
	UNSIGNEDINT = GL_UNSIGNED_INT,
	INT = GL_INT,
	FLOAT = GL_FLOAT,
	UNSIGNEDBYTE332 = GL_UNSIGNED_BYTE_3_3_2,
	UNSIGNEDBYTE233REV = GL_UNSIGNED_BYTE_2_3_3_REV,
	UNSIGNEDSHORT565 = GL_UNSIGNED_SHORT_5_6_5,
	UNSIGNEDSHORT565REV = GL_UNSIGNED_SHORT_5_6_5_REV,
	UNSIGNEDSHORT4444 = GL_UNSIGNED_SHORT_4_4_4_4,
	UNSIGNEDSHORT4444REV = GL_UNSIGNED_SHORT_4_4_4_4_REV,
	UNSIGNEDSHORT5551 = GL_UNSIGNED_SHORT_5_5_5_1,
	UNSIGNEDSHORT1555REV = GL_UNSIGNED_SHORT_1_5_5_5_REV,
	UNSIGNEDINT8888 = GL_UNSIGNED_INT_8_8_8_8,
	UNSIGNEDINT8888REV = GL_UNSIGNED_INT_8_8_8_8_REV,
	UNSIGNEDINT1010102 = GL_UNSIGNED_INT_10_10_10_2,
	UNSIGNEDINT2101010REV = GL_UNSIGNED_INT_2_10_10_10_REV
	
};

inline constexpr GLenum PixelDataTypeToGLenum(pixelDataType format) noexcept
{
	return enumValue(format);
}


struct Texture2DDataCreateInfo
{
	int width = 0, height = 0;
	void* data = nullptr;
	pixelDataType type;
	TextureInternalFormat internalFormat;
	int mipLevels = 1;
};
class Texture2D
{
public:
	Texture2D(int width, int height, void* data, pixelDataType type, TextureInternalFormat internalFormat, int mipLevels = 1) noexcept;
	Texture2D(const Texture2DDataCreateInfo& createInfo) noexcept;
	~Texture2D() noexcept;

	//delete copy constructor and copy assignment operator
	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;
	//move constructor
	Texture2D(Texture2D&& other) noexcept;
	//move assignment operator
	Texture2D& operator=(Texture2D&& other) noexcept;

	void SubImage(int width, int height, const void* data, pixelDataType type, TextureBaseFormat baseFormat, int level = 0, int xOfffset = 0, int yOffset = 0) const noexcept;

	void Bind(unsigned int slot = 0) const noexcept;
	void Unbind(unsigned int slot = 0) const noexcept;

private:
	unsigned int m_ID;
};

