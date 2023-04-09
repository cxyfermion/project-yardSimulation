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

/*
�ӳ���ɫ��
��������һֱʹ�õĹ��շ�ʽ����������Ⱦ(Forward Rendering)����������ɫ��(Forward Shading)������������Ⱦ�����һ�ַǳ�ֱ�ӵķ�ʽ���ڳ��������Ǹ������й�Դ����һ�����壬֮������Ⱦ��һ�����壬�Դ����ơ�
���ǳ�������⣬Ҳ������ʵ�֣�����ͬʱ���Գ������ܵ�Ӱ��Ҳ�ܴ���Ϊ����ÿһ����Ҫ��Ⱦ�����壬����Ҫ��ÿһ����Դÿһ����Ҫ��Ⱦ��Ƭ�ν��е��������Ƿǳ���ģ���Ϊ�󲿷�Ƭ����ɫ����������ᱻ֮���������ǣ�������Ⱦ�����ڳ�������Ϊ����ĸ��Ӷ�(��������غ���һ��������)�˷Ѵ�����Ƭ����ɫ������ʱ�䡣
�ӳ���ɫ��(Deferred Shading)������˵���ӳ���Ⱦ(Deferred Rendering)��Ϊ�˽����������������ˣ�������ȵظı���������Ⱦ����ķ�ʽ����������Ż�ӵ�д�����Դ�ĳ����ṩ�˺ܶ��ѡ����Ϊ���ܹ�����Ⱦ�ϰ�������ǧ��Դ��ͬʱ���ܹ����������˽��ܵ�֡�ʡ�

�ӳ���ɫ�����������ӳ�(Defer)���Ƴ�(Postpone)�󲿷ּ������ǳ������Ⱦ(���ǹ���)�����ڽ��д�����뷨����������������׶�(Pass)
�ڵ�һ�����δ���׶�(Geometry Pass)�У���������Ⱦ����һ�Σ�֮���ȡ����ĸ��ּ�����Ϣ����������һϵ�н���G����(G-buffer)�������У�����λ������(Position Vector)����ɫ����(Color Vector)��������(Normal Vector)��/����ֵ(Specular Value)��
��������Щ������G�����еļ�����Ϣ������֮��������(�����ӵ�)���ռ��㡣
���ǻ��ڵڶ������մ���׶�(Lighting Pass)��ʹ��G�����ڵ��������ݡ��ڹ��մ���׶��У�������Ⱦһ����Ļ��С�ķ��Σ���ʹ��G�����еļ������ݶ�ÿһ��Ƭ�μ��㳡���Ĺ��գ���ÿ�����������Ƕ����G������е�����
���Ƕ�����Ⱦ���̽��н�������߼���Ƭ�δ���Ų�����ڽ��У�������ֱ�ӽ�ÿ������Ӷ�����ɫ������Ƭ����ɫ�������ռ�����̻��Ǻ�������ǰһ������������������Ҫ�Ӷ�Ӧ��G��������Ƕ�����ɫ��(��һЩuniform����)�����ȡ��������ˡ�

������Ⱦ����һ���ܴ�ĺô������ܱ�֤��G�����е�Ƭ�κ�����Ļ�ϳ��ֵ�������������Ƭ����Ϣ��һ���ģ���Ϊ��Ȳ����Ѿ����ս������Ƭ����Ϣ��Ϊ����Ƭ�Ρ�������֤�˶����ڹ��մ���׶��д����ÿһ�����ض�ֻ����һ�Σ����������ܹ�ʡ�ºܶ����õ���Ⱦ���á�����֮�⣬�ӳ���Ⱦ������������������Ż����Ӷ���Ⱦ����Ĺ�Դ��
��Ȼ���ַ���Ҳ��������ȱ�ݣ� ����G����Ҫ��������������ɫ�����д洢��ԱȽϴ�ĳ������ݣ�������ıȽ϶���Դ棬����������λ������֮�����Ҫ�߾��ȵĳ������ݡ� ����һ��ȱ���������֧�ֻ�ɫ(��Ϊ����ֻ����ǰ���Ƭ����Ϣ)�� ���Ҳ����ʹ��MSAA�ˡ�����⼸���������ǿ�����һЩ��ͨ���˷���Щȱ�㣬��Щ���������ڽ̵̳�������ۡ�
�ڼ��δ���׶������G����ǳ���Ч����Ϊ����ֱ�Ӵ�������λ�ã���ɫ�����Ƿ��ߵȶ�����Ϣ��֡�����У����⼸���������Ĵ���ʱ�䡣�ڴ˻�����ʹ�ö���ȾĿ��(Multiple Render Targets, MRT)��������������������һ����Ⱦ����֮����������еĹ�����

G����
G����(G-buffer)�Ƕ������������������ص����ݣ��������Ĺ��մ���׶���ʹ�õ�����������ܳơ��ô˻��ᣬ������˳�㸴ϰһ����������Ⱦ������һ��Ƭ������Ҫ���������ݣ�
    һ��3Dλ������������(��ֵ)Ƭ��λ�ñ�����lightDir��viewDirʹ��
    һ��RGB��������ɫ������Ҳ���Ƿ�����(Albedo)
    һ��3D���������ж�ƽ���б��
    һ������ǿ��(Specular Intensity)����ֵ
    ���й�Դ��λ�ú���ɫ����
    ��һ��߹۲��ߵ�λ������
������Щ(��Ƭ��)�����Ĵ���Ȩ�����Ǿ��ܹ��������Ǻ���Ϥ��(����-)���Ϲ���(Blinn-Phong Lighting)�ˡ���Դ��λ�ã���ɫ������ҵĹ۲�λ�ÿ���ͨ��uniform���������ã�����������������ÿ�������Ƭ�ζ��ǲ�ͬ�ġ������������ĳ�ַ�ʽ������ȫ��ͬ�����ݵ����յ��ӳٹ��մ���׶��У����Ǿ��ܼ�����֮ǰ��ͬ�Ĺ���Ч���ˣ���������ֻ������Ⱦһ��2D���ε�Ƭ�Ρ�
OpenGL��û���������������������ܴ洢�Ķ���������������Ӧ�������һ��������Ļ��С�������д���������Ƭ�����ݲ���֮����մ���׶���ʹ�õĿ������ˡ���ΪG����������͹��մ���׶��е�2D����һ�������ǻ��ú�������Ⱦ������ȫһ����Ƭ�����ݣ����ڹ��մ���׶�������һ��һӳ�䡣
����������α�����л��������ģ�
    while(...) // ��Ϸѭ��
    {
        // 1. ���δ���׶Σ���Ⱦ���еļ���/��ɫ���ݵ�G����
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gBufferShader.Use();
        for(Object obj : Objects)
        {
            ConfigureShaderTransformsAndUniforms();
            obj.Draw();
        }
        // 2. ���մ���׶Σ�ʹ��G������㳡���Ĺ���
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        lightingPassShader.Use();
        BindAllGBufferTextures();
        SetLightingUniforms();
        RenderQuad();
    }
����ÿһ��Ƭ��������Ҫ����������У�һ��λ��������һ����������һ����ɫ������һ������ǿ��ֵ�����������ڼ��δ���׶�����Ҫ��Ⱦ���������еĶ��󲢴�����Щ���ݷ�����G�����С�
���ǿ����ٴ�ʹ�ö���ȾĿ��(Multiple Render Targets)����һ����Ⱦ����֮����Ⱦ�����ɫ���壬��֮ǰ�ķ���̳�������Ҳ�򵥵��ἰ������
���ڼ�����Ⱦ����׶Σ�����������Ҫ��ʼ��һ��֡����������Ǻ�ֱ�۵ĳ���ΪgBuffer���������˶����ɫ�����һ�������������Ⱦ�������(Depth Renderbuffer Object)��
����λ�úͷ���������������ϣ��ʹ�ø߾��ȵ�����(ÿ����16��32λ�ĸ�����)�������ڷ����ʺ;���ֵ��ʹ��Ĭ�ϵ�����(ÿ����8λ������)�͹��ˡ�
    GLuint gBuffer;
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    GLuint gPosition, gNormal, gColorSpec;

    // - λ����ɫ����
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0

    // - ������ɫ����
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    // - ��ɫ + ������ɫ����
    glGenTextures(1, &gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

    // - ����OpenGL���ǽ�Ҫʹ��(֡�����)������ɫ������������Ⱦ
    GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    // ֮��ͬ�������Ⱦ�������(Render Buffer Object)Ϊ��Ȼ���(Depth Buffer)�������������
    [...]
��������ʹ���˶���ȾĿ�꣬������Ҫ��ʽ����OpenGL������Ҫʹ��glDrawBuffers��Ⱦ���Ǻ�GBuffer�������ĸ���ɫ���塣ͬ����Ҫע����ǣ�����ʹ��RGB����������λ�úͷ��ߵ����ݣ���Ϊÿ������ֻ�������������������ǽ���ɫ�;���ǿ�����ݺϲ���һ�𣬴洢��һ��������RGBA�������棬�������ǾͲ���Ҫ����һ���������ɫ���������ˡ���������ӳ���Ⱦ���߱��Խ��Խ���ӣ���Ҫ��������ݵ�ʱ����ͻ�ܿ췢���µķ�ʽ��������ݵ�һ�������������С�
������������Ҫ��Ⱦ���ǵ�G�����С�����ÿ�������������䣬һ�����ߺ�һ������ǿ���������ǻ���ʹ��һЩ���������Ƭ����ɫ���Ķ�������Ⱦ���ǵ�G������ȥ��
    #version 330 core
    layout (location = 0) out vec3 gPosition;
    layout (location = 1) out vec3 gNormal;
    layout (location = 2) out vec4 gAlbedoSpec;
    in vec2 TexCoords;
    in vec3 FragPos;
    in vec3 Normal;
    uniform sampler2D texture_diffuse1;
    uniform sampler2D texture_specular1;
    void main()
    {
        // �洢��һ��G���������е�Ƭ��λ������
        gPosition = FragPos;
        // ͬ���洢��ÿ����Ƭ�η��ߵ�G������
        gNormal = normalize(Normal);
        // ���������ÿ����Ƭ����ɫ
        gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
        // �洢����ǿ�ȵ�gAlbedoSpec��alpha����
        gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;
    }
��Ϊ����ʹ���˶���ȾĿ�꣬�������ָʾ��(Layout Specifier)������OpenGL������Ҫ��Ⱦ����ǰ�Ļ�Ծ֡�����е���һ����ɫ���塣ע�����ǲ�û�д��澵��ǿ�ȵ�һ����������ɫ���������У���Ϊ���ǿ��Դ����������ĸ���ֵ��������ɫ���������alpha�����С�
���ס����Ϊ�й��ռ��㣬���Ա�֤���б�����һ������ռ䵱��������Ҫ������������������ռ��д洢(������)���еı�����

�ӳٹ��մ���׶�
���������Ѿ�����һ��ѵ�Ƭ�����ݴ�����G�����й����Ǵ��ã����ǿ���ѡ��ͨ��һ������һ�����صر�������G���������������������������������Ϊ�����㷨�����룬����ȫ���㳡�����յĹ�����ɫ��
�������е�G������������������ձ任��Ƭ��ֵ������ֻ��Ҫ��ÿһ������ִ��һ�ΰ���Ĺ�����������ˡ���ʹ���ӳٹ��շǳ���Ч���ر�������Ҫ���ô�������Ƭ����ɫ���ĸ��ӳ����С�

����������մ���׶Σ����ǽ�����Ⱦһ��2Dȫ���ķ���(��һ������ڴ���Ч��)������ÿ������������һ������Ĺ���Ƭ����ɫ����
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderLightingPass.Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    // ͬ�����͹�����ص�uniform
    SendAllLightUniformsToShader(shaderLightingPass);
    glUniform3fv(glGetUniformLocation(shaderLightingPass.Program, "viewPos"), 1, &camera.Position[0]);
    RenderQuad();
��������Ⱦ֮ǰ����G������������ص��������ҷ��͹�����ص�uniform��������ɫ���С�
���մ���׶ε�Ƭ����ɫ��������֮ǰһֱ���õĹ��ս̳���ɫ���Ƿǳ����Ƶģ��������������һ���µķ������Ӷ�ʹ�����ܹ���ȡ���յ������������Ȼ��Щ�������ǻ��G������ֱ�Ӳ�����
    #version 330 core
    out vec4 FragColor;
    in vec2 TexCoords;

    uniform sampler2D gPosition;
    uniform sampler2D gNormal;
    uniform sampler2D gAlbedoSpec;

    struct Light {
        vec3 Position;
        vec3 Color;
    };
    const int NR_LIGHTS = 32;
    uniform Light lights[NR_LIGHTS];
    uniform vec3 viewPos;

    void main()
    {
        // ��G�����л�ȡ����
        vec3 FragPos = texture(gPosition, TexCoords).rgb;
        vec3 Normal = texture(gNormal, TexCoords).rgb;
        vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
        float Specular = texture(gAlbedoSpec, TexCoords).a;

        // Ȼ�������һ���ؼ������
        vec3 lighting = Albedo * 0.1; // Ӳ���뻷�����շ���
        vec3 viewDir = normalize(viewPos - FragPos);
        for(int i = 0; i < NR_LIGHTS; ++i)
        {
            // ������
            vec3 lightDir = normalize(lights[i].Position - FragPos);
            vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * lights[i].Color;
            lighting += diffuse;
        }

        FragColor = vec4(lighting, 1.0);
    }
���մ���׶���ɫ����������uniform��������G���壬���ǰ����������ڼ��δ���׶δ�����������ݡ��������������ʹ�õ�ǰƬ�ε��������������Щ���ݣ����ǽ����ú�֮ǰ��ȫһ����Ƭ��ֵ�������������ֱ����Ⱦ�����塣
��Ƭ����ɫ����һ��ʼ������ͨ��һ���򵥵�������Ҵ�G���������л�ȡ�˹�����صı�����ע�����Ǵ�gAlbedoSpec������ͬʱ��ȡ��Albedo��ɫ��Spqcularǿ�ȡ�
��Ϊ���������Ѿ����˱�Ҫ����Ƭ�α���(����ص�uniform����)�����㲼��-���Ϲ���(Blinn-Phong Lighting)�����ǲ���Ҫ�Թ��մ������κ��޸��ˡ��������ӳ���ɫ����Ψһ��Ҫ�ĵľ��ǻ�ȡ������������ķ�����
�ӳ���ɫ��������һ��ȱ����������ܽ��л��(Blending)����ΪG���������е����ݶ��Ǵ�һ��������Ƭ�������ģ��������Ҫ�Զ��Ƭ�ε���Ͻ��в������ӳ���ɫ������һ��ȱ���������ʹ��Դ󲿷ֳ����Ĺ���ʹ����ͬ�Ĺ����㷨�������ͨ������������ڲ��ʵ����ݵ�G��������������һȱ�㡣
Ϊ�˿˷���Щȱ��(�ر��ǻ��)������ͨ���ָ����ǵ���Ⱦ��Ϊ�������֣�һ�����ӳ���Ⱦ�Ĳ��֣���һ����ר��Ϊ�˻�ϻ����������ʺ��ӳ���Ⱦ���ߵ���ɫ��Ч������Ƶĵ�������Ⱦ�Ĳ��֡�

����ӳ���Ⱦ��������Ⱦ
����������Ҫ��Ⱦÿһ����ԴΪһ��3D�����壬�������ڹ�Դ��λ���������ӳ���Ⱦ��һ�𷢳���Դ����ɫ�������ԣ�������Ҫ���ĵ�һ���¾������ӳ���Ⱦ����֮��������Ⱦ���еĹ�Դ���������ӳ���Ⱦ���ߵ������С���������ֻ��Ҫ�������������Ⱦ�����壬ֻ�ǻ�����������ӳ���Ⱦ����֮����С��������������
    // �ӳ���Ⱦ������Ⱦ�׶�
    [...]
    RenderQuad();

    // �������������һ��������Ⱦ���й�������
    shaderLightBox.Use();
    glUniformMatrix4fv(locProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
    for (GLuint i = 0; i < lightPositions.size(); i++)
    {
        model = glm::mat4();
        model = glm::translate(model, lightPositions[i]);
        model = glm::scale(model, glm::vec3(0.25f));
        glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(locLightcolor, 1, &lightColors[i][0]);
        RenderCube();
    }
Ȼ������Щ��Ⱦ�����������岢û�п��ǵ����Ǵ�����ӳ���Ⱦ���ļ������(Depth)��Ϣ�����ҽ����������Ⱦ��֮ǰ��Ⱦ��������֮�ϣ��Ⲣ����������Ҫ�Ľ����
������Ҫ���ľ������ȸ��Ƴ��ڼ�����Ⱦ�׶��д���������Ϣ���������Ĭ�ϵ�֡�������Ȼ��壬Ȼ�����ǲ���Ⱦ�������塣����֮��ֻ�е�����֮ǰ��Ⱦ���ļ������Ϸ���ʱ�򣬹��������Ƭ�βŻᱻ��Ⱦ������
���ǿ���ʹ��glBlitFramebuffer����һ��֡��������ݵ���һ��֡�����У������������Ҳ�ڿ���ݵĽ̳���ʹ�ù���������ԭ���ز�����֡���塣glBlitFramebuffer��������������Ǹ���һ���û������֡����������һ���û������֡��������
���Ǵ��������ӳ���Ⱦ�׶�����������������Ϣ��gBuffer���FBO�С�������ǽ����Ǽ򵥸���������Ȼ������ݵ�Ĭ��֡�������Ȼ����У���ô��������ͻ����ǳ��������еļ����嶼��������Ⱦ������һ����Ⱦ������
�����ڿ���ݽ̳��н��ܵ�������������Ҫָ��һ��֡����Ϊ��֡����(Read Framebuffer)���������Ƶ�ָ��һ��֡����Ϊд֡����(Write Framebuffer)��
    glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // д�뵽Ĭ��֡����
    glBlitFramebuffer(
      0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST
    );
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // ������֮ǰһ����Ⱦ��������
    [...]
���������Ǹ���������֡�������Ȼ�����Ϣ��Ĭ��֡�������Ȼ��壬������ɫ�����ģ�建������Ҳ����������������������ǽ���������Ⱦ�������壬������ļ����彫�ῴ��������ʵ�ˣ�����ֻ�Ǽ򵥵�ճ�������嵽2D����֮��
�������ַ��������Ǿ��ܹ����׵ؽ���ӳ���ɫ����������ɫ���ˡ�������̫���ˣ��������ڿ���Ӧ�û�ϻ�����Ⱦ��Ҫ������ɫ��Ч���������ˣ������ӳ���Ⱦ���ǲ����������ġ�

����Ĺ�Դ
�ӳ���Ⱦһֱ�����޵�ԭ��������ܹ���Ⱦ�����Ĺ�Դ�������Ĵ��������ܡ�Ȼ�����ӳ���Ⱦ����������֧�ַǳ������Ĺ�Դ����Ϊ������Ȼ����Ҫ�Գ�����ÿһ����Դ����ÿһ��Ƭ�εĹ��շ����������ô�����Դ��Ϊ���ܵ��������ܹ����ӳ���Ⱦ�������õ�һ���ǳ������Ż��������(Light Volumes)
ͨ������£���������Ⱦһ�����ӹ��ճ����µ�Ƭ����ɫ��ʱ�����ǻ���㳡����ÿһ����Դ�Ĺ��ף��������������Ƭ���ж�Զ���ܴ�һ���ֵĹ�Դ�����Ͳ��ᵽ�����Ƭ�Σ�����Ϊʲô���ǻ�Ҫ�˷���ô����������أ�
�����ڹ����������뷨���Ǽ����Դ�İ뾶�����������Ҳ���ǹ��ܹ�����Ƭ�εķ�Χ�����ڴ󲿷ֹ�Դ��ʹ����ĳ����ʽ��˥��(Attenuation)�����ǿ��������������Դ�ܹ���������·�̣�����˵�ǰ뾶�����ǽ�����ֻ��Ҫ����Щ��һ������������ڵ�Ƭ�ν��з��صĹ�����������ˡ�����Ը�����ʡ�����ܿɹ۵ļ���������Ϊ��������ֻ����Ҫ������¼�����ա�
����ʹ�õ�˥�����������Ŀ��ӷ�Χ�ڻ������Ǻڰ��ģ��������������Ҫ������Ϊһ����5/256
���Ӻڰ������ȣ�������ͻ���̫��Ӷ���õ�Ч��ֻҪ���û������ڹ������Ե����һ��ͻأ�Ľضϣ����������û���ˡ���Ȼ�����������ڳ��������ͣ�һ���ߵ����ȷ�ֵ�������С�Ĺ�������Ӷ���ø��ߵ�Ч�ʣ�Ȼ����ͬ�������һ�������׷��ֵĸ����ã��Ǿ��ǹ���ڹ�����߽翴����ͻȻ�ϵ���

����������һ��ͨ�ù�ʽ�Ӷ��������Ǽ���x��ֵ������Դ�Ĺ�����뾶��ֻҪ�����ṩ��һ�����������ԺͶ����������
    GLfloat constant  = 1.0;
    GLfloat linear    = 0.7;
    GLfloat quadratic = 1.8;
    GLfloat lightMax  = std::fmaxf(std::fmaxf(lightColor.r, lightColor.g), lightColor.b);
    GLfloat radius    =
      (-linear +  std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * lightMax)))
      / (2 * quadratic);
���᷵��һ�������1.0��5.0��Χ�ڵİ뾶ֵ����ȡ���ڹ�����ǿ�ȡ�
���ڳ�����ÿһ����Դ�����Ƕ��������İ뾶��������Ƭ���ڹ�Դ������ڲ�ʱ�ż���ù�Դ�Ĺ��ա������Ǹ��¹��Ĺ��մ���׶�Ƭ����ɫ���������ǵ��˼�������Ĺ������ע�����ַ�������������ѧĿ�ģ���ʵ�ʳ������ǲ����еģ����ǻ��ں�����������
    struct Light {
        [...]
        float Radius;
    };
    void main()
    {
        [...]
        for(int i = 0; i < NR_LIGHTS; ++i)
        {
            // �����Դ�͸�Ƭ�μ����
            float distance = length(lights[i].Position - FragPos);
            if(distance < lights[i].Radius)
            {
                // ִ�д�������
                [...]
            }
        }
    }

����ʹ�ù����
�����Ǹ�Ƭ����ɫ����ʵ������²��������ع�����������ֻ��ʾ�����ǿ��Բ�֪������ʹ�ù�������ٹ������㡣Ȼ����ʵ�ϣ����GPU��GLSL�����ó��Ż�ѭ���ͷ�֧��
��һȱ�ݵ�ԭ����GPU����ɫ���������Ǹ߶Ȳ��еģ��󲿷ֵļܹ�Ҫ�����һ������̼߳��ϣ�GPU��Ҫ����������ȫһ������ɫ������Ӷ���ø�Ч�ʡ���ͨ����ζ��һ����ɫ������ʱ����ִ��һ��if������еķ�֧�Ӷ���֤��ɫ�����ж���һ���ģ���ʹ������֮ǰ�İ뾶����Ż���ȫ������ã�������Ȼ�ڶ����й�Դ������գ�
ʹ�ù�������õķ�������Ⱦһ��ʵ�ʵ����壬�����ݹ�����İ뾶���š���Щ������ķ����ڹ�Դ��λ�ã��������Ǹ��ݹ�����뾶���ŵģ�����������ø����˹�Ŀ����������������ǵļ��ɣ�����ʹ�ô�����ͬ���ӳ�Ƭ����ɫ������Ⱦ���塣��Ϊ�����������ȫƥ������Ӱ�����ص���ɫ�����ã�����ֻ��Ⱦ����Ӱ������ض��������������ء��������ͼչʾ����һ���ɣ�
����Ӧ���ڳ�����ÿ����Դ�ϣ��������õ�Ƭ����ӻ����һ����������֮ǰ������һ���ģ�����һ��ֻ��Ⱦ���ڹ�Դ��ص�Ƭ�Ρ�����Ч�ؼ����˴�nr_objects * nr_lights��nr_objects + nr_lights�ļ���������ʹ�ö��Դ��������Ⱦ����ޱȸ�Ч��������Ϊʲô�ӳ���Ⱦ�ǳ��ʺ���Ⱦ�ܴ�������Դ��
Ȼ�����������Ȼ��һ�����⣺���޳�(Face Culling)��Ҫ������(�������ǻ���Ⱦһ����Ч������)�������������õ�ʱ���û����ܽ���һ����Դ�Ĺ������Ȼ������֮���������Ͳ��ٱ���Ⱦ��(���ڱ����޳�)�����ʹ�ù�Դ��Ӱ����ʧ������������ͨ��һ��ģ�建�弼���������
��Ⱦ�����ȷʵ��������ص����ܸ�������Ȼ��ͨ������ͨ���ӳ���Ⱦ���죬����Ȼ������õ��Ż����������������ӳ���Ⱦ�ĸ�����(���Ҹ���Ч)����չ�����ӳٹ���(Deferred Lighting)����Ƭʽ�ӳ���ɫ��(Tile-based Deferred Shading)����Щ������ܴ�̶�����ߴ�����Դ��Ⱦ��Ч�ʣ�����Ҳ������һ����Ը�Ч�Ķ��ز��������(MSAA)��

�ӳ���Ⱦ vs ������Ⱦ
�������ӳ���ɫ��������(û�й����)�Ѿ���һ���ܴ���Ż��ˣ�ÿ�����ؽ�������һ��������Ƭ����ɫ����Ȼ������������Ⱦ������ͨ�����һ���������ж��Ƭ����ɫ������Ȼ���ӳ���Ⱦȷʵ����һЩȱ�㣺���ڴ濪����û��MSAA�ͻ��(����Ҫ������Ⱦ�����)��
������һ����С�ĳ�������û�кܶ�Ĺ�Դʱ���ӳ���Ⱦ����һ�������һ�㣬������Щʱ�����ڿ��������������ŵ㻹�������Ȼ����һ�������ӵĳ����У��ӳ���Ⱦ����ٱ��һ����Ҫ���Ż����ر������˸��Ƚ����Ż���չ��ʱ��
�������Ȼ��ָ����������������ͨ��������Ⱦ��ɵ�Ч���ܹ�ͬ�����ӳ���Ⱦ������ʵ�֣���ͨ����ҪһЩС�ķ��벽�衣�ٸ����ӣ����������Ҫ���ӳ���Ⱦ����ʹ�÷�����ͼ(Normal Mapping)��������Ҫ�ı伸����Ⱦ�׶���ɫ�������һ������ռ䷨��(World-space Normal)��
���ӷ�����ͼ����ȡ����(ʹ��һ��TBN����)�����Ǳ��淨�ߣ�������Ⱦ�׶��еĹ�������һ�㶼����Ҫ�䡣�������Ҫ���Ӳ���ͼ��������������Ҫ�ڲ���һ������������䣬���棬�ͷ�������֮ǰ�����û�������Ⱦ�׶��е��������ꡣ



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

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader shaderGeometryPass("res/Shaders/Shader_advancedLight/8/g_buffer.vert", "res/Shaders/Shader_advancedLight/8/g_buffer.frag");
    Shader shaderLightingPass("res/Shaders/Shader_advancedLight/8/deferred_shading.vert", "res/Shaders/Shader_advancedLight/8/deferred_shading.frag");
    Shader shaderLightBox("res/Shaders/Shader_advancedLight/8/deferred_light_box.vert", "res/Shaders/Shader_advancedLight/8/deferred_light_box.frag");

    // load models
    // -----------
    Model backpack("res/object/nanosuit/nanosuit.obj");
    std::vector<glm::vec3> objectPositions;
    objectPositions.push_back(glm::vec3(-3.0, -0.5, -3.0));
    objectPositions.push_back(glm::vec3(0.0, -0.5, -3.0));
    objectPositions.push_back(glm::vec3(3.0, -0.5, -3.0));
    objectPositions.push_back(glm::vec3(-3.0, -0.5, 0.0));
    objectPositions.push_back(glm::vec3(0.0, -0.5, 0.0));
    objectPositions.push_back(glm::vec3(3.0, -0.5, 0.0));
    objectPositions.push_back(glm::vec3(-3.0, -0.5, 3.0));
    objectPositions.push_back(glm::vec3(0.0, -0.5, 3.0));
    objectPositions.push_back(glm::vec3(3.0, -0.5, 3.0));


    // configure g-buffer framebuffer
    // ------------------------------
    unsigned int gBuffer;
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    unsigned int gPosition, gNormal, gAlbedoSpec;
    // position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    // normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    // color + specular color buffer
    glGenTextures(1, &gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
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

    // lighting info
    // -------------
    const unsigned int NR_LIGHTS = 32;
    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;
    srand(13);
    for (unsigned int i = 0; i < NR_LIGHTS; i++)
    {
        // calculate slightly random offsets
        float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
        float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
        float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
        lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
        // also calculate random color
        float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
        float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
        float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
        lightColors.push_back(glm::vec3(rColor, gColor, bColor));
    }

    // shader configuration
    // --------------------
    shaderLightingPass.use();
    shaderLightingPass.setInt("gPosition", 0);
    shaderLightingPass.setInt("gNormal", 1);
    shaderLightingPass.setInt("gAlbedoSpec", 2);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        auto currentFrame = static_cast<float>(glfwGetTime());
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
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        shaderGeometryPass.use();
        shaderGeometryPass.setMat4("projection", projection);
        shaderGeometryPass.setMat4("view", view);
        for (unsigned int i = 0; i < objectPositions.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, objectPositions[i]);
            model = glm::scale(model, glm::vec3(0.5f));
            shaderGeometryPass.setMat4("model", model);
            backpack.Draw(shaderGeometryPass);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 2. lighting pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
        // -----------------------------------------------------------------------------------------------------------------------
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderLightingPass.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
        // send light relevant uniforms
        for (unsigned int i = 0; i < lightPositions.size(); i++)
        {
            shaderLightingPass.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
            shaderLightingPass.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
            // update attenuation parameters and calculate radius
            const float linear = 0.7f;
            const float quadratic = 1.8f;
            shaderLightingPass.setFloat("lights[" + std::to_string(i) + "].Linear", linear);
            shaderLightingPass.setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
        }
        shaderLightingPass.setVec3("viewPos", camera.Position);
        // finally render quad
        renderQuad();

        // 2.5. copy content of geometry's depth buffer to default framebuffer's depth buffer
        // ----------------------------------------------------------------------------------
        glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
        // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
        // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
        // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
        glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 3. render lights on top of scene
        // --------------------------------
        shaderLightBox.use();
        shaderLightBox.setMat4("projection", projection);
        shaderLightBox.setMat4("view", view);
        for (unsigned int i = 0; i < lightPositions.size(); i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPositions[i]);
            model = glm::scale(model, glm::vec3(0.125f));
            shaderLightBox.setMat4("model", model);
            shaderLightBox.setVec3("lightColor", lightColors[i]);
            renderCube();
        }


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