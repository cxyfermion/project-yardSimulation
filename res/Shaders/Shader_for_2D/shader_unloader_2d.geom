#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

in VS_OUT {
	vec2 gAttr;
} gs_in[];

out vec2 fAttr;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    float ratio = 0.5;
    fAttr = gs_in[0].gAttr;
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(0, 0.189282 * ratio, 0.0, 0.0));
    EmitVertex();
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(0.12, 0.12 * ratio, 0.0, 0.0));
    EmitVertex();
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(-0.12, 0.12 * ratio, 0.0, 0.0));
    EmitVertex();
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(0.12, -0.12 * ratio, 0.0, 0.0));
    EmitVertex();
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(-0.12, -0.12 * ratio, 0.0, 0.0));
    EmitVertex();
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(0, -0.189282 * ratio, 0.0, 0.0));
    EmitVertex();
    EndPrimitive();
}
