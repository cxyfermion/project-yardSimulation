#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aAttri;
layout (location = 2) in float aFocus;

out VS_OUT {
	vec3 gAttri;
    float gFocus;
} vs_out;

void main()
{
    vs_out.gAttri = aAttri;
    vs_out.gFocus = aFocus;
    gl_Position = vec4(aPos, 1.0f); 
}