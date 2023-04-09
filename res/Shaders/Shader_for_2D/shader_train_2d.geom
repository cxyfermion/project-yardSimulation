#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
	vec4 gAttr;     //state当前状态,percent存储量百分比，focus获得焦点（未到港时x,y都为0），type货物种类
} gs_in[];

out vec4 fAttr;     //r,g,b,focus

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    float ratio = 0.7f;
    vec3 contain_color = vec3(1.0) * step(gs_in[0].gAttr.w, 0.1);
    contain_color += vec3(0.3) * step(0.9, gs_in[0].gAttr.w) * step(gs_in[0].gAttr.w, 1.1);
    contain_color += vec3(0.4) * step(1.9, gs_in[0].gAttr.w) * step(gs_in[0].gAttr.w, 2.1);
    contain_color += vec3(0.85) * step(2.9, gs_in[0].gAttr.w) * step(gs_in[0].gAttr.w, 3.1);
    contain_color += vec3(0.6) * step(3.9, gs_in[0].gAttr.w) * step(gs_in[0].gAttr.w, 4.1);
    contain_color += vec3(0.7, 0.7, 0.0) * step(4.9, gs_in[0].gAttr.w) * step(gs_in[0].gAttr.w, 5.1);
    contain_color += vec3(0.9, 0.9, 0.4) * step(5.9, gs_in[0].gAttr.w) * step(gs_in[0].gAttr.w, 6.1);
    contain_color += vec3(0.0) * step(6.9, gs_in[0].gAttr.w) * step(gs_in[0].gAttr.w, 7.1);
    vec3 gColor = vec3(1.0f, 0.0f, 0.0f) * step(gs_in[0].gAttr.x, 0.1);
    gColor += (contain_color + (vec3(1.0) - contain_color) * gs_in[0].gAttr.y) * step(0.9, gs_in[0].gAttr.x) * step(gs_in[0].gAttr.x, 1.1);
    gColor += vec3(0.0f, 0.0f, 1.0f) * step(1.9, gs_in[0].gAttr.x) * step(gs_in[0].gAttr.x, 2.1);
    fAttr = vec4(gColor, gs_in[0].gAttr.z);
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(-0.05 * ratio, -0.08, 0.0, 0.0));   
    EmitVertex();   
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(-0.05 * ratio,  0.08, 0.0, 0.0));
    EmitVertex();
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4( 0.05 * ratio, -0.08, 0.0, 0.0));
    EmitVertex();
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4( 0.05 * ratio,  0.08, 0.0, 0.0));
    EmitVertex();
    EndPrimitive();
}
