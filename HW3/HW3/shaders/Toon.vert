#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

vec4 worldPos;

out vec3 FragPos;
out vec3 Normal;

void main()
{
	worldPos = M * vec4(aPos, 1.0);
	gl_Position = P * V * worldPos;
	
	FragPos = vec3(worldPos);
	Normal = normalize(mat3(transpose(inverse(M))) * aNormal);
}