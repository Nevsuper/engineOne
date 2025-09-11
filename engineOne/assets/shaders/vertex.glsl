#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord; // Texture coordinates

out vec2 vTexCoord; // Pass to fragment shader


uniform mat4 u_Model,u_View, u_Projection;

void main()
{
	gl_Position = u_Projection*u_View*u_Model*vec4(aPos, 1.0);
	vTexCoord = aTexCoord;
}