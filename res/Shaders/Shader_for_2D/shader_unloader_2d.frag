#version 330 core
out vec4 FragColor;
in vec2 fAttr;

void main()
{
    if (fAttr.y > 0.9)
        discard;
    //0红色未运行，1蓝色设备就绪，2绿色负载运行
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f) * step(fAttr.x, 0.1);
    FragColor += vec4(0.0f, 0.0f, 1.0f, 1.0f) * step(0.9, fAttr.x) * step(fAttr.x, 1.1);
    FragColor += vec4(0.0f, 1.0f, 0.0f, 1.0f) * step(1.9, fAttr.x);
}
