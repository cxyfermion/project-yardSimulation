#version 330 core
out vec4 FragColor;
in vec2 fAttr;

void main()
{
    if (fAttr.y > 0.9)
        discard;
    //0��ɫδ���У�1��ɫ�豸������2��ɫ��������
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f) * step(fAttr.x, 0.1);
    FragColor += vec4(0.0f, 0.0f, 1.0f, 1.0f) * step(0.9, fAttr.x) * step(fAttr.x, 1.1);
    FragColor += vec4(0.0f, 1.0f, 0.0f, 1.0f) * step(1.9, fAttr.x);
}
