#version 460 core

in vec2 vTexCoord;       // From vertex shader
in vec3 vNormal;         // From vertex shader
in vec3 vFragPos;        // From vertex shader

out vec4 FragColor;

uniform vec3 u_lightPos;    // Light position in world space
uniform vec3 u_viewPos;     // Camera position in world space
uniform vec4 u_Color;       // Base object color

// Lighting parameters
uniform vec3 u_lightColor = vec3(1.0, 1.0, 1.0);  // White light
uniform float u_ambientStrength = 0.1;            // Ambient intensity
uniform float u_specularStrength = 0.5;           // Specular intensity
uniform float u_shininess = 32.0;                 // Shininess factor

void main()
{
    // --- Normalize input vectors ---
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(u_lightPos - vFragPos);
    vec3 viewDir  = normalize(u_viewPos - vFragPos);

    // --- Ambient ---
    vec3 ambient = u_ambientStrength * u_lightColor;

    // --- Diffuse ---
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_lightColor;

    // --- Specular (Blinn-Phong) ---
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), u_shininess);
    vec3 specular = u_specularStrength * spec * u_lightColor;

    // --- Combine ---
    vec3 result = (ambient + diffuse + specular) * vec3(u_Color);

    FragColor = vec4(result, u_Color.a);
}
