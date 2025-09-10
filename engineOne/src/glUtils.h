#pragma once
#include<glad/gl.h>

inline void SafeDeleteGLBuffer(GLuint& id)
{
	if (id != 0)
	{
		glDeleteBuffers(1, &id);
		id = 0;
	}
}

inline void SafeDeleteGLProgram(GLuint& id) 
{
	if (id != 0) 
	{
		glDeleteProgram(id);
		id = 0;
	}
}

inline void SafeDeleteGLShader(GLuint& id) 
{
	if (id != 0) 
	{
		glDeleteShader(id);
		id = 0;
	}
}

inline void SafeDeleteGLTexture(GLuint& id)
{
	if (id != 0) 
	{
		glDeleteTextures(1, &id);
		id = 0;
	}
}

inline void SafeDeleteGLVertexArray(GLuint& id) 
{
	if (id != 0) 
	{
		glDeleteVertexArrays(1, &id);
		id = 0;
	}
}

