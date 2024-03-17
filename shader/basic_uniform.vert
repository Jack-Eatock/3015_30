#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;
out vec3 PosRelativeToPerspective;

// Matrices
uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewPerspective;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

uniform mat4 model;

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

    gl_Position =  ModelViewPerspective * vec4(VertexPosition,1.0);
}
