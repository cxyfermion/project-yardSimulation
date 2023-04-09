#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
	vec3 gAttr;
} gs_in[];

out vec3 fColor;
out float fFocus;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 colors;

void main()
{
    fFocus = gs_in[0].gAttr.y;
    //◊Ûœ¬Ω«
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(-0.12, -0.12, 0.0, 0.0));
    fColor = vec3(0.7) * step(gs_in[0].gAttr.x, -0.9);
    fColor += vec3(1.0, 0.0, 0.0) * step(-0.1, gs_in[0].gAttr.x) * step(gs_in[0].gAttr.x, 0.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(0.0, 1.0, 0.0) * step(1.9, gs_in[0].gAttr.z) * step(gs_in[0].gAttr.z, 2.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(1.0, 0.0, 0.0) * step(2.9, gs_in[0].gAttr.z) * step(gs_in[0].gAttr.z, 3.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(1.0, 1.0, 0.0) * colors * step(0.9, gs_in[0].gAttr.z) * step(gs_in[0].gAttr.z, 1.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(1.0, 1.0, 0.0) * colors * step(gs_in[0].gAttr.z, 0.1);
    EmitVertex();
    //”“œ¬Ω«
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(0.12, -0.12, 0.0, 0.0));
    fColor = vec3(0.7) * step(gs_in[0].gAttr.x, -0.9);
    fColor += vec3(1.0, 0.0, 0.0) * step(-0.1, gs_in[0].gAttr.x) * step(gs_in[0].gAttr.x, 0.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(1.0, 0.0, 0.0) * step(1.9, gs_in[0].gAttr.z) * step(gs_in[0].gAttr.z, 2.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(0.0, 1.0, 0.0) * step(2.9, gs_in[0].gAttr.z) * step(gs_in[0].gAttr.z, 3.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(0.5, 1.0, 0.0) * colors * step(0.9, gs_in[0].gAttr.z) * step(gs_in[0].gAttr.z, 1.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(0.5, 1.0, 0.0) * colors * step(gs_in[0].gAttr.z, 0.1);
    EmitVertex();
    //◊Û…œΩ«
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(-0.12, 0.12, 0.0, 0.0));
    fColor = vec3(0.7) * step(gs_in[0].gAttr.x, -0.9);
    fColor += vec3(1.0, 0.0, 0.0) * step(-0.1, gs_in[0].gAttr.x) * step(gs_in[0].gAttr.x, 0.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(0.0, 1.0, 0.0) * step(1.9, gs_in[0].gAttr.z) * step(gs_in[0].gAttr.z, 2.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(1.0, 0.0, 0.0) * step(2.9, gs_in[0].gAttr.z) * step(gs_in[0].gAttr.z, 3.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(1.0, 0.5, 1.0) * colors * step(0.9, gs_in[0].gAttr.z) * step(gs_in[0].gAttr.z, 1.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(1.0, 0.5, 1.0) * colors * step(gs_in[0].gAttr.z, 0.1);
    EmitVertex();
    //”“…œΩ«
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(0.12, 0.12, 0.0, 0.0));
    fColor = vec3(0.7) * step(gs_in[0].gAttr.x, -0.9);
    fColor += vec3(1.0, 0.0, 0.0) * step(-0.1, gs_in[0].gAttr.x) * step(gs_in[0].gAttr.x, 0.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(1.0, 0.0, 0.0) * step(1.9, gs_in[0].gAttr.z) * step(gs_in[0].gAttr.z, 2.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(0.0, 1.0, 0.0) * step(2.9, gs_in[0].gAttr.z) * step(gs_in[0].gAttr.z, 3.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(0.5, 0.5, 1.0) * colors * step(0.9, gs_in[0].gAttr.z) * step(gs_in[0].gAttr.z, 1.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(0.5, 0.5, 1.0) * colors * step(gs_in[0].gAttr.z, 0.1);
    EmitVertex();
    //Œ›∂•
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(0.0, 0.22, 0.0, 0.0));
    fColor = vec3(0.7) * step(gs_in[0].gAttr.x, -0.9);
    fColor += vec3(1.0, 0.0, 0.0) * step(-0.1, gs_in[0].gAttr.x) * step(gs_in[0].gAttr.x, 0.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(0.8, 0.8, 0.0) * step(1.9, gs_in[0].gAttr.z) * step(gs_in[0].gAttr.z, 2.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(0.8, 0.8, 0.0) * step(2.9, gs_in[0].gAttr.z) * step(gs_in[0].gAttr.z, 3.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(1.0, 0.8, 0.0) * colors * step(0.9, gs_in[0].gAttr.z) * step(gs_in[0].gAttr.z, 1.1);
    fColor += step(0.9, gs_in[0].gAttr.x) * vec3(0.8, 1.0, 0.0) * colors * step(gs_in[0].gAttr.z, 0.1);
    EmitVertex();
    EndPrimitive();
}
