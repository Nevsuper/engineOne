#include "pch.h"
#include "Texture.h"
#include "glUtils.h"
#include <cmath>
#include <algorithm>

TextureBaseFormat GetBaseFormatFromInternalFormat(TextureInternalFormat internalFormat) {
    switch (internalFormat) {
    case TextureInternalFormat::R8:
    case TextureInternalFormat::R16F:
        return TextureBaseFormat::RED;

    case TextureInternalFormat::RG8:
    case TextureInternalFormat::RG16F:
        return TextureBaseFormat::RG;

    case TextureInternalFormat::RGB8:
    case TextureInternalFormat::RGB16F:
        return TextureBaseFormat::RGB;

    case TextureInternalFormat::RGBA8:
    case TextureInternalFormat::RGBA16F:
        return TextureBaseFormat::RGBA;

    case TextureInternalFormat::DEPTH24:
    case TextureInternalFormat::DEPTH32F:
        return TextureBaseFormat::DEPTH_COMPONENT;

    case TextureInternalFormat::DEPTH24_STENCIL8:
        return TextureBaseFormat::DEPTH_STENCIL;

    default:
        return TextureBaseFormat::RGBA; // Fallback
    }
}

Texture2D::Texture2D(int width, int height, void* data, pixelDataType type, TextureInternalFormat internalFormat, int mipLevels) noexcept
	:
	Texture2D(Texture2DDataCreateInfo(
		width,height,data,type, internalFormat,mipLevels
	))
{}

Texture2D::Texture2D(const Texture2DDataCreateInfo& createInfo) noexcept
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
	// After creating the texture and before SubImage
	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureStorage2D(m_ID, createInfo.mipLevels, TextureInternalFormatToGLenum(createInfo.internalFormat), createInfo.width, createInfo.height);
	TextureBaseFormat baseFormat = GetBaseFormatFromInternalFormat(createInfo.internalFormat);

	SubImage(createInfo.width, createInfo.height, createInfo.data, createInfo.type, baseFormat);
}

Texture2D::~Texture2D() noexcept
{
	SafeDeleteGLTexture(m_ID);
}

Texture2D::Texture2D(Texture2D&& other) noexcept
{
	m_ID = other.m_ID;
	other.m_ID = 0;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
{
	if(this != &other)
	{
		SafeDeleteGLTexture(m_ID);
		m_ID = other.m_ID;
		other.m_ID = 0;
	}
	return *this;
}

void Texture2D::SubImage(int width, int height, const void* data, pixelDataType type, TextureBaseFormat baseFormat, int level, int xOfffset, int yOffset) const noexcept
{
	glTextureSubImage2D(m_ID, level, xOfffset, yOffset, width, height, TextureBaseFormatToGLenum(baseFormat), PixelDataTypeToGLenum(type), data);
}



void Texture2D::Bind(unsigned int slot) const noexcept
{
	glBindTextureUnit(slot, m_ID);
}

void Texture2D::Unbind(unsigned int slot) const noexcept
{
	glBindTextureUnit(slot, 0);
}
