#pragma once

#include<Windows.h>

bool loadModernOpenGLFunctions(HINSTANCE hInstance);
void unloadModernOpenGLFunctions();

class GLLoader
{
public:
	 GLLoader(HINSTANCE hInstance) noexcept
		:
		m_IsLoaded(loadModernOpenGLFunctions(hInstance))
	{} 

	~GLLoader() noexcept { unloadModernOpenGLFunctions(); }

	GLLoader(const GLLoader&) = delete;
	GLLoader& operator=(const GLLoader&) = delete;
	GLLoader(GLLoader&&) = delete;
	GLLoader& operator=(GLLoader&&) = delete;

	bool isLoaded() const noexcept { return m_IsLoaded; }
private:
	bool m_IsLoaded;
};



