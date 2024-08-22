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
	// Calculate L, N, Viewer
	vec3 L = normalize(LightPos-FragPos);
	vec3 N = normalize(Normal);
	vec3 Viewer = normalize(C-FragPos);
	// Calculate R based on L and N
	vec3 R = normalize(2*dot(L, N)*N-L);
	
	// Calculate two versions of Phong Model: original and strengthened
	vec3 light = La*Ka + Ld*Kd*dot(L, N) + Ls*Ks*pow(dot(Viewer, R), Alpha);
	vec3 stronglight = La*Ka + Ld*Kd + Ls*Ks;

	// Calculate two versions of color: original texture and white
	vec4 color = texture(Texture, texCoord);
	vec4 white = vec4(1.0);

	// Calculate new light with original light and strengthened light
	vec3 newlight = mix(light, stronglight, 1-dot(N, Viewer));
	// Calculate new color with original color and white
	vec4 newcolor = mix(color, white, (1-dot(N, Viewer))*0.7);
	
	FragColor = vec4(newlight, 1.0)*newcolor;
}