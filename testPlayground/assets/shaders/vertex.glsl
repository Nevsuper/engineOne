#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 vNormal;      // Pass normal to fragment shader
out vec3 vFragPos;     // Pass world-space position to fragment shader (optional for lighting)

uniform mat4 u_Model,u_View, u_Projection;

void main()
{
	vec4 worldPos = u_Model * vec4(aPos, 1.0);
	gl_Position = u_Projection*u_View*u_Model*vec4(aPos, 1.0);
	vFragPos = worldPos.xyz;           // Needed if you do lighting in world space
    vNormal = aNormal; // Correct normal transform
}