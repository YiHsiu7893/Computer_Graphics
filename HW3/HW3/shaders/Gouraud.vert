#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 C;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Alpha;
uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;
uniform vec3 LightPos;

vec4 worldPos;

out vec3 light;
out vec2 texCoord; 

void main()
{
	worldPos = M * vec4(aPos, 1.0);
	gl_Position = P * V * worldPos;

	// Calculate L, N, V
	vec3 L = normalize(LightPos-vec3(worldPos));
	vec3 N = normalize(mat3(transpose(inverse(M))) * aNormal);
	vec3 Viewer = normalize(C-vec3(worldPos));
	// Calculate R based on L and N
	vec3 R = normalize(2*dot(L, N)*N-L);
	
	// Calculate the three components of Phong Model
	vec3 ambient = La*Ka;
	vec3 diffuse = Ld*Kd*dot(L, N);
	vec3 specular = Ls*Ks*pow(dot(Viewer, R), Alpha);
	light = ambient + diffuse + specular;

	texCoord = aTexCoord;
}