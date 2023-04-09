#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 Position;

uniform float texture_state;
uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{    
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - cameraPos);
    //���Ӳ���
    //vec3 R = reflect(I, normalize(Normal));
    //��������
    //vec3 R = refract(I, normalize(Normal), ratio);
    vec3 R = reflect(I, normalize(Normal)) * step(texture_state, 0.5) + refract(I, normalize(Normal), ratio) * (1 - step(texture_state, 0.5));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}