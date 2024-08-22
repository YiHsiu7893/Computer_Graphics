#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
	vec2 texCoord;
    vec3 normal;
} gs_in[];

uniform float time;

out vec2 texCoordGS;

vec4 explode(vec4 position, vec3 normal)
{
    vec3 direction = normal * time * 2; 
    return position + vec4(direction, 0.0);
} 


void main()
{
	// Get Normal
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 normal = normalize(cross(a, b));
    
	for(int i=0; i<3; i++)
	{
		gl_Position = explode(gl_in[i].gl_Position, normal);
		texCoordGS = gs_in[i].texCoord;
		EmitVertex();
	}
	
    EndPrimitive();
}