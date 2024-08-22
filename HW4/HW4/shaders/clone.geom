#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 27) out;

in VS_OUT {
	vec2 texCoord;
    vec3 normal;
} gs_in[];

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform float time;

out vec2 texCoordGS;

void main()
{
     for (int i = 0; i < 3; ++i)
    {
        gl_Position = gl_in[i].gl_Position;
        gl_Position = P * V * M * gl_Position;
        texCoordGS = gs_in[i].texCoord;
        EmitVertex();
    }
    EndPrimitive();

    for (int i = 0; i < 3; ++i)
    {
        gl_Position = gl_in[i].gl_Position;
        gl_Position.x += time;
        gl_Position = P * V * M * gl_Position;
        texCoordGS = gs_in[i].texCoord;
        EmitVertex();
    }
    EndPrimitive();
    
    for (int i = 0; i < 3; ++i)
    {
        gl_Position = gl_in[i].gl_Position;
        gl_Position.x -= time;
        gl_Position = P * V * M * gl_Position;
        texCoordGS = gs_in[i].texCoord;
        EmitVertex();
    }
    EndPrimitive();

    for (int i = 0; i < 3; ++i)
    {
        gl_Position = gl_in[i].gl_Position;
        gl_Position.z += time;
        gl_Position = P * V * M * gl_Position;
        texCoordGS = gs_in[i].texCoord;
        EmitVertex();
    }
    EndPrimitive();

    for (int i = 0; i < 3; ++i)
    {
        gl_Position = gl_in[i].gl_Position;
        gl_Position.z -= time;
        gl_Position = P * V * M * gl_Position;
        texCoordGS = gs_in[i].texCoord;
        EmitVertex();
    }
    EndPrimitive();

    for (int i = 0; i < 3; ++i)
    {
        gl_Position = gl_in[i].gl_Position;
        gl_Position.x += time;
        gl_Position.z += time;
        gl_Position = P * V * M * gl_Position;
        texCoordGS = gs_in[i].texCoord;
        EmitVertex();
    }
    EndPrimitive();

    for (int i = 0; i < 3; ++i)
    {
        gl_Position = gl_in[i].gl_Position;
        gl_Position.x += time;
        gl_Position.z -= time;
        gl_Position = P * V * M * gl_Position;
        texCoordGS = gs_in[i].texCoord;
        EmitVertex();
    }
    EndPrimitive();

    for (int i = 0; i < 3; ++i)
    {
        gl_Position = gl_in[i].gl_Position;
        gl_Position.x -= time;
        gl_Position.z += time;
        gl_Position = P * V * M * gl_Position;
        texCoordGS = gs_in[i].texCoord;
        EmitVertex();
    }
    EndPrimitive();

    for (int i = 0; i < 3; ++i)
    {
        gl_Position = gl_in[i].gl_Position;
        gl_Position.x -= time;
        gl_Position.z -= time;
        gl_Position = P * V * M * gl_Position;
        texCoordGS = gs_in[i].texCoord;
        EmitVertex();
    }
    EndPrimitive();
}