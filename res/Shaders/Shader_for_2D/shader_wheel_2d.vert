#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float aFocus;

out VS_OUT {
	vec3 gColor;
    float gFocus;
} vs_out;

void main()
{
    vs_out.gColor = aColor;
    vs_out.gFocus = aFocus;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f); 
}