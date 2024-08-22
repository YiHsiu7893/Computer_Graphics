#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform float squeezeFactor;
//uniform float offest;

out vec2 texCoord;
out vec3 normal;

vec4 worldPos;

void main()
{
	// TODO: Implement squeeze effect and update normal transformation
	
	vec3 squeezedPos = aPos;
	// Adjust the vertex position to create a squeeze effect
	squeezedPos.y += aPos.z * sin(squeezeFactor) / 2.0;
	squeezedPos.z += aPos.y * sin(squeezeFactor) / 2.0;

	// Update worldPos
	worldPos = M * vec4(squeezedPos, 1.0);

	// Calculate the final gl_Position
	gl_Position = P * V * worldPos;

	// Update the normal transformation
	normal = mat3(transpose(inverse(M))) * aNormal;
	
	texCoord = aTexCoord;
}