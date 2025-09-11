#version 460 core

in vec2 vTexCoord;       // From vertex shader
out vec4 FragColor;

layout(binding = 0) uniform sampler2D u_Texture; // Texture sampler
//uniform vec4 u_Color;
void main()
{
		  FragColor = texture(u_Texture, vTexCoord);
}