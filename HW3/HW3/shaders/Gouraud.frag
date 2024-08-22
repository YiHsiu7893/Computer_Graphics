#version 330 core

in vec3 light;
in vec2 texCoord; 

uniform sampler2D Texture;

out vec4 FragColor;

void main()
{
	vec4 color = texture(Texture, texCoord);
	
	FragColor = vec4(light, 1.0)*color;
}