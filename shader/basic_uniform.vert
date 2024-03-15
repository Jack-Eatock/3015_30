#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 Position;
out vec3 Normal;

// Matrices
uniform mat4 ModelViewMatrix;
uniform mat4 ModelViewPerspective;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

void GetCamSpaceValues(out vec3 normal, out vec3 position)
{
    position = (ModelViewMatrix * vec4(VertexPosition, 1.0f)).xyz;
    normal = normalize(NormalMatrix*VertexNormal);
}

void main()
{
    GetCamSpaceValues(Normal,Position);

    // Setting the vertex pos
    gl_Position = ModelViewPerspective * vec4(VertexPosition,1.0);
}
