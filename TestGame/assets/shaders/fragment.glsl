#version 460 core

in vec2 vTexCoord;       // From vertex shader
in vec3 vNormal;         // From vertex shader
in vec3 vFragPos;        // From vertex shader

out vec4 FragColor;

uniform sampler2D u_Texture;       // Diffuse texture
uniform sampler2D u_AlphaTexture;  // Alpha texture
uniform vec3 u_lightPos;           // Light position in world space
uniform vec3 u_viewPos;            // Camera position in world space

void main()
{
    // --- Texture ---
    vec3 texColor = texture(u_Texture, vTexCoord).rgb;
   
    float alpha = texture(u_AlphaTexture, vTexCoord).r;
    
    FragColor = vec4(texColor, alpha);
   
   //  --- Normal ---
    vec3 norm = normalize(vNormal);

    // --- Diffuse ---
    vec3 lightDir = normalize(u_lightPos - vFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = texColor * diff;

    // --- Ambient lighting (prevents completely black areas) ---
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * texColor;


    // --- Specular ---
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_viewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1.0); // white specular

    // --- Combine lighting ---
    vec3 result = diffuse + specular + ambient;


    FragColor = vec4(result, alpha);
}
