#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aAttr;

out VS_OUT {
	vec3 gAttr;
} vs_out;

void main()
{
    vs_out.gAttr = aAttr;
    gl_Position = vec4(aPos, 1.0f); 
}
