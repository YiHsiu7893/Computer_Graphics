#version 330 core

in vec3 FragPos;
in vec3 Normal;

uniform vec3 C;
uniform vec3 LightPos;

out vec4 FragColor;

void main()
{
	// Calculate L, N, V
	vec3 L = normalize(LightPos-FragPos);
	vec3 N = normalize(Normal);
	vec3 Viewer = normalize(C-FragPos);
	// Calculate R based on L and N
	vec3 R = normalize(2*dot(L, N)*N-L);
	
	// Calculate the angle between L and N, and the specular intensity
	float angle = dot(L, N);
	float specI = dot(Viewer, R);
	
	// Decide three levels of color
	if(angle<0) FragColor = vec4(0.1, 0.1, 0.1, 1.0);
	else if(specI>0.8) FragColor = vec4(0.9, 0.8, 0.8, 1.0);  // +-37
	else FragColor = vec4(0.5, 0.3, 0.2, 1.0);
}