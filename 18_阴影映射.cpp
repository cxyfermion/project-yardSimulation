#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<cmath>
#include"res/GL_heads/Shader.h"
#include"res/GL_heads/Camera.hpp"
#include "stb_image.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>

#define GLSL_VERSION "#version 330\n"

/*
��Ӱӳ��
��Ӱ�ǹ��߱��赲�Ľ������һ����Դ�Ĺ�����������������赲���ܹ��ﵽһ������ı����ʱ����ô������������Ӱ���ˡ���Ӱ�ܹ�ʹ������������ʵ�ö࣬���ҿ����ù۲��߻������֮��Ŀռ�λ�ù�ϵ���������������ȸ�����ܹ��õ���������
��Ƶ��Ϸ�н϶�ʹ�õ�һ�ּ�������Ӱ��ͼ��shadow mapping����Ч�����������������ʵ�֡���Ӱ��ͼ����������⣬����Ҳ����̫�ͣ����ҷǳ�������չ�ɸ��߼����㷨������ Omnidirectional Shadow Maps�� Cascaded Shadow Maps����

��Ӱӳ��
��Ӱӳ��(Shadow Mapping)�����˼·�ǳ��򵥣������Թ��λ��Ϊ�ӽǽ�����Ⱦ�������ܿ����Ķ�����������������������һ��������Ӱ֮���ˡ�
������һ���ذ壬�ڹ�Դ����֮����һ������ӡ����ڹ�Դ������߷���ȥ�����Կ���������ӣ����������ذ��һ���֣��ⲿ�־�Ӧ������Ӱ���ˡ�
�Դӹ�Դ�����������ϵĳ�ǧ���������б����Ǹ������������ܵľٴ룬ʵʱ��Ⱦ�ϻ�������ȡ�����ǿ��Բ�ȡ���ƾٴ룬����Ͷ���������ߡ�������ʹ�õ��Ƿǳ���Ϥ�Ķ�������Ȼ��塣
����ܼǵ�����Ȳ��Խ̳��У�����Ȼ������һ��ֵ��������ӽ��£���Ӧ��һ��Ƭ�ε�һ��0��1֮������ֵ��������Ǵӹ�Դ��͸��ͼ����Ⱦ�������������ֵ�Ľ�����浽�����л�������ͨ�����ַ�ʽ�����Ǿ��ܶԹ�Դ��͸��ͼ��������������ֵ���в��������գ����ֵ�ͻ���ʾ�ӹ�Դ��͸��ͼ�¼����ĵ�һ��Ƭ���ˡ����ǹܴ����������е�������Щ���ֵ�����������ͼ��depth map������Ӱ��ͼ��

����Ⲣû��λ�ã���Ϊ�����涨Ϊ����Զ��Ȼ����Ϊ��ʵ����Ӱ��ͼ�����ǵô�һ�����͸��ͼ��Ⱦ�����������͵��ڹ�ķ����ĳһ������Ⱦ������

��Ӱӳ��������������ɣ����ȣ�������Ⱦ�����ͼ��Ȼ������������һ����Ⱦ������ʹ�����ɵ������ͼ������Ƭ���Ƿ�����Ӱ֮�С�

�����ͼ
��һ��������Ҫ����һ�������ͼ(Depth Map)�������ͼ�Ǵӹ��͸��ͼ����Ⱦ�������������������Ӱ����Ϊ������Ҫ����������Ⱦ������浽һ�������У����ǽ��ٴ���Ҫ֡���塣
���ȣ�����ҪΪ��Ⱦ�������ͼ����һ��֡�������
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
Ȼ�󣬴���һ��2D�����ṩ��֡�������Ȼ���ʹ�ã�
    const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
���������ͼ��̫���ӡ���Ϊ����ֻ�������ֵ������Ҫ�������ʽָ��ΪGL_DEPTH_COMPONENT�����ǻ�Ҫ������ĸ߿�����Ϊ1024�����������ͼ�ķֱ��ʡ�
�����ǰ����ɵ����������Ϊ֡�������Ȼ��壺
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
������Ҫ��ֻ���ڴӹ��͸��ͼ����Ⱦ������ʱ�������Ϣ��������ɫ����û���á�Ȼ������������ɫ�����֡��������ǲ������ģ�����������Ҫ��ʽ����OpenGL���ǲ������κ���ɫ���ݽ�����Ⱦ������ͨ��������glDrawBuffer��glReadBuffer�Ѷ��ͻ��ƻ�������ΪGL_NONE��������¡�

�������ý����ֵ��Ⱦ�������֡��������ǾͿ��Կ�ʼ��һ���ˣ����������ͼ�������������������Ⱦ�׶Σ��������е���������
    // 1. ��ѡ��Ⱦ�����ͼ
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    ConfigureShaderAndMatrices();
    RenderScene();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // 2. ������һ����Ⱦ�����������ʹ�������ͼ
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ConfigureShaderAndMatrices();
    glBindTexture(GL_TEXTURE_2D, depthMap);
    RenderScene();
��δ�����ȥ��һЩϸ�ڣ������������Ӱӳ��Ļ���˼·������һ��Ҫ�ǵõ���glViewport����Ϊ��Ӱ��ͼ����������ԭ����Ⱦ�ĳ�����ͨ���Ǵ��ڷֱ��ʣ����Ų�ͬ�ķֱ��ʣ�������Ҫ�ı��ӿڣ�viewport���Ĳ�������Ӧ��Ӱ��ͼ�ĳߴ硣����������˸����ӿڲ��������������ͼҪô̫СҪô�Ͳ�������

��Դ�ռ�ı任
ǰ���Ƕδ�����һ��������ĺ�����ConfigureShaderAndMatrices�����������ڵڶ�������ȷ��Ϊÿ�����������˺��ʵ�ͶӰ����ͼ�����Լ���ص�ģ�;���Ȼ������һ�������У����Ǵӹ��λ�õ���Ұ��ʹ���˲�ͬ��ͶӰ����ͼ��������Ⱦ�ĳ�����
��Ϊ����ʹ�õ���һ�����й��߶�ƽ�еĶ���⡣�������ԭ�����ǽ�Ϊ��Դʹ������ͶӰ����͸��ͼ��û���κα��Σ�
    GLfloat near_plane = 1.0f, far_plane = 7.5f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
�����и����ڽ̵̳�demo������ʹ�õ�����ͶӰ��������ӡ���ΪͶӰ�����Ӿ�����������ķ�Χ���Լ���Щ�������ᱻ���У�����Ҫ��֤ͶӰ��׶��frustum���Ĵ�С���԰��������������ͼ�а��������塣�������Ƭ�β��������ͼ��ʱ�����ǾͲ��������Ӱ��
Ϊ�˴���һ����ͼ�������任ÿ�����壬�����Ǳ任���ӹ�Դ�ӽǿɼ��Ŀռ��У����ǽ�ʹ��glm::lookAt��������δӹ�Դ��λ�ÿ��򳡾����롣
    glm::mat4 lightView = glm::lookAt(glm::vec(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
��������Ϊ�����ṩ��һ����ռ�ı任��������ÿ������ռ�����任����Դ�����������Ǹ��ռ䣻������������Ⱦ�����ͼ����Ҫ�ġ�
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;
����lightSpaceMatrixֻҪ��shader�ṩ��ռ��ͶӰ����ͼ�������Ǿ���������������Ⱦ�����ˡ�Ȼ��������ֻ�������ֵ����������Ƭ�μ��㶼�����ǵ���ɫ���н��С�Ϊ���������ܣ����ǽ�ʹ��һ����֮��ͬ����Ϊ�򵥵���ɫ������Ⱦ�������ͼ��

��Ⱦ�������ͼ
�������Թ��͸��ͼ���г�����Ⱦ��ʱ�����ǻ���һ���Ƚϼ򵥵���ɫ���������ɫ�����˰Ѷ���任����ռ����⣬�������ø����ˡ�����򵥵���ɫ������simpleDepthShader������ʹ������������ɫ����
    #version 330 core
    layout (location = 0) in vec3 position;
    uniform mat4 lightSpaceMatrix;
    uniform mat4 model;
    void main()
    {
        gl_Position = lightSpaceMatrix * model * vec4(position, 1.0f);
    }
���������ɫ����һ������ģ�͵�һ�����㣬ʹ��lightSpaceMatrix�任����ռ��С�
��������û����ɫ���壬����Ƭ�β���Ҫ�κδ����������ǿ��Լ򵥵�ʹ��һ����Ƭ����ɫ����
    #version 330 core
    void main()
    {
        // gl_FragDepth = gl_FragCoord.z;
    }
�����Ƭ����ɫ��ʲôҲ���ɣ����������Ȼ���ᱻ���¡����ǿ���ȡ�����е�ע�ͣ�����ʽ������ȣ����������ָע�͵�����֮���Ǹ���Ч�ʵģ���Ϊ�ײ�������ζ���Ĭ��ȥ������Ȼ��塣
��Ⱦ��Ȼ������ڳ��ˣ�
    simpleDepthShader.Use();
    glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    RenderScene(simpleDepthShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
�����RenderScene�����Ĳ�����һ����ɫ������shader program����������������صĻ��ƺ�����������Ҫ�ĵط�������Ӧ��ģ�;���

����ڹ��͸��ͼ�ӽ��£�����������ÿ���ɼ�Ƭ�ε��������������Ȼ��塣ͨ�����������Ͷ�䵽һ��2D�ı����ϣ���������֡����һ�����ĺ���������ƣ�����������Ļ����ʾ���������ǻ��������Ķ�����
�������ͼ��Ⱦ���ı����ϵ�Ƭ����ɫ����
    #version 330 core
    out vec4 color;
    in vec2 TexCoords;
    uniform sampler2D depthMap;
    void main()
    {
        float depthValue = texture(depthMap, TexCoords).r;
        color = vec4(vec3(depthValue), 1.0);
    }
Ҫע����ǵ���͸��ͶӰ����ȡ������ͶӰ��������ʾ���ʱ����һЩ��΢�ĸĶ�����Ϊʹ��͸��ͶӰʱ������Ƿ����Եġ����ڽ̵̳�������ǻ�������Щ��֮ͬ����

��Ⱦ��Ӱ
��ȷ�����������ͼ�Ժ����ǾͿ��Կ�ʼ������Ӱ�ˡ���δ�����Ƭ����ɫ����ִ�У���������һ��Ƭ���Ƿ�����Ӱ֮�У����������ڶ�����ɫ���н��й�ռ�ı任��
    #version 330 core
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 normal;
    layout (location = 2) in vec2 texCoords;

    out vec2 TexCoords;

    out VS_OUT {
        vec3 FragPos;
        vec3 Normal;
        vec2 TexCoords;
        vec4 FragPosLightSpace;
    } vs_out;

    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;
    uniform mat4 lightSpaceMatrix;

    void main()
    {
        gl_Position = projection * view * model * vec4(position, 1.0f);
        vs_out.FragPos = vec3(model * vec4(position, 1.0));
        vs_out.Normal = transpose(inverse(mat3(model))) * normal;
        vs_out.TexCoords = texCoords;
        vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    }
������µĵط���FragPosLightSpace������������������ͬһ��lightSpaceMatrix��������ռ䶥��λ��ת��Ϊ��ռ䡣������ɫ������һ����ͨ�ľ��任������ռ䶥��λ��vs_out.FragPos��һ����ռ��vs_out.FragPosLightSpace��Ƭ����ɫ����
Ƭ����ɫ��ʹ��Blinn-Phong����ģ����Ⱦ���������ǽ��ż����һ��shadowֵ����fragment����Ӱ��ʱ��1.0������Ӱ����0.0��Ȼ��diffuse��specular��ɫ����������ӰԪ�ء�������Ӱ������ȫ�ڵģ�����ɢ�䣩�����ǰ�ambient�����ӳ˷����޳���
    #version 330 core
    out vec4 FragColor;

    in VS_OUT {
        vec3 FragPos;
        vec3 Normal;
        vec2 TexCoords;
        vec4 FragPosLightSpace;
    } fs_in;

    uniform sampler2D diffuseTexture;
    uniform sampler2D shadowMap;

    uniform vec3 lightPos;
    uniform vec3 viewPos;

    float ShadowCalculation(vec4 fragPosLightSpace)
    {
        [...]
    }

    void main()
    {
        vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
        vec3 normal = normalize(fs_in.Normal);
        vec3 lightColor = vec3(1.0);
        // Ambient
        vec3 ambient = 0.15 * color;
        // Diffuse
        vec3 lightDir = normalize(lightPos - fs_in.FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * lightColor;
        // Specular
        vec3 viewDir = normalize(viewPos - fs_in.FragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = 0.0;
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
        vec3 specular = spec * lightColor;
        // ������Ӱ
        float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
        vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

        FragColor = vec4(lighting, 1.0f);
    }
Ƭ����ɫ���󲿷��ǴӸ߼����ս̳��и��ƹ�����ֻ���������˸���Ӱ���㡣��������һ��shadowCalculation����������������Ӱ��Ƭ����ɫ��������������ǰ�diffuse��specular����(1-��ӰԪ��)�����ʾ���Ƭ���ж��ɷֲ�����Ӱ�С����Ƭ����ɫ������Ҫ�����������룬һ���ǹ�ռ��Ƭ��λ�ú͵�һ����Ⱦ�׶εõ��������ͼ��
����Ҫ���һ��Ƭ���Ƿ�����Ӱ�У��ѹ�ռ�Ƭ��λ��ת��Ϊ���пռ�ı�׼���豸���ꡣ�������ڶ�����ɫ�����һ�����пռ䶥��λ�õ�gl_Positionʱ��OpenGL�Զ�����һ��͸�ӳ����������пռ�����ķ�Χ-w��wתΪ-1��1����Ҫ��x��y��zԪ�س���������wԪ����ʵ�֡����ڲ��пռ��FragPosLightSpace������ͨ��gl_Position����Ƭ����ɫ������Ǳ����Լ���͸�ӳ�����
    float ShadowCalculation(vec4 fragPosLightSpace)
    {
        // ִ��͸�ӳ���
        vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
        [...]
    }
������Ƭ���ڹ�ռ��-1��1�ķ�Χ��
��ʹ������ͶӰ���󣬶���wԪ���Ա��ֲ��䣬������һ��ʵ���Ϻ������塣���ǣ���ʹ��͸��ͶӰ��ʱ����Ǳ�����ˣ�����Ϊ�˱�֤������ͶӰ�����¶���Ч�͵��������С�
��Ϊ���������ͼ�������0��1�ķ�Χ������Ҳ����ʹ��projCoords�������ͼ��ȥ�������������ǽ�NDC����任Ϊ0��1�ķ�Χ�� ������ע���������˼�ǣ������projCoords��xyz��������[-1,1]�������ָ�������Զƽ��֮��ĵ�ų���������Ϊ�˺������ͼ�������Ƚϣ�z������Ҫ�任��[0,1]��Ϊ����Ϊ�������ͼ�в��������꣬xy����Ҳ��Ҫ�任��[0,1]����������projCoords��������Ҫ�任��[0,1]��Χ����
    projCoords = projCoords * 0.5 + 0.5;
������ЩͶӰ���꣬���Ǿ��ܴ������ͼ�в����õ�0��1�Ľ�����ӵ�һ����Ⱦ�׶ε�projCoords����ֱ�Ӷ�Ӧ�ڱ任����NDC���ꡣ���ǽ��õ����λ����Ұ���������ȣ�
    float closestDepth = texture(shadowMap, projCoords.xy).r;
Ϊ�˵õ�Ƭ�εĵ�ǰ��ȣ����Ǽ򵥻�ȡͶӰ������z���꣬���������Թ��͸���ӽǵ�Ƭ�ε���ȡ�
    float currentDepth = projCoords.z;
ʵ�ʵĶԱȾ��Ǽ򵥼��currentDepth�Ƿ����closetDepth������ǣ���ôƬ�ξ�����Ӱ�С�
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
������shadowCalculation�����������ģ�
    float ShadowCalculation(vec4 fragPosLightSpace)
    {
        // ִ��͸�ӳ���
        vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
        // �任��[0,1]�ķ�Χ
        projCoords = projCoords * 0.5 + 0.5;
        // ȡ�����������(ʹ��[0,1]��Χ�µ�fragPosLight������)
        float closestDepth = texture(shadowMap, projCoords.xy).r;
        // ȡ�õ�ǰƬ���ڹ�Դ�ӽ��µ����
        float currentDepth = projCoords.z;
        // ��鵱ǰƬ���Ƿ�����Ӱ��
        float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
        return shadow;
    }

�Ľ���Ӱ��ͼ
��Ӱʧ��
ǰ���ͼƬ�������в��Եĵط����Ŵ󿴻ᷢ�����Ե�������ʽ
���ǿ��Կ����ذ��ı�����Ⱦ���ܴ�һ�齻����ߡ�������Ӱ��ͼ�Ĳ���ʵ�н�����Ӱʧ��(Shadow Acne)
��Ϊ��Ӱ��ͼ�����ڷֱ��ʣ��ھ����Դ�Ƚ�Զ������£����Ƭ�ο��ܴ������ͼ��ͬһ��ֵ��ȥ������ͼƬÿ��б�´��������ͼһ���������������ء�����Կ��������Ƭ�δ�ͬһ�����ֵ���в�����
��Ȼ�ܶ�ʱ��û���⣬���ǵ���Դ��һ���Ƕȳ�������ʱ��ͻ�����⣬��������������ͼҲ�Ǵ�һ���Ƕ��½�����Ⱦ�ġ����Ƭ�ξͻ��ͬһ��б�µ�������������в�������Щ�ڵذ����棬��Щ�ڵذ����棻�����������õ�����Ӱ�����˲��졣��Ϊ�������ЩƬ�α���Ϊ������Ӱ֮�У���Щ���ڣ��ɴ˲�����ͼƬ�е�������ʽ��
���ǿ�����һ��������Ӱƫ�ƣ�shadow bias���ļ��������������⣬���Ǽ򵥵ĶԱ������ȣ��������ͼ��Ӧ��һ��ƫ����������Ƭ�ξͲ��ᱻ�������Ϊ�ڱ���֮���ˡ�
ʹ����ƫ���������в����㶼����˱ȱ�����ȸ�С�����ֵ�����������������ȷ�ر�������û���κ���Ӱ�����ǿ�������ʵ�����ƫ�ƣ�
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
һ��0.005��ƫ�ƾ��ܰﵽ�ܴ��æ��������Щ�����¶Ⱥܴ���Ȼ�������Ӱʧ�档��һ�����ӿɿ��İ취�ܹ����ݱ��泯����ߵĽǶȸ���ƫ������ʹ�õ�ˣ�
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
����������һ��ƫ���������ֵ0.05����һ����Сֵ0.005�������ǻ��ڱ��淨�ߺ͹��շ���ġ�������ذ������ı��漸�����Դ��ֱ���õ���ƫ�ƾͺ�С��������������Ĳ������ֱ���õ���ƫ�ƾ͸���

����
ʹ����Ӱƫ�Ƶ�һ��ȱ������������ʵ�����Ӧ����ƽ�ơ�ƫ���п����㹻�������ڿ��Կ�����Ӱ���ʵ������λ�õ�ƫ��
�����Ӱʧ���������(Peter Panning)����Ϊ���忴�������������ڱ���֮�ϣ���עPeter Pan����ͯ���˵��ˣ���panning��ƽ�ơ�����֮�⣬���ұ˵����Ǹ���ɵ��к�������
���ǿ���ʹ��һ���м��ɽ���󲿷ֵ�Peter panning���⣺����Ⱦ�����ͼʱ��ʹ�������޳���front face culling����Ҳ��ǵ������޳��̳���OpenGLĬ���Ǳ����޳�������Ҫ����OpenGL����Ҫ�޳����档
��Ϊ����ֻ��Ҫ�����ͼ�����ֵ������ʵ�������������������ǵ����滹�Ǳ��涼û���⡣ʹ�ñ�����Ȳ����д�����Ϊ��Ӱ�������ڲ��д�������Ҳ��������

Ϊ���޸�peter���ƣ�����Ҫ���������޳����ȱ��뿪��GL_CULL_FACE��
    glCullFace(GL_FRONT);
    RenderSceneToDepthMap();
    glCullFace(GL_BACK); // ��Ҫ�������ԭ�ȵ�culling face
��ʮ����Ч�ؽ����peter panning�����⣬��ֻ���ڲ�������⿪�ڵ�ʵ��������Ч�����ǵĳ����У����������Ϲ����ĺܺã����ڵذ�����Ч����Ϊ�����޳���ȫ�Ƴ��˵ذ塣������һ��������ƽ�棬���ᱻ��ȫ�޳���������˴���ʹ��������ɽ��peter panning���뿼�ǵ�ֻ���޳����������������塣

��������
������ϲ��ϲ������һ���Ӿ����죬���ǹ����׶���ɼ�������һ�ɱ���Ϊ�Ǵ�����Ӱ�У���������Ĵ�����Ӱ֮�С��������״������Ϊ���������׶��ͶӰ�����1.0�������������������ͻᳬ����Ĭ�ϵ�0��1�ķ�Χ�����������Ʒ�ʽ�����ǽ���õ�����ȷ����Ƚ���������ǻ�����ʵ�����Թ�Դ�����ֵ��
�������ͼ�п�����������һ�����򣬳���������ͳ�Ϊ����Ӱ���������ʵ���ϴ����������ͼ�Ĵ�С�������ͼͶӰ���˵ذ��ϡ��������������ԭ��������֮ǰ�������ͼ�Ļ��Ʒ�ʽ���ó���GL_REPEAT��
�������������г��������ͼ���������ȷ�Χ��1.0���������������꽫��Զ������Ӱ֮�С����ǿ��Դ���һ���߿���ɫ��Ȼ��������ͼ��������ѡ������ΪGL_CLAMP_TO_BORDER��
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
����������ǲ��������ͼ0��1���귶Χ����������������ܻ᷵��һ��1.0�����ֵ����ӰֵΪ0.0��
��һ����ȹ��Զƽ�滹ҪԶʱ������ͶӰ�����z�������1.0����������£�GL_CLAMP_TO_BORDER���Ʒ�ʽ�������ã���Ϊ���ǰ������zԪ�غ������ͼ��ֵ�����˶Աȣ�������Ϊ����1.0��z����true��
����������Ҳ�ܼ򵥣�ֻҪͶӰ������z�������1.0�����ǾͰ�shadow��ֵǿ����Ϊ0.0��
    float ShadowCalculation(vec4 fragPosLightSpace)
    {
        [...]
        if(projCoords.z > 1.0)
            shadow = 0.0;
        return shadow;
    }
���Զƽ�棬���������ͼ����Ϊһ���ֹ�ָ���ı߽���ɫ�����ܽ�������ͼ��������������
��Щ�����ζ�ţ�ֻ���������ͼ��Χ���ڵı�ͶӰ��fragment���������Ӱ�������κγ�����Χ�Ķ�����û����Ӱ����������Ϸ��ͨ����ֻ������Զ�����ͻ������֮ǰ���Ǹ����Եĺ�ɫ����Ч������ʵ��

PCF
��Ӱ�����Ѿ����ŵ��������ˣ��������Բ���������Ҫ�ġ������Ŵ���Ӱ����Ӱӳ��Էֱ��ʵ������ܿ��ú����ԡ�
��Ϊ�����ͼ��һ���̶��ķֱ��ʣ����Ƭ�ζ�Ӧ��һ���������ء�������Ƕ��Ƭ�λ�������ͼ��ͬһ�����ֵ���в������⼸��Ƭ�α�õ�����ͬһ����Ӱ����ͻ������ݱߡ�
�����ͨ�����������ͼ�ķֱ��ʵķ�ʽ�����;�ݿ飬Ҳ���Գ��Ծ����ܵ��ù����׶�ӽ�������

��һ�������������ģ������������PCF��percentage-closer filtering��������һ�ֶ����ͬ���˷�ʽ����ϣ������������Ӱ��ʹ���ǳ��ָ��ٵľ�ݿ��Ӳ�ߡ�����˼���Ǵ������ͼ�ж�β�����ÿһ�β������������궼���в�ͬ��ÿ������������������Ҳ���ܲ�����Ӱ�С����еĴ���������Ž����һ�𣬽���ƽ���������Ǿ͵õ��������Ӱ��
һ���򵥵�PCF��ʵ���Ǽ򵥵Ĵ������������ܶ������ͼ������Ȼ��ѽ��ƽ��������
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
���textureSize����һ�����������������0��mipmap��vec2���͵Ŀ�͸ߡ���1����������һ�������������صĴ�С���������Զ������������ƫ�ƣ�ȷ��ÿ�������������Բ�ͬ�����ֵ���������ǲ����õ�9��ֵ��������ͶӰ�����x��yֵ����Χ��Ϊ��Ӱ�赲���в��ԣ�������ͨ������������Ŀ�����ƽ������

���� vs ͶӰ
����Ⱦ�����ͼ��ʱ������(Orthographic)��ͶӰ(Projection)����֮��������ͬ������ͶӰ���󲢲��Ὣ������͸��ͼ���б��Σ���������/���߶���ƽ�еģ���ʹ�����ڶ������˵�Ǹ��ܺõ�ͶӰ����Ȼ��͸��ͶӰ���󣬻Ὣ���ж������͸�ӹ�ϵ���б��Σ������˶���ͬ��
͸��ͶӰ���ڹ�Դ��˵������������⣬�������Լ���λ�õġ�͸��ͶӰ��˸��������ڵ��Դ�;۹���ϣ�������ͶӰ�������ڶ�����ϡ�
��һ��ϸ΢����ǣ�͸��ͶӰ���󣬽���Ȼ����Ӿ���������õ�һ������ȫ�׵Ľ���������������Ϊ͸��ͶӰ�£���ȱ���˷����Ե����ֵ�����Ĵ�����ɱ淶Χ��λ�ڽ�ƽ�渽����Ϊ�˿�����ʹ������ͶӰһ�����ʵع۲����ֵ��������Ƚ����������ֵת��Ϊ���Եģ�����������Ȳ��Խ̳����Ѿ����۹���������
    #version 330 core
    out vec4 color;
    in vec2 TexCoords;
    uniform sampler2D depthMap;
    uniform float near_plane;
    uniform float far_plane;
    float LinearizeDepth(float depth)
    {
        float z = depth * 2.0 - 1.0; // Back to NDC
        return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
    }
    void main()
    {
        float depthValue = texture(depthMap, TexCoords).r;
        color = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
        // color = vec4(vec3(depthValue), 1.0); // orthographic
    }

Enable/Disable shadows by pressing 'SPACE'

*/

// Properties
const GLuint SCR_WIDTH = 800, SCR_HEIGHT = 600;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
GLuint loadTexture(const GLchar* path);
void RenderScene(Shader& shader);
void renderScene(const Shader& shader);
void RenderCube();
void RenderQuad();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// Delta
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Options
GLboolean shadows = true;

// Global variables
GLuint woodTexture;
GLuint planeVAO;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //// Initialize GLEW to setup the OpenGL Function pointers
    //glewExperimental = GL_TRUE;
    //glewInit();

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("res/Shaders/Shader_advancedLight/2/shadow_mapping.vert", "res/Shaders/Shader_advancedLight/2/shadow_mapping.frag");
    Shader simpleDepthShader("res/Shaders/Shader_advancedLight/2/shadow_mapping_depth.vert", "res/Shaders/Shader_advancedLight/2/shadow_mapping_depth.frag");
    Shader debugDepthQuad("res/Shaders/Shader_advancedLight/2/debug_quad.vert", "res/Shaders/Shader_advancedLight/2/debug_quad.frag");

    // Set texture samples
    shader.use();
    glUniform1i(glGetUniformLocation(shader.ID, "diffuseTexture"), 0);
    glUniform1i(glGetUniformLocation(shader.ID, "shadowMap"), 1);
    debugDepthQuad.use();
    debugDepthQuad.setInt("depthMap", 0);

    GLfloat planeVertices[] = {
        // Positions          // Normals         // Texture Coords
        25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
        -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
        -25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

        25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
        25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f,
        -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f
    };
    // Setup plane VAO
    GLuint planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glBindVertexArray(0);

    // Light source
    glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

    // Load textures
    woodTexture = loadTexture("res/textures/wood.jpg");

    // Configure depth map FBO
    const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // - Create depth texture
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //������Ӱ����
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Check and call events
        Do_Movement();
        
        // Change light position over time
        lightPos.x = sin(glfwGetTime()) * 3.0f;
        lightPos.z = cos(glfwGetTime()) * 2.0f;
        lightPos.y = 5.0 + cos(glfwGetTime()) * 1.0f;

        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 7.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        //lightProjection = glm::perspective(45.0f, (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // Note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene.
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        simpleDepthShader.use();
        simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodTexture);
        renderScene(simpleDepthShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // 2. render scene as normal using the generated depth/shadow map  
        shader.use();
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        // set light uniforms
        shader.setVec3("viewPos", camera.Position);
        shader.setVec3("lightPos", lightPos);
        shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glUniform1i(glGetUniformLocation(shader.ID, "shadows"), shadows);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        renderScene(shader);
        // 3. DEBUG: visualize depth map by rendering it to plane
        // render Depth map to quad for visual debugging
        debugDepthQuad.use();
        debugDepthQuad.setFloat("near_plane", near_plane);
        debugDepthQuad.setFloat("far_plane", far_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        //renderQuad();


        // Swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// renders the 3D scene
// --------------------
void renderScene(const Shader& shader)
{
    // floor
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // cubes
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    RenderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    RenderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.25));
    shader.setMat4("model", model);
    RenderCube();
}



