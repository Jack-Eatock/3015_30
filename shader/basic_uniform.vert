#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

flat out vec3 LightIntensity;
out vec3 Colour;

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


// Matrices
uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewPerspective;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

void GetCamSpaceValues(out vec3 normal, out vec3 cameraPosition)
{
    cameraPosition = (ModelViewMatrix * vec4(VertexPosition, 1.0f)).xyz;
    normal = normalize(NormalMatrix*VertexNormal);
}

vec3 phongModel(LightInfo Light, vec3 cameraPosition, vec3 normal)
{
    // Light direction
    vec3 lightDirection = normalize(vec3(Light.Position.xyz - cameraPosition));

    // Diffuse
    float lightDirDotNormal = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = Material.Diffuse * lightDirDotNormal;

    // Ambient
    vec3 ambient = Light.AmbientColour * Material.Ambient;

    // Specular
    vec3 specular = vec3(0.0);
    if (lightDirDotNormal > 0.0)
    {
        vec3 dirBackFromCam = normalize(-cameraPosition.xyz);
        vec3 reflection = reflect(-lightDirection, normal);
        specular = Material.Specular * pow(max(dot(reflection, dirBackFromCam), 0.0), Material.Shininess);
    }

    // Calculating total light
    return ambient + (diffuse + specular) * Light.Colour;
}

void main()
{
    // CamPos and Normal
    vec3 cameraPosition; vec3 normal;
    GetCamSpaceValues(normal,cameraPosition);

    Colour = vec3(0.0);
    for (int i = 0; i < Lights.length; i++)
        Colour += phongModel(Lights[i], cameraPosition, normal);
   
    // Setting the vertex pos
    gl_Position = ModelViewPerspective * vec4(VertexPosition,1.0);
}
