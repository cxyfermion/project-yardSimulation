#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
	vec3 gAttr;         //state当前状态,percent存储量百分比，focus获得焦点
    vec2 gLevelType;    //level吨位级别，type货物种类
} gs_in[];

out vec4 fAttr;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 contain_color = vec3(1.0) * step(gs_in[0].gLevelType.y, 0.1);
    contain_color += vec3(0.3) * step(0.9, gs_in[0].gLevelType.y) * step(gs_in[0].gLevelType.y, 1.1);
    contain_color += vec3(0.4) * step(1.9, gs_in[0].gLevelType.y) * step(gs_in[0].gLevelType.y, 2.1);
    contain_color += vec3(0.85) * step(2.9, gs_in[0].gLevelType.y) * step(gs_in[0].gLevelType.y, 3.1);
    contain_color += vec3(0.6) * step(3.9, gs_in[0].gLevelType.y) * step(gs_in[0].gLevelType.y, 4.1);
    contain_color += vec3(0.7, 0.7, 0.0) * step(4.9, gs_in[0].gLevelType.y) * step(gs_in[0].gLevelType.y, 5.1);
    contain_color += vec3(0.9, 0.9, 0.4) * step(5.9, gs_in[0].gLevelType.y) * step(gs_in[0].gLevelType.y, 6.1);
    contain_color += vec3(0.0) * step(6.9, gs_in[0].gLevelType.y) * step(gs_in[0].gLevelType.y, 7.1);
    vec3 gColor = vec3(1.0f, 0.0f, 0.0f) * step(0.9, gs_in[0].gAttr.x) * step(gs_in[0].gAttr.x, 1.1);
    gColor += vec3(0.0f, 1.0f, 0.0f) * step(1.9, gs_in[0].gAttr.x) * step(gs_in[0].gAttr.x, 2.1);
    gColor += (contain_color + (vec3(1.0) - contain_color) * gs_in[0].gAttr.y) * step(2.9, gs_in[0].gAttr.x) * step(gs_in[0].gAttr.x, 3.1);
    gColor += vec3(0.0f, 0.0f, 1.0f) * step(3.9, gs_in[0].gAttr.x) * step(gs_in[0].gAttr.x, 4.1);
    fAttr = vec4(gColor, gs_in[0].gAttr.z);
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(-0.16 * gs_in[0].gLevelType.x,  0.08 * gs_in[0].gLevelType.x, 0.0, 0.0));   
    EmitVertex();   
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(-0.16 * gs_in[0].gLevelType.x, -0.08 * gs_in[0].gLevelType.x, 0.0, 0.0));
    EmitVertex();
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4( 0.26 * gs_in[0].gLevelType.x,  0.08 * gs_in[0].gLevelType.x, 0.0, 0.0));
    EmitVertex();
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4( 0.26 * gs_in[0].gLevelType.x, -0.08 * gs_in[0].gLevelType.x, 0.0, 0.0));
    EmitVertex();
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4( 0.38 * gs_in[0].gLevelType.x,  0.00, 0.0, 0.0));
    EmitVertex();
    EndPrimitive();
}
