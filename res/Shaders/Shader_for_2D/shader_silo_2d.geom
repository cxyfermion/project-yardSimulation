#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
	vec3 gAttr;         //state_up当前状态,state_down当前状态,percent存储量百分比
    vec2 gState;        //type货物类型,focus获得焦点
} gs_in[];

out vec4 fAttr;         //r,g,b,focus

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 contain_color = vec3(0.3) * step(gs_in[0].gState.x, 0.1);
    contain_color += vec3(0.1) * step(0.9, gs_in[0].gState.x) * step(gs_in[0].gState.x, 1.1);
    float ratio_silo = 0.8;
    //底边
    vec3 gColor = (vec3(1.0) - gs_in[0].gAttr.z * (vec3(1.0) - contain_color)) * step(gs_in[0].gAttr.y, 0.1);
    gColor += vec3(0.0f, 1.0f, 0.0f) * step(0.9, gs_in[0].gAttr.y) * step(gs_in[0].gAttr.y, 1.1);
    fAttr = vec4(gColor, gs_in[0].gState.y);
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(-0.16 * ratio_silo, -0.08 * ratio_silo, 0.0, 0.0));   
    EmitVertex();   
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4( 0.16 * ratio_silo, -0.08 * ratio_silo, 0.0, 0.0));
    EmitVertex();
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4( 0.0,  0.08 * ratio_silo, 0.0, 0.0));
    //顶点
    gColor = (vec3(1.0) - gs_in[0].gAttr.z * (vec3(1.0) - contain_color)) * step(gs_in[0].gAttr.x, 0.1);
    gColor += vec3(0.0f, 1.0f, 0.0f) * step(0.9, gs_in[0].gAttr.x) * step(gs_in[0].gAttr.x, 1.1);
    fAttr = vec4(gColor, gs_in[0].gState.y);
    EmitVertex();
    EndPrimitive();
}
