#version 330 core
out vec4 FragColor;
in vec3 fColor;
in float fFocus;
uniform float value;

void main()
{
	if (fFocus > 0.9f)
		discard;
	else
	{
		if (fColor.z > 0.98f)		//ю╤и╚в╢л╛иак╦
			FragColor = vec4(fColor.x, fColor.y, value, 1.0f);
		else
			FragColor = vec4(fColor, 1.0f);
	}
}
