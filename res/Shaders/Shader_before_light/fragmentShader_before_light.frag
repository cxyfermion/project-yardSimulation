#version 330 core

out vec4 FragColor;
in vec3 ourColor;
//uniform float xColor;
uniform vec3 colors;
uniform float alpha;
uniform bool isTexture;
uniform bool isColor;
in vec2 TexCoords;
uniform sampler2D texture_1;
uniform sampler2D texture_2;

void main()
{
	//FragColor = texture(texture_1, TexCoords);
	//FragColor = texture(texture_1, TexCoords) * vec4(xColor, ourColor.y, ourColor.z, 1.0f);
	//FragColor = texture(texture_1, 1 * TexCoords) * vec4(ourColor * colors, 1.0f);
	//FragColor = vec4(xColor, ourColor.y, ourColor.z, 1.0f);
	//FragColor = mix(texture(texture_1, 1 * TexCoords), texture(texture_2, 3 * TexCoords) * vec4(ourColor * colors, 1.0f), alpha);
	if (isTexture)
	{
		if (isColor)
		{
			//FragColor = mix(texture(texture_1, 1 * TexCoords), texture(texture_2, 3 * TexCoords) * vec4(ourColor * colors, 1.0f), alpha);
			//处理透明通道
			vec4 texColor = texture(texture_2, 3 * TexCoords);
			if(texColor.a < 0.1)
				//discard;
				FragColor = texture(texture_1, 1 * TexCoords);
			else
				FragColor = mix(texture(texture_1, 1 * TexCoords), texColor * vec4(ourColor * colors, 1.0f), alpha);
		}
		else
		{
			//FragColor = mix(texture(texture_1, 1 * TexCoords), texture(texture_2, 3 * TexCoords), alpha);
			vec4 texColor = texture(texture_2, 3 * TexCoords);
			if(texColor.a < 0.1)
				//discard;
				FragColor = texture(texture_1, 1 * TexCoords);
			else
				FragColor = mix(texture(texture_1, 1 * TexCoords), texColor, alpha);
		}
	}
	else
	{
		if (isColor)
			FragColor = vec4(colors, 1.0f);
		else
			FragColor = vec4(0.0f);

	}
};