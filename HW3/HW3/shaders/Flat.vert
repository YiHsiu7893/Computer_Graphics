#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

vec4 worldPos;

out Tex_OUT{
	vec2 tCoord;
}tex_out;

out POS_OUT{
	vec3 Pos;
}pos_out;

void main()
{
	worldPos = M * vec4(aPos, 1.0);
	gl_Position = P * V * worldPos;
	
	tex_out.tCoord = aTexCoord;
	pos_out.Pos = vec3(worldPos);
}