#version 330 core
out vec4 FragColor;
in vec4 fAttr;

void main()
{
    if (fAttr.w > 0.9)
        discard;
    FragColor = vec4(fAttr.x, fAttr.y, fAttr.z, 1.0f);
}
