#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=4) out;

in VS_OUT {
	vec2 texCoord;
    vec3 normal;
} gs_in[];

out vec2 texCoordGS;

const float FUR_LENGTH = 0.025;

void main()
{
	// Original triangle
    for (int i = 0; i < gl_in.length(); ++i)
	{
		texCoordGS = gs_in[i].texCoord;

        gl_Position = gl_in[i].gl_Position;
	    EmitVertex();
	}
	
	// Additional point
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 N = normalize(cross(a, b));
	vec4 center = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0;
	
	gl_Position = center + vec4(N*FUR_LENGTH, 0.0);
	EmitVertex();
		
	EndPrimitive();
}