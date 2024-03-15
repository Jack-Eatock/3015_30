#version 460

layout (location = 0) out vec4 FragColor;

in vec3 Position;
in vec3 Normal;

// Light Uniforms
uniform struct LightInfo
{
    vec4 Position;
    vec3 Colour;
    vec3 AmbientColour;
}Lights[3];

// Material Uniforms
uniform struct MaterialInfo
{
    vec3 Diffuse;
    vec3 Ambient;
    vec3 Specular;
    float Shininess;
}Material;

vec3 phongModel(LightInfo Light, vec3 position, vec3 normal)
{
    // Light direction
    vec3 lightDirection = normalize(vec3(Light.Position.xyz - position));

    // Diffuse
    float lightDirDotNormal = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = Material.Diffuse * lightDirDotNormal;

    // Ambient
    vec3 ambient = Light.AmbientColour * Material.Ambient;

    // Specular
    vec3 specular = vec3(0.0);
    if (lightDirDotNormal > 0.0)
    {
        vec3 dirBackFromCam = normalize(-position.xyz);
        vec3 reflection = reflect(-lightDirection, normal);
        specular = Material.Specular * pow(max(dot(reflection, dirBackFromCam), 0.0), Material.Shininess);
    }

    // Calculating total light
    return ambient + (diffuse + specular) * Light.Colour;
}


void main() {
   
    // Calculate colour from each light.
    vec3 Colour = vec3(0.0);
    for (int i = 0; i < Lights.length; i++)
        Colour += phongModel(Lights[i], Position, Normal);

    FragColor = vec4(Colour, 1.0);
}
