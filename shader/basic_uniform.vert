#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;
layout (location = 3) in vec4 VertexTangent;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;
out vec3 PosRelativeToPerspective;
out vec3 ViewDir;

// Matrices
uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewPerspective;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

uniform mat4 model;

uniform float Time;
uniform float Freq =5;
uniform float Velocity = 1.5;
uniform float Amp = 2;


void GetCamSpaceValues(out vec3 normal, out vec3 position)
{
    position = (model * vec4(VertexPosition, 1.0f)).xyz;
    PosRelativeToPerspective =  (ModelViewMatrix * vec4(VertexPosition, 1.0f)).xyz;
    normal = normalize(VertexNormal);
}

void main()
{
    TexCoord = VertexTexCoord;
    GetCamSpaceValues(Normal,Position);

    // Water Surface animation
    if (Time != 0)
    {
        float u = Freq*PosRelativeToPerspective.x-Velocity*Time;
        Position.y = Amp * sin(u);
        Position.x = Amp * sin(u);
        vec3 n = vec3(.0);
        n.xy = normalize(vec2(cos(u), (1.0)));
        Normal = normalize(VertexNormal);
    }
  

    // Normal
    vec3 tangent = normalize(vec3(VertexTangent));
    vec3 binormal = normalize(cross(Normal, tangent)) * VertexTangent.w;

    mat3 toObjectLocal = mat3(
        tangent.x, binormal.x, Normal.x,
        tangent.y,  binormal.y, Normal.y,
        tangent.z,  binormal.z, Normal.z
    );
    ViewDir = toObjectLocal*normalize(-Position);
    gl_Position =  ModelViewPerspective * vec4(VertexPosition,1.0);
}
