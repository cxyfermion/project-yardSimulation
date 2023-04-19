#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 bPos;
layout (location = 2) in vec2 aAttr;

out VS_OUT {
    vec3 bvert;
	vec2 gAttr;
} vs_out;

void main()
{
    vs_out.bvert = bPos;
    vs_out.gAttr = aAttr;
    gl_Position = vec4(aPos.x, aPos.y, bPos.z, 1.0f); 
}
