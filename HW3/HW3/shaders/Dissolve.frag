#version 330 core

in vec2 texCoord;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 C;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Alpha;
uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;
uniform vec3 LightPos;
uniform sampler2D Texture;

out vec4 FragColor;

void main()
{
	// Calculate L, N, V
	vec3 L = normalize(LightPos-FragPos);
	vec3 N = normalize(Normal);
	vec3 Viewer = normalize(C-FragPos);
	// Calculate R based on L and N
	vec3 R = normalize(2*dot(L, N)*N-L);
	
	// Calculate the three components of Phong Model
	vec3 ambient = La*Ka;
	vec3 diffuse = Ld*Kd*dot(L, N);
	vec3 specular = Ls*Ks*pow(dot(Viewer, R), Alpha);
	vec3 light = ambient + diffuse + specular;

	vec4 color = texture(Texture, texCoord);

	FragColor = vec4(light, 1.0)*color;
}