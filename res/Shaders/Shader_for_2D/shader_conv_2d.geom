#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

in VS_OUT {
    vec4 bvert;
	vec3 gColor;
	float focus;
} gs_in[];

out float fValue;
out float fFocus;
out vec3 fColor;

void build_belt(vec4 position)
{    
    gl_Position = position;
    EmitVertex();
    gl_Position = gs_in[0].bvert;
    EmitVertex();
    EndPrimitive();
}

void main() 
{    
    fFocus = gs_in[0].focus;
	fColor = gs_in[0].gColor;
    build_belt(gl_in[0].gl_Position);
}