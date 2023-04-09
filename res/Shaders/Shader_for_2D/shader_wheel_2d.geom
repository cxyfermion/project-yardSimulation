#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
	vec3 gColor;
    float gFocus;
} gs_in[];

out vec3 fColor;
out float fFocus;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    fFocus = gs_in[0].gFocus;
	fColor = gs_in[0].gColor;
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(-0.16, -0.16, 0.0, 0.0));   
    EmitVertex();   
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4( 0.16, -0.16, 0.0, 0.0));
    EmitVertex();
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(-0.16,  0.16, 0.0, 0.0));
    EmitVertex();
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4( 0.16,  0.16, 0.0, 0.0));
    EmitVertex();
    EndPrimitive();
}