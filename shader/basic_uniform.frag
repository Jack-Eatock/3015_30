#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;
layout (binding = 0 ) uniform sampler2D Tex1;
layout (binding = 1 ) uniform sampler2D Tex2;

// Toon Shader
const int levels = 4;
const float scaleFactor = 1.0/levels;

// Fog
uniform struct FogInfo
{
    float MaxDist;
    float MinDist;
    vec3 Color;
}Fog;

// Light Uniforms
uniform struct SpotLightInfo
{
    vec3 Position;
    vec3 Colour;
    vec3 AmbientColour;
    vec3 Direction;
    float Exponent;
    float Cuttoff;
}SpotLight;

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

vec3 BlinnPhong(LightInfo Light, vec3 position, vec3 normal, vec3 texColor)
{
    // Light direction
    vec3 lightDirection = normalize(vec3(Light.Position.xyz - position));

    // Diffuse
    float lightDirDotNormal = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = Material.Diffuse * floor(lightDirDotNormal * levels) * scaleFactor;
    //vec3 diffuse = Material.Diffuse * lightDirDotNormal; (No toon shading)
    if (texColor.r > 0 || texColor.g > 0 || texColor.b > 0 )
        diffuse = texColor * floor(lightDirDotNormal * levels) * scaleFactor;

    // Ambient
    vec3 ambient = Light.AmbientColour * Material.Ambient;
    if (texColor.r > 0 || texColor.g > 0 || texColor.b > 0)
         ambient = Light.AmbientColour * texColor;
        
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

vec3 BlinnPhongSpot(SpotLightInfo Light, vec3 position, vec3 normal,  vec3 texColor)
{
    // Light direction
    vec3 lightDirection = normalize(Light.Position - position);

    // Spot Circle
    float cosAng = dot(-lightDirection, normalize(Light.Direction));
    float angle = acos(cosAng);
    float spotScale = 0;
    if (angle >= 0.0 && angle < Light.Cuttoff)
        spotScale = pow(cosAng, Light.Exponent);

    // Diffuse
    float lightDirDotNormal = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = Material.Diffuse * floor(lightDirDotNormal * levels) * scaleFactor;
    //vec3 diffuse = Material.Diffuse * lightDirDotNormal; (No toon shading)
    if (texColor.r > 0 || texColor.g > 0 || texColor.b > 0 )
        diffuse = texColor * lightDirDotNormal; //floor(lightDirDotNormal * levels) * scaleFactor;

    // Ambient
    vec3 ambient = Light.AmbientColour * Material.Ambient;
    if (texColor.r > 0 || texColor.g > 0 || texColor.b > 0)
         ambient = Light.AmbientColour * texColor;

    // Specular
    vec3 specular = vec3(0.0);
    if (lightDirDotNormal > 0.0)
    {
        vec3 dirBackFromCam = normalize(-position.xyz);
        vec3 reflection =  normalize(dirBackFromCam + lightDirection);
        specular = Material.Specular * pow(max(dot(reflection, normal), 0.0), Material.Shininess);
    }

    // Calculating total light
    return ambient + spotScale * (diffuse + specular) * Light.Colour;
}


void main() 
{
    // Depth of the scene
    float dist = abs(Position.z);
    float fogFactor = (Fog.MaxDist - dist)/(Fog.MaxDist - Fog.MinDist);
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    vec3 shadeColor = vec3(0);

    // Texture
    vec3 texColor = texture(Tex1, TexCoord).rgb;
    vec4 texColor2 = texture(Tex2, TexCoord);
    texColor = mix(texColor, texColor2.rgb, texColor2.a);

    // Calculate each light.
    for (int i = 0; i < Lights.length; i++)
        shadeColor += BlinnPhong(Lights[i], Position, Normal, texColor);

    // Calculate each Spot light
    //shadeColor += BlinnPhongSpot(SpotLight, Position, Normal, texColor);

    // Mix light and fog
    vec3 Colour = mix(Fog.Color, shadeColor, fogFactor);

    FragColor = vec4(Colour, 1.0);
}
