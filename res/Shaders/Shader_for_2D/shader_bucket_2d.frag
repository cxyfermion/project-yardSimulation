#version 330 core
out vec4 FragColor;
in vec2 fAttr;

void main()
{
    if (fAttr.y > 0.9)
        discard;
    //0��ɫ��ú��1��ɫ����
    FragColor = vec4(fAttr.x, fAttr.x, fAttr.x, 1.0f);
}