void RenderScene(Shader& shader)
{
    // Floor
    glm::mat4 model;
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    // Cubes
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    RenderCube();
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    RenderCube();
    model = glm::mat4();
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
    model = glm::rotate(model, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.5));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    RenderCube();
}


// RenderQuad() Renders a 1x1 quad in NDC, best used for framebuffer color targets
// and post-processing effects.
GLuint quadVAO = 0;
GLuint quadVBO;
void RenderQuad()
{
    if (quadVAO == 0)
    {
        GLfloat quadVertices[] = {
            // Positions        // Texture Coords
            -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
             1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
             1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        };
        // Setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}


// RenderCube() Renders a 1x1 3D cube in NDC.
GLuint cubeVAO = 0;
GLuint cubeVBO = 0;
void RenderCube()
{
    // Initialize (if necessary)
    if (cubeVAO == 0)
    {
        GLfloat vertices[] = {
            // Back face
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
            // Front face
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
            // Left face
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
            // Right face
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
            // Bottom face
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            // Top face
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // Fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // Link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // Render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// This function loads a texture from file. Note: texture loading functions like these are usually 
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio). 
// For learning purposes we'll just define it as a utility function.
GLuint loadTexture(const GLchar* path)
{
    // Generate texture ID and load texture data 
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width, height, nrComponents;
    unsigned char* image = stbi_load(path, &width, &height, &nrComponents, 0);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);
    return textureID;

}

bool keys[1024];
bool keysPressed[1024];
// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if (keys[GLFW_KEY_W])
        camera.ProcessKayboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.ProcessKayboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.ProcessKayboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.ProcessKayboard(RIGHT, deltaTime);

    if (keys[GLFW_KEY_SPACE] && !keysPressed[GLFW_KEY_SPACE])
    {
        shadows = !shadows;
        keysPressed[GLFW_KEY_SPACE] = true;
    }
}

GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key <= 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
            keysPressed[key] = false;
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}