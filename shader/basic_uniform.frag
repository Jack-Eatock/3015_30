#version 460

in vec3 ViewDir;
in vec3 PosRelativeToPerspective;
in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

// Edge
uniform float EdgeThreshold;
uniform int Pass;
const vec3 lum = vec3(.3, 0.3, .0722);

layout (location = 0) out vec4 FragColor;
layout (binding = 1 ) uniform sampler2D Tex1; // Diffuse
layout (binding = 2 ) uniform sampler2D Tex2; // Moss
layout (binding = 3 ) uniform sampler2D TexNormal; // Normal Map
 
layout (binding = 0) uniform sampler2D RenderTex;

uniform bool displayRender; 

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
    vec3 Position;
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
    //vec3 diffuse = Material.Diffuse * lightDirDotNormal;// (No toon shading)
    if (texColor.r > 0 || texColor.g > 0 || texColor.b > 0 )
        diffuse = texColor * floor(lightDirDotNormal * levels) * scaleFactor;

    // Ambient
    vec3 ambient = Light.AmbientColour * Material.Ambient;
    if (texColor.r > 0 || texColor.g > 0 || texColor.b > 0)
         ambient = Light.AmbientColour * texColor;
        
    // Specular
    vec3 specular = vec3(0.0);
    if (lightDirDotNormal > 0.0 && Material.Shininess != 0 )
    {
        vec3 dirBackFromCam = normalize(ViewDir);
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

float luminance(vec3 color)
{
    return dot(lum, color);
}

vec4 pass1()
{
    vec3 normTexture = texture(TexNormal, TexCoord).xyz;
    vec3 norm = normalize(normTexture * 2.0 - 1.0);   

    // Depth of the scene
    float dist = abs(PosRelativeToPerspective.z);
    float fogFactor = (Fog.MaxDist - dist)/(Fog.MaxDist - Fog.MinDist);
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    vec3 shadeColor = vec3(0);

    // Texture
    vec3 texColor = texture(Tex1, TexCoord).rgb;
    vec4 texColor2 = texture(Tex2, TexCoord);
    if (texColor2.r > 0 || texColor2.g > 0 || texColor2.b > 0)
        texColor = mix(texColor, texColor2.rgb, texColor2.a);

    // Do we want to display a render instead?
    if (displayRender)
    {
         vec3 texRender = texture(RenderTex, TexCoord).rgb;
         texColor = texRender;
    }
   

    // Does this object have a normal texture? 
    if (normTexture.r > 0 || normTexture.g > 0 || normTexture.b > 0)   
    {
        // Calculate each light.
        for (int i = 0; i < Lights.length; i++)
            shadeColor += BlinnPhong(Lights[i], Position, norm, texColor);

        // Calculate each Spot light
        //shadeColor += BlinnPhongSpot(SpotLight, Position, norm, texColor);

    }
    else
    {
        // Calculate each light.
        for (int i = 0; i < Lights.length; i++)
            shadeColor += BlinnPhong(Lights[i], Position, Normal, texColor);

        // Calculate each Spot light
        //shadeColor += BlinnPhongSpot(SpotLight, Position, Normal, texColor);
    }

    // Mix light and fog
    vec3 Colour = mix(Fog.Color, shadeColor, fogFactor);

    return vec4(Colour, 1.0);
}



vec4 pass2()
{
    ivec2 pix = ivec2(gl_FragCoord.xy);
    float s00 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(-1,1)).rgb);
    float s10 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(-1,0)).rgb);
    float s20 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(-1,-1)).rgb);
    float s01 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(0,1)).rgb);
    float s21 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(0,-1)).rgb);
    float s02 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(1,1)).rgb);
    float s12 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(1,0)).rgb);
    float s22 = luminance(texelFetchOffset(RenderTex, pix, 0, ivec2(1,-1)).rgb);

    float sx = s00+2*s10+s20-(s02+2*s12+s22);
    float sy = s00+2*s01+s02-(s20+2*s21+s22);
    float g  = sx*sx+sy*sy;
    if (g >EdgeThreshold)
        return vec4(0.1);
    else
        return texelFetch(RenderTex,pix, 0); // vec(0,0,0,1);
}

void main() 
{

   if (Pass == 1) FragColor = pass1();
   else if (Pass == 2) FragColor += pass2();
}
