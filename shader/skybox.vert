#version 460

layout (location = 0) in vec3 VertexPosition;

out vec3 Vec;

uniform mat4 ModelViewPerspective;


void main()
{
    Vec = VertexPosition;

    // Setting the vertex pos
    gl_Position = ModelViewPerspective * vec4(VertexPosition,1.0);
}
