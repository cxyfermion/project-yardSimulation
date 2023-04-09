#version 330 core
out vec4 FragColor;
in vec3 fColor;
in float fFocus;

void main()
{
    if (fFocus > 0.9)
        discard;
    FragColor = vec4(fColor, 1.0f);   
}