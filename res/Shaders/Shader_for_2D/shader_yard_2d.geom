#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
	vec3 gAttri;
    float gFocus;
} gs_in[];

out vec3 fColor;
out float fFocus;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float yard_warn;

void main()
{
    float ratio = 0.3f;
    fFocus = gs_in[0].gFocus;
    //堆料颜色
    vec3 contain_color = vec3(1.0) * yard_warn * step(gs_in[0].gAttri.z, 0.1);
    contain_color += vec3(0.3) * yard_warn * step(0.9, gs_in[0].gAttri.z) * step(gs_in[0].gAttri.z, 1.1);
    contain_color += vec3(0.4) * yard_warn * step(1.9, gs_in[0].gAttri.z) * step(gs_in[0].gAttri.z, 2.1);
    contain_color += vec3(0.85) * yard_warn * step(2.9, gs_in[0].gAttri.z) * step(gs_in[0].gAttri.z, 3.1);
    contain_color += vec3(0.6) * yard_warn * step(3.9, gs_in[0].gAttri.z) * step(gs_in[0].gAttri.z, 4.1);
    contain_color += vec3(0.7, 0.7, 0.0) * yard_warn * step(4.9, gs_in[0].gAttri.z) * step(gs_in[0].gAttri.z, 5.1);
    contain_color += vec3(0.9, 0.9, 0.4) * yard_warn * step(5.9, gs_in[0].gAttri.z) * step(gs_in[0].gAttri.z, 6.1);
    contain_color += vec3(0.0) * yard_warn * step(6.9, gs_in[0].gAttri.z) * step(gs_in[0].gAttri.z, 7.1);
    //左下角
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(-0.20 * ratio, -0.10 * ratio, 0.0, 0.0));    
    fColor = contain_color;
    EmitVertex();   
    //右下角
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4( 0.20 * ratio, -0.10 * ratio, 0.0, 0.0));
    EmitVertex();
    //左上角
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(-0.20 * ratio,  0.10 * ratio, 0.0, 0.0));
    fColor = contain_color * step(gs_in[0].gAttri.x, 0.1);
    fColor += vec3(0.0, 1.0, 0.0) * step(0.9, gs_in[0].gAttri.x) * step(gs_in[0].gAttri.x, 1.1);
    fColor += vec3(0.0, 0.0, 1.0) * step(1.9, gs_in[0].gAttri.x) * step(gs_in[0].gAttri.x, 2.1);
    EmitVertex();
    //右上角
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4( 0.20 * ratio,  0.10 * ratio, 0.0, 0.0));
    fColor = contain_color * step(gs_in[0].gAttri.y, 0.1);
    fColor += vec3(0.0, 1.0, 0.0) * step(0.9, gs_in[0].gAttri.y) * step(gs_in[0].gAttri.y, 1.1);
    fColor += vec3(0.0, 0.0, 1.0) * step(1.9, gs_in[0].gAttri.y) * step(gs_in[0].gAttri.y, 2.1);
    EmitVertex();
    EndPrimitive();
}