#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 bPos;
layout(location = 2) in vec3 aColor;
layout(location = 3) in float aFocus;

out VS_OUT {
    vec4 bvert;
	vec3 gColor;
	float focus;
} vs_out;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vs_out.focus = aFocus;
	vs_out.bvert = projection * view * model * vec4(bPos.x, bPos.y, bPos.z, 1.0f);
	vs_out.gColor = aColor;
	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, bPos.z, 1.0f);
}