#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<cmath>
#include"res/GL_heads/Shader.h"
#include"res/GL_heads/Camera.hpp"
//#include "stb_image.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>
#include <vector>
#include"res/GL_heads/Model.hpp"
#include <random>

/*
SSAO
�����Ѿ���ǰ��Ļ����̳��м򵥽��ܵ����ⲿ�����ݣ���������(Ambient Lighting)���������������Ǽ��볡����������е�һ���̶����ճ�������������ģ����ɢ��(Scattering)��
����ʵ�У����߻������ⷽ��ɢ�䣬����ǿ���ǻ�һֱ�ı�ģ����Լ�ӱ��յ����ǲ��ֳ���ҲӦ���б仯��ǿ�ȣ�������һ�ɲ���Ļ����⡣
����һ�ּ�ӹ��յ�ģ������������ڱ�(Ambient Occlusion)������ԭ����ͨ�������塢�׶��ͷǳ�������ǽ��䰵�ķ�������ģ�����ӹ��ա���Щ����ܴ�̶����Ǳ���Χ�ļ������ڱεģ����߻������ʧ��������Щ�ط������������һЩ��

�������ڱ���һ����������ܴ�����ܿ�������Ϊ������Ҫ������Χ�ļ����塣���ǿ��ԶԿռ���ÿһ�㷢�����������ȷ�����ڱ�������������ʵʱ�����л�ܿ��ɴ����⡣��2007�꣬Crytek��˾������һ�������Ļ�ռ价�����ڱ�(Screen-Space Ambient Occlusion, SSAO)�ļ����������������ǵĿ������µ�Σ���ϡ�
��һ����ʹ������Ļ�ռ䳡������ȶ�������ʵ�ļ�����������ȷ���ڱ�������һ��������������Ļ������ڱβ����ٶȿ죬���һ��ܻ�úܺõ�Ч����ʹ������Ϊ����ʵʱ�������ڱεı�׼��
SSAO�����ԭ��ܼ򵥣����������ı���(Screen-filled Quad)�ϵ�ÿһ��Ƭ�Σ����Ƕ�������ܱ����ֵ����һ���ڱ�����(Occlusion Factor)������ڱ�����֮��ᱻ�������ٻ��ߵ���Ƭ�εĻ������շ�����
�ڱ�������ͨ���ɼ�Ƭ����Χ���ͺ���(Kernel)�Ķ��������������͵�ǰƬ�����ֵ�Աȶ��õ��ġ�����Ƭ�����ֵ�����ĸ�������������Ҫ���ڱ����ӡ�

�����ԣ���ȾЧ���������;��������ǲ���������������ֱ�ӹ�ϵ�������������̫�ͣ���Ⱦ�ľ��Ȼἱ����٣����ǻ�õ�һ�ֽ�������(Banding)��Ч���������̫���ˣ�������Ӱ�����ܡ�
���ǿ���ͨ����������Ե���������(Sample Kernel)�Ĳ����дӶ�������������Ŀ��ͨ�������ת�������ģ����������������������еõ��������Ľ����
Ȼ������Ȼ����һ�����鷳����Ϊ�����������һ�������Ե�����ͼ�������ǽ���Ҫͨ��ģ��������޸���һ���⡣

Crytek��˾������SSAO���������һ��������Ӿ������Ϊʹ�õĲ���������һ�����壬������ƽ����ǽ��Ҳ���Եû����ɵģ���Ϊ������һ�������������ǽ����������ϡ�
���ǽ�����ʹ������Ĳ������ģ���ʹ��һ�����ű��淨�����İ�����������ġ�
ͨ���ڷ��������(Normal-oriented Hemisphere)��Χ���������ǽ����ῼ�ǵ�Ƭ�εײ��ļ�����.�������˻������ڱλ����ɵĸо����Ӷ���������ʵ�Ľ����

��������
SSAO��Ҫ��ȡ���������Ϣ����Ϊ������ҪһЩ��ʽ��ȷ��һ��Ƭ�ε��ڱ����ӡ�����ÿһ��Ƭ�Σ����ǽ���Ҫ��Щ���ݣ�
    ��Ƭ��λ������
    ��Ƭ�εķ�������
    ��Ƭ�εķ�����ɫ
    ��������
    ������ת�������ĵ������תʸ��
ͨ��ʹ��һ����Ƭ�ι۲�ռ�λ�ã����ǿ��Խ�һ������������Ķ�׼Ƭ�εĹ۲�ռ���淨�ߡ�����ÿһ�������������ǻ������������������ȽϽ�����������Ļ������תʸ����΢ƫתһ�㣻��������õ��ڱ����ӽ���֮�������������յĻ������շ�����
����SSAO��һ����Ļ�ռ似�ɣ����Ƕ�����2D�ı�����ÿһ��Ƭ�μ�����һЧ����Ҳ����˵����û�г����м��������Ϣ��
����������ֻ����Ⱦ���������ݵ���Ļ�ռ������У�����֮���ٻὫ�����ݷ��͵�SSAO��ɫ���У�֮�����Ǿ��ܷ��ʵ���Щ�����������ˡ�����㿴��ǰ��һƪ�̳̣���ᷢ������ӳ���Ⱦ�����ơ���Ҳ����˵SSAO���ӳ���Ⱦ�������ؼ��ݣ���Ϊ�����Ѿ���λ�úͷ���������G�������ˡ�

���������Ѿ�������Ƭ��λ�úͷ�������(G������)������ֻ��Ҫ����һ�¼�����ɫ������������Ƭ�ε�������Ⱦ����ˡ�������������Ȳ�����һ��ѧ����֪ʶ�����ǿ��Դ�gl_FragCoord.z����ȡ������ȣ�#version 330 core
    #version 330 core
    layout (location = 0) out vec4 gPositionDepth;
    layout (location = 1) out vec3 gNormal;
    layout (location = 2) out vec4 gAlbedoSpec;

    in vec2 TexCoords;
    in vec3 FragPos;
    in vec3 Normal;

    const float NEAR = 0.1; // ͶӰ����Ľ�ƽ��
    const float FAR = 50.0f; // ͶӰ�����Զƽ��
    float LinearizeDepth(float depth)
    {
        float z = depth * 2.0 - 1.0; // �ص�NDC
        return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));    
    }

    void main()
    {    
        // ����Ƭ�ε�λ��ʸ������һ��G��������
        gPositionDepth.xyz = FragPos;
        // ����������ȵ�gPositionDepth��alpha����
        gPositionDepth.a = LinearizeDepth(gl_FragCoord.z); 
        // ���淨����Ϣ��G����
        gNormal = normalize(Normal);
        // ����������ɫ
        gAlbedoSpec.rgb = vec3(0.95);
    }
��ȡ����������������ڹ۲�ռ��еģ�����֮�������Ҳ���ڹ۲�ռ��С�ȷ��G�����е�λ�úͷ��߶��ڹ۲�ռ���(���Ϲ۲����Ҳһ��)���۲�ռ��������ֵ֮��ᱻ������gPositionDepth��ɫ�����alpha�����У�ʡ������������һ���µ���ɫ��������

gPositionDepth��ɫ�����������ó�������������
    glGenTextures(1, &gPositionDepth);
    glBindTexture(GL_TEXTURE_2D, gPositionDepth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
���������һ����������������ǿ�����������ÿһ������������ȡ���ֵ��ע�����ǰ��������ֵ�洢Ϊ�˸������ݣ�������0.1��50.0��Χ���ֵ�����ᱻ������[0.0, 1.0]֮���ˡ�����㲻�ø���ֵ�洢��Щ������ݣ�ȷ�������Ƚ�ֵ����FAR����׼�����ǣ��ٴ洢��gPositionDepth�����У������Ժ����ɫ���������Ƶķ����ؽ����ǡ�ͬ����Ҫע�����GL_CLAMP_TO_EDGE�������װ�������Ᵽ֤�����ǲ��᲻С�Ĳ���������Ļ�ռ�������Ĭ����������֮������ֵ��
������������Ҫ�����İ���������ĺ�һЩ�����������ת��

�������
������Ҫ���ű��淨�߷������ɴ�������������������������̵̳Ŀ�ʼ���ܵ�������������Ҫ�����γɰ����ε����������ڶ�ÿ�����淨�߷������ɲ������ķǳ����ѣ�Ҳ����ʵ�ʣ����ǽ������߿ռ�(Tangent Space)�����ɲ������ģ���������ָ����z����
����������һ����λ�������ǿ��Ի��һ��ӵ�����64����ֵ�Ĳ������ģ�
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // �������������Χ0.0 - 1.0
    std::default_random_engine generator;
    std::vector<glm::vec3> ssaoKernel;
    for (GLuint i = 0; i < 64; ++i)
    {
        glm::vec3 sample(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator)
        );
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        GLfloat scale = GLfloat(i) / 64.0;
        ssaoKernel.push_back(sample);
    }
���������߿ռ�����-1.0��1.0Ϊ��Χ�任x��y���򣬲���0.0��1.0Ϊ��Χ�任������z����(�����-1.0��1.0Ϊ��Χ��ȡ�����ľͱ��������)�����ڲ������Ľ������ű��淨�߶��룬���õ�����ʸ�������ڰ����
Ŀǰ�����е���������ƽ���ֲ��ڲ���������ģ��������Ǹ�Ը�⽫�����ע����ڿ�������Ƭ�ε��ڱ��ϣ�Ҳ���ǽ�������������ԭ��ֲ������ǿ�����һ�����ٲ�ֵ����ʵ������
      ...[���Ϻ���]
       scale = lerp(0.1f, 1.0f, scale * scale);
       sample *= scale;
       ssaoKernel.push_back(sample);
    }
lerp������Ϊ��
    GLfloat lerp(GLfloat a, GLfloat b, GLfloat f)
    {
        return a + f * (b - a);
    }
��͸�������һ���󲿷���������ԭ��ĺ��ķֲ���

�������ת��
ͨ������һЩ����Ե����������ϣ����ǿ��Դ����ٻ�ò�����������������������ǿ��ԶԳ�����ÿһ��Ƭ�δ���һ�������ת�����������ܿ콫�ڴ�ľ������ԣ����õķ����Ǵ���һ��С�������ת��������ƽ������Ļ�ϡ�
���Ǵ���һ��4x4�������߿ռ�ƽ�淨�ߵ������ת�������飺
    std::vector<glm::vec3> ssaoNoise;
    for (GLuint i = 0; i < 16; i++)
    {
        glm::vec3 noise(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            0.0f);
        ssaoNoise.push_back(noise);
    }
���ڲ���������������z���������߿ռ�����ת�������趨z����Ϊ0.0���Ӷ�Χ��z����ת��
���ǽ���������һ�����������ת������4x4�����ǵ��趨���ķ�װ����ΪGL_REPEAT���Ӷ���֤�����ʵ�ƽ������Ļ�ϡ�
    GLuint noiseTexture;
    glGenTextures(1, &noiseTexture);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
���������������е�����������ݣ�������������Ҫʵ��SSAO��

SSAO��ɫ��
SSAO��ɫ����2D�������ı��������У�������ÿһ�����ɵ�Ƭ�μ����ڱ�ֵ(Ϊ�������յĹ�����ɫ����ʹ��)������������Ҫ�洢SSAO�׶εĽ�������ǻ���Ҫ�ڴ���һ��֡�������
    GLuint ssaoFBO;
    glGenFramebuffers(1, &ssaoFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    GLuint ssaoColorBuffer;

    glGenTextures(1, &ssaoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
���ڻ����ڱεĽ����һ���Ҷ�ֵ�����ǽ�ֻ��Ҫ����ĺ�ɫ�������������ǽ���ɫ������ڲ���ʽ����ΪGL_RED��

��ȾSSAO�����Ĺ��̻���������
    // ���δ���׶�: ��Ⱦ��G������
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        [...]
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ʹ��G������ȾSSAO����
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderSSAO.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPositionDepth);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        SendKernelSamplesToShader();
        glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(projection));
        RenderQuad();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ���մ���׶�: ��Ⱦ��������
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderLightingPass.Use();
    [...]
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    [...]
    RenderQuad();
shaderSSAO�����ɫ������ӦG��������(�����������)����������ͷ���������������Ϊ���������
    #version 330 core
    out float FragColor;
    in vec2 TexCoords;

    uniform sampler2D gPositionDepth;
    uniform sampler2D gNormal;
    uniform sampler2D texNoise;

    uniform vec3 samples[64];
    uniform mat4 projection;

    // ��Ļ��ƽ����������������Ļ�ֱ��ʳ���������С��ֵ������
    const vec2 noiseScale = vec2(800.0/4.0, 600.0/4.0); // ��Ļ = 800x600

    void main()
    {
        [...]
    }
ע������������һ��noiseScale�ı�����������Ҫ����������ƽ��(Tile)����Ļ�ϣ���������TexCoords��ȡֵ��0.0��1.0֮�䣬texNoise��������ƽ�̡��������ǽ�ͨ����Ļ�ֱ��ʳ������������С�ķ�ʽ����TexCoords�����Ŵ�С������֮����ȡ�������������ʱ��ʹ�á�
    vec3 fragPos = texture(gPositionDepth, TexCoords).xyz;
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 randomVec = texture(texNoise, TexCoords * noiseScale).xyz;
�������ǽ�texNoise��ƽ�̲�������ΪGL_REPEAT�������ֵ������ȫ�������ظ�������fragPog��normal���������Ǿ����㹻������������һ��TBN���󣬽����������߿ռ�任���۲�ռ䡣
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
ͨ��ʹ��һ������Gramm-Schmidt����(Gramm-Schmidt Process)�Ĺ��̣����Ǵ�����һ��������(Orthogonal Basis)��ÿһ�����������randomVec��ֵ��΢��б��ע����Ϊ����ʹ����һ�����������������������������û��Ҫ��һ��ǡ�����ż���������TBN����Ҳ���ǲ���Ҫ�𶥵�����(��˫��)������

���������Ƕ�ÿ�������������е����������������߿ռ�任���۲�ռ䣬�����Ǽӵ���ǰ����λ���ϣ�����Ƭ��λ������봢����ԭʼ��Ȼ����е�������Ƚ��бȽϡ�������һ������������
    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {
        // ��ȡ����λ��
        vec3 sample = TBN * samples[i]; // ����->�۲�ռ�
        sample = fragPos + sample * radius;

        [...]
    }
�����kernelSize��radius������������������Ч�������������Ƿֱ𱣳����ǵ�Ĭ��ֵΪ64��1.0������ÿһ�ε����������ȱ任�����������۲�ռ䡣֮�����ǻ�ӹ۲�ռ����ƫ���������۲�ռ�Ƭ��λ���ϣ��������radius����ƫ������������(�����)SSAO����Чȡ���뾶��
���������Ǳ任sample����Ļ�ռ䣬�Ӷ����ǿ��Ծ�������ֱ����Ⱦ����λ�õ���Ļ��һ��ȡ��sample��(����)���ֵ�������������Ŀǰ�ڹ۲�ռ䣬���ǽ�����ʹ��projection����uniform�任�����ü��ռ䡣
�ڱ������任���ü��ռ�֮��������xyz��������w��������͸�ӻ��֡�������õı�׼���豸����֮��任��[0.0, 1.0]��Χ�Ա�����ʹ������ȥȡ���������
    float sampleDepth = -texture(gPositionDepth, offset.xy).w;
����ʹ��offset������x��y�������������������Ӷ���ȡ����λ�ôӹ۲����ӽǵ����ֵ(��һ�������ڱεĿɼ�Ƭ��)�����ǽ�������������ĵ�ǰ���ֵ�Ƿ���ڴ洢�����ֵ������ǵģ���ӵ����յĹ��������ϡ�
    occlusion += (sampleDepth >= sample.z ? 1.0 : 0.0);
�Ⲣû����ȫ��������Ϊ��Ȼ����һ��С������Ҫ���ǡ������һ�����������Ե��Ƭ��ʱ�������ῼ�ǲ��Ա���֮�µı�������ֵ����Щֵ����(����ȷ��)Ӱ���ڱ����ӡ����ǿ���ͨ������һ����Χ���Ӷ�����������
��������һ����Χ���ԴӶ���֤����ֻ���������ֵ��ȡ���뾶��ʱӰ���ڱ����ӡ����������һ�л��ɣ�
    float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
    occlusion += (sampleDepth >= sample.z ? 1.0 : 0.0) * rangeCheck;
��������ʹ����GLSL��smoothstep���������ǳ��⻬���ڵ�һ�͵ڶ���������Χ�ڲ�ֵ�˵����������������Ȳ��������ȡֵ��radius֮�䣬���ǵ�ֵ����⻬�ظ�������������߲�ֵ��0.0��1.0֮�䣺
�������ʹ��һ�������ֵ��radius֮���ͻȻ�Ƴ��ڱι��׵�Ӳ���޷�Χ���(Hard Cut-off Range Check)�����ǽ����ڷ�Χ���Ӧ�õĵط�����һ�����Ե�(���ѿ���)��Ե��
���һ����������Ҫ���ڱι��׸��ݺ��ĵĴ�С��׼��������������ע��������1.0��ȥ���ڱ����ӣ��Ա�ֱ��ʹ���ڱ�����ȥ���Ż������շ�����
    }
    occlusion = 1.0 - (occlusion / kernelSize);
    FragColor = occlusion;
���ڵ�Ч����Ȼ���������Ǻ������������ظ�������������ͼ�������ɼ���Ϊ�˴���һ���⻬�Ļ����ڱν����������Ҫģ�������ڱ�����

�����ڱ�ģ��
��SSAO�׶κ͹��ս׶�֮�䣬������Ҫ����ģ��SSAO����Ĵ������������ִ�����һ��֡�������������ģ�������
    GLuint ssaoBlurFBO, ssaoColorBufferBlur;
    glGenFramebuffers(1, &ssaoBlurFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glGenTextures(1, &ssaoColorBufferBlur);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
����ƽ�̵����������������һ�µ�����ԣ����ǿ���ʹ����һ����������һ���򵥵�ģ����ɫ����
    #version 330 core
    in vec2 TexCoords;
    out float fragColor;

    uniform sampler2D ssaoInput;

    void main() {
        vec2 texelSize = 1.0 / vec2(textureSize(ssaoInput, 0));
        float result = 0.0;
        for (int x = -2; x < 2; ++x)
        {
            for (int y = -2; y < 2; ++y)
            {
                vec2 offset = vec2(float(x), float(y)) * texelSize;
                result += texture(ssaoInput, TexCoords + offset).r;
            }
        }
        fragColor = result / (4.0 * 4.0);
    }
�������Ǳ�������Χ��-2.0��2.0֮���SSAO����Ԫ(Texel)����������������ά����ͬ������SSAO��������ͨ��ʹ�÷���vec2����ά�ȵ�textureSize����������Ԫ����ʵ��Сƫ����ÿһ���������ꡣ����ƽ�����õĽ�������һ���򵥵�����Ч��ģ��Ч��

Ӧ�û����ڱ�
Ӧ���ڱ����ӵ����շ����м���򵥣�����Ҫ����ֻ�ǽ���Ƭ�λ����ڱ����ӳ˵����ջ��������ϡ��������ʹ���ϸ��̳��е�Blinn-Phong�ӳٹ�����ɫ��������һ���޸ģ����ǽ���õ��������Ƭ����ɫ����
    #version 330 core
    out vec4 FragColor;
    in vec2 TexCoords;

    uniform sampler2D gPositionDepth;
    uniform sampler2D gNormal;
    uniform sampler2D gAlbedo;
    uniform sampler2D ssao;

    struct Light {
        vec3 Position;
        vec3 Color;

        float Linear;
        float Quadratic;
        float Radius;
    };
    uniform Light light;

    void main()
    {
        // ��G��������ȡ����
        vec3 FragPos = texture(gPositionDepth, TexCoords).rgb;
        vec3 Normal = texture(gNormal, TexCoords).rgb;
        vec3 Diffuse = texture(gAlbedo, TexCoords).rgb;
        float AmbientOcclusion = texture(ssao, TexCoords).r;

        // Blinn-Phong (�۲�ռ���)
        vec3 ambient = vec3(0.3 * AmbientOcclusion); // �������Ǽ����ڱ�����
        vec3 lighting  = ambient;
        vec3 viewDir  = normalize(-FragPos); // Viewpos Ϊ (0.0.0)���ڹ۲�ռ���
        // ������
        vec3 lightDir = normalize(light.Position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * light.Color;
        // ����
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 8.0);
        vec3 specular = light.Color * spec;
        // ˥��
        float dist = length(light.Position - FragPos);
        float attenuation = 1.0 / (1.0 + light.Linear * dist + light.Quadratic * dist * dist);
        diffuse  *= attenuation;
        specular *= attenuation;
        lighting += diffuse + specular;

        FragColor = vec4(lighting, 1.0);
    }
(���˽���ĵ��۲�ռ�)�Ա���֮ǰ�Ĺ���ʵ�֣�Ψһ�������Ķ����ǳ�������������AmbientOcclusionֵ�ĳ˷���ͨ���ڳ����м���һ������ɫ�ĵ��Դ

��Ļ�ռ价���ڱ���һ���ɸ߶��Զ����Ч��������Ч���ܴ�̶������������Ǹ��ݳ������͵������Ĳ��������������͵ĳ�����������ʲô�����Ĳ�����Ϸ�ʽ��һЩ����ֻ��С�뾶����¹���������Щ��������Ҫ����İ뾶�͸���������������ܿ���������ʵ����ǰ�����ʾ����64�����������ڱȽ϶���ˣ�����Ե�����С�ĺ��Ĵ�С�Ӷ���ø��õĽ����
һЩ����Ե���(����˵ͨ��uniform)�Ĳ��������Ĵ�С���뾶��/���������ĵĴ�С����Ҳ�����������յ��ڱ�ֵ��һ���û�������ݴӶ���������ǿ�ȣ�
    occlusion = 1.0 - (occlusion / kernelSize);
    FragColor = pow(occlusion, power);






*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path, bool gammaCorrection);
void renderQuad();
void renderCube();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float ourLerp(float a, float b, float f)
{
    return a + f * (b - a);
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader shaderGeometryPass("res/Shaders/Shader_advancedLight/9/ssao_geometry.vert", "res/Shaders/Shader_advancedLight/9/ssao_geometry.frag");
    Shader shaderLightingPass("res/Shaders/Shader_advancedLight/9/ssao.vert", "res/Shaders/Shader_advancedLight/9/ssao_lighting.frag");
    Shader shaderSSAO("res/Shaders/Shader_advancedLight/9/ssao.vert", "res/Shaders/Shader_advancedLight/9/ssao.frag");
    Shader shaderSSAOBlur("res/Shaders/Shader_advancedLight/9/ssao.vert", "res/Shaders/Shader_advancedLight/9/ssao_blur.frag");

    // load models
    // -----------
    //Model backpack("res/object/nanosuit/nanosuit.obj");
    Model backpack("res/object/backpack/backpack.obj");

    // configure g-buffer framebuffer
    // ------------------------------
    unsigned int gBuffer;
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    unsigned int gPosition, gNormal, gAlbedo;
    // position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    // normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    // color + specular color buffer
    glGenTextures(1, &gAlbedo);
    glBindTexture(GL_TEXTURE_2D, gAlbedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);
    // create and attach depth buffer (renderbuffer)
    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // also create framebuffer to hold SSAO processing stage 
    // -----------------------------------------------------
    unsigned int ssaoFBO, ssaoBlurFBO;
    glGenFramebuffers(1, &ssaoFBO);  glGenFramebuffers(1, &ssaoBlurFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    unsigned int ssaoColorBuffer, ssaoColorBufferBlur;
    // SSAO color buffer
    glGenTextures(1, &ssaoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "SSAO Framebuffer not complete!" << std::endl;
    // and blur stage
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glGenTextures(1, &ssaoColorBufferBlur);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // generate sample kernel
    // ----------------------
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
    std::default_random_engine generator;
    std::vector<glm::vec3> ssaoKernel;
    for (unsigned int i = 0; i < 64; ++i)
    {
        glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        float scale = float(i) / 64.0f;

        // scale samples s.t. they're more aligned to center of kernel
        scale = ourLerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        ssaoKernel.push_back(sample);
    }

    // generate noise texture
    // ----------------------
    std::vector<glm::vec3> ssaoNoise;
    for (unsigned int i = 0; i < 16; i++)
    {
        glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
        ssaoNoise.push_back(noise);
    }
    unsigned int noiseTexture; glGenTextures(1, &noiseTexture);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // lighting info
    // -------------
    glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
    glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);

    // shader configuration
    // --------------------
    shaderLightingPass.use();
    shaderLightingPass.setInt("gPosition", 0);
    shaderLightingPass.setInt("gNormal", 1);
    shaderLightingPass.setInt("gAlbedo", 2);
    shaderLightingPass.setInt("ssao", 3);
    shaderSSAO.use();
    shaderSSAO.setInt("gPosition", 0);
    shaderSSAO.setInt("gNormal", 1);
    shaderSSAO.setInt("texNoise", 2);
    shaderSSAOBlur.use();
    shaderSSAOBlur.setInt("ssaoInput", 0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 1. geometry pass: render scene's geometry/color data into gbuffer
        // -----------------------------------------------------------------
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 50.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        shaderGeometryPass.use();
        shaderGeometryPass.setMat4("projection", projection);
        shaderGeometryPass.setMat4("view", view);
        // room cube
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0, 7.0f, 0.0f));
        model = glm::scale(model, glm::vec3(7.5f, 7.5f, 7.5f));
        shaderGeometryPass.setMat4("model", model);
        shaderGeometryPass.setInt("invertedNormals", 1); // invert normals as we're inside the cube
        renderCube();
        shaderGeometryPass.setInt("invertedNormals", 0);
        // backpack model on the floor
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
        model = glm::scale(model, glm::vec3(1.0f));
        shaderGeometryPass.setMat4("model", model);
        backpack.Draw(shaderGeometryPass);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        // 2. generate SSAO texture
        // ------------------------
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderSSAO.use();
        // Send kernel + rotation 
        for (unsigned int i = 0; i < 64; ++i)
            shaderSSAO.setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
        shaderSSAO.setMat4("projection", projection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, noiseTexture);
        renderQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        // 3. blur SSAO texture to remove noise
        // ------------------------------------
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderSSAOBlur.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        renderQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        // 4. lighting pass: traditional deferred Blinn-Phong lighting with added screen-space ambient occlusion
        // -----------------------------------------------------------------------------------------------------
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderLightingPass.use();
        // send light relevant uniforms
        glm::vec3 lightPosView = glm::vec3(camera.GetViewMatrix() * glm::vec4(lightPos, 1.0));
        shaderLightingPass.setVec3("light.Position", lightPosView);
        shaderLightingPass.setVec3("light.Color", lightColor);
        // Update attenuation parameters
        const float linear = 0.09f;
        const float quadratic = 0.032f;
        shaderLightingPass.setFloat("light.Linear", linear);
        shaderLightingPass.setFloat("light.Quadratic", quadratic);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gAlbedo);
        glActiveTexture(GL_TEXTURE3); // add extra SSAO texture to lighting pass
        glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
        renderQuad();


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
             // bottom face
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
              1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             // top face
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
              1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
              1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
              1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}


// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKayboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKayboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKayboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKayboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}