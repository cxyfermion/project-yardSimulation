#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

in VS_OUT {
    vec3 bvert;         //x2,y2,z
	vec2 gAttr;         //type,focus
} gs_in[];

out vec4 fAttr;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 contain_color = vec3(1.0) * step(gs_in[0].gAttr.x, 0.1);
    contain_color += vec3(0.3) * step(0.9, gs_in[0].gAttr.x) * step(gs_in[0].gAttr.x, 1.1);
    contain_color += vec3(0.4) * step(1.9, gs_in[0].gAttr.x) * step(gs_in[0].gAttr.x, 2.1);
    contain_color += vec3(0.85) * step(2.9, gs_in[0].gAttr.x) * step(gs_in[0].gAttr.x, 3.1);
    contain_color += vec3(0.6) * step(3.9, gs_in[0].gAttr.x) * step(gs_in[0].gAttr.x, 4.1);
    contain_color += vec3(0.7, 0.7, 0.0) * step(4.9, gs_in[0].gAttr.x) * step(gs_in[0].gAttr.x, 5.1);
    contain_color += vec3(0.9, 0.9, 0.4) * step(5.9, gs_in[0].gAttr.x) * step(gs_in[0].gAttr.x, 6.1);
    contain_color += vec3(0.0) * step(6.9, gs_in[0].gAttr.x) * step(gs_in[0].gAttr.x, 7.1);
    fAttr = vec4(contain_color, gs_in[0].gAttr.y);
    gl_Position = projection * view * model * gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = projection * view * model * vec4(gs_in[0].bvert, 1.0);
    EmitVertex();
    EndPrimitive();
}
