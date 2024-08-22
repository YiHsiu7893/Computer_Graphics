#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in Tex_OUT{
	vec2 tCoord;
}tex_in[];

in POS_OUT{
	vec3 Pos;
}pos_in[];

out vec2 texCoord;
out vec3 FragPos;
out vec3 Normal;

void main(void)
{
	// Get the position of the three points of a polygon
	vec3 p0 = pos_in[0].Pos;
	vec3 p1 = pos_in[1].Pos;
	vec3 p2 = pos_in[2].Pos;
	// Define the normal of this polygon
	vec3 polyNormal = normalize(cross(p1-p0, p2-p0));

	for(int i=0; i<3; i++)
	{
		gl_Position = gl_in[i].gl_Position;

		texCoord = tex_in[i].tCoord;
		FragPos = pos_in[i].Pos;
		Normal = polyNormal;

		EmitVertex();
	}
	EndPrimitive();
}