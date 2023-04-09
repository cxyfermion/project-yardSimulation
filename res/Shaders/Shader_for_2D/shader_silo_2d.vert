#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aAttr;
layout (location = 2) in vec2 aState;

out VS_OUT {
	vec3 gAttr;
    vec2 gState;
} vs_out;

void main()
{
    vs_out.gAttr = aAttr;
    vs_out.gState = aState;
    gl_Position = vec4(aPos, 1.0f); 
}
