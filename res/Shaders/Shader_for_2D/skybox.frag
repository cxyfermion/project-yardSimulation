#version 330 core
out vec4 FragColor;
in vec3 TexCoords;
uniform samplerCube skybox;
uniform float gray_ratio;

void main()
{    
    FragColor = gray_ratio * texture(skybox, TexCoords);
}