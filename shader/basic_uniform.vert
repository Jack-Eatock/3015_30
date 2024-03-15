#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

// Light Uniforms
uniform struct LightInfo
{
    vec4 Position;
    vec3 Diffuse;
    vec3 Specular;
    vec3 Ambient;
}Light;

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
uniform mat3 NormalMatrix;

void GetCamSpaceValues(out vec3 normal, out vec4 cameraPosition)
{
    cameraPosition = ModelViewMatrix * vec4(VertexPosition, 1.0f);
    normal = normalize(NormalMatrix*VertexNormal);
}

void main()
{
    // CamPos and Normal
    vec4 cameraPosition; vec3 normal;
    GetCamSpaceValues(normal,cameraPosition);

    // Light direction
    vec3 lightDirection = normalize(vec3(Light.Position - cameraPosition));

    // Diffuse
    float lightDirDotNormal = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = Light.Diffuse * Material.Diffuse * lightDirDotNormal;

    // Ambient
    vec3 ambient = Light.Ambient * Material.Ambient;

    // Specular
    vec3 specular = vec3(0.0);
    if (lightDirDotNormal > 0.0)
    {
        vec3 dirBackFromCam = normalize(-cameraPosition.xyz);
        vec3 reflection = reflect(-lightDirection, normal);
        specular = Light.Specular * Material.Specular * pow(max(dot(reflection, dirBackFromCam), 0.0), Material.Shininess);
    }

    // Calculating total light
    LightIntensity = diffuse + ambient + specular;

    // Setting the vertex pos
    gl_Position = ModelViewPerspective * vec4(VertexPosition,1.0);
}
