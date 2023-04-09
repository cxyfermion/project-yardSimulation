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
#include <vector>
//#include"Model.hpp"
#include <random>

/*
���淴�� IBL
�ڱ��̳��У����ǽ��ص��ע���䷽�̵ľ��沿�֣�
���ע�⵽ Cook-Torrance ���沿�֣����ԣ������������ϲ��ǳ���������������ⷽ��Ӱ�죬�����ӽ�Ӱ�졣�����ͼ������������ⷽ������п��ܵ��ӽǷ���Ļ��֣�����������Ἣ���Ӵ�ʵʱ����̫����Epic Games �����һ���������������Ԥ���㾵�沿�ֵľ����Ϊʵʱ��������һЩ��Э�����ַ�������Ϊ�ָ���ͽ��Ʒ���split sum approximation����
�ָ���ͽ��ƽ����̵ľ��沿�ַָ�����������Ĳ��֣����ǿ��Ե���������Ȼ���� PBR ��ɫ������ͣ������ڼ�Ӿ��淴�䲿�� IBL���ָ���ͽ��Ʒ�����������֮ǰ�����ͼԤ����ķ�������Ҫ HDR ������ͼ��Ϊ�������롣
��������նȾ����ͬ�ģ����ܣ�ԭ�������޷��Ժ��������ʵʱ�����ֵľ��淴�䲿�֡���ˣ��������Ԥ����������֣��Եõ����� IBL ��ͼ�����Ķ�������Ƭ�εķ��߶�����ͼ���������㡣���ǣ���һ���ط��е㼬�֣������ܹ�Ԥ������ն�ͼ������Ϊ����ֽ������ڣ����ҿ��Խ������䷴���ʳ������Ƴ����֣�����һ�Σ����ֲ�����ȡ���ڣ��� BRDF ���Կ�����
��λ��ֻ������������޷�������������������Ԥ�����������ͼ����ǰһ���̳���������λ����˴��޹ء���ʵʱ״̬�£���ÿ�ֿ��ܵĺ͵����Ԥ����û����ǲ����еġ� Epic Games �ķָ���ͽ��Ʒ���Ԥ����ֳ����������Ĳ�����⣬�ٽ���������������õ����ĸ�����Ԥ���������ָ���ͽ��Ʒ������淴����ֲ�����������Ļ��֣�
����ĵ�һ���ֱ���ΪԤ�˲�������ͼ���������ڷ��ն�ͼ����Ԥ�ȼ���Ļ��������ͼ������ο����˴ֲڶȡ���Ϊ���Ŵֲڶȵ����ӣ����뻷����ͼ����Ĳ������������ɢ�����·����ģ�������Զ��ھ����ÿ���ֲڶȼ������ǽ���˳���ģ����Ľ���洢��Ԥ�˲���ͼ�� mipmap �С����磬Ԥ���˵Ļ�����ͼ���� 5 �� mipmap �����д洢 5 ����ͬ�ֲڶ�ֵ��Ԥ������
����ʹ�� Cook-Torrance BRDF �ķ��߷ֲ�����(NDF)���ɲ�����������ɢ��ǿ�ȣ��ú��������ߺ��ӽǷ�����Ϊ���롣���������ھ��������ͼʱ���Ȳ�֪���ӽǷ������ Epic Games �����ӽǷ��򡪡�Ҳ���Ǿ��淴�䷽�򡪡����ǵ��������������������һ�����ơ�����ɴ������£�
    vec3 N = normalize(w_o);
    vec3 R = N;
    vec3 V = R;
������Ԥ���˵Ļ�������Ͳ���Ҫ�����ӽǷ����ˡ�����ζ�ŵ�������ͼ�ĽǶȹ۲����ľ��淴��ʱ���õ����ӽǾ��淴��Ч�����Ǻܺã�ͼƬ�������¡�Moving Frostbite to PBR������Ȼ����ͨ��������Ϊ����һ���������Э��
��ʽ�ĵڶ����ֵ��ھ��淴����ֵ� BRDF ���֡�������Ǽ���ÿ��������������ȶ��ǰ�ɫ�ģ���� �����Ϳ����ڸ����ֲڶȡ����� ���� �н� ������£�Ԥ���� BRDF ����Ӧ�����Epic Games ��Ԥ����õ� BRDF ��ÿ���ֲڶȺ�����ǵ���ϵ���Ӧ����洢��һ�� 2D ��������(LUT)�ϣ���ΪBRDF������ͼ��2D ��������洢�Ƿ�������Ӧ��ϵ����R ͨ������ƫ��ֵ��G ͨ��������Ϊ�����ṩ�˷ָ�澵�淴����ֵĵڶ�������
���ɲ��������ʱ�������� BRDF �����루��Χ�� 0.0 �� 1.0 ֮�䣩��Ϊ�����꣬�Դֲڶ���Ϊ�����ꡣ���˴� BRDF ������ͼ��Ԥ���˵Ļ�����ͼ�����ǾͿ��Խ����߽���������Ի�þ��淴����ֵĽ����
    float lod             = getMipLevelFromRoughness(roughness);
    vec3 prefilteredColor = textureCubeLod(PrefilteredEnvMap, refVec, lod);
    vec2 envBRDF          = texture2D(BRDFIntegrationMap, vec2(NdotV, roughness)).xy;
    vec3 indirectSpecular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

Ԥ�˲�HDR������ͼ
Ԥ�˲�������ͼ�ķ��������ǶԷ������ͼ�����ķ����ǳ����ơ����ھ����ÿ���ֲڶȼ������ǽ���˳���ģ����Ľ���洢��Ԥ�˲���ͼ�� mipmap �С� ���ȣ�������Ҫ����һ���µ���������ͼ������Ԥ���˵Ļ�����ͼ���ݡ�Ϊ��ȷ��Ϊ�� mip ��������㹻���ڴ棬һ���򵥷����ǵ��� glGenerateMipmap��
    unsigned int prefilterMap;
    glGenTextures(1, &prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
ע�⣬��Ϊ���Ǽƻ����� prefilterMap �� mipmap��������Ҫȷ��������С����������Ϊ GL_LINEAR_MIPMAP_LINEAR �����������Թ��ˡ����洢����Ԥ�˲��ľ��淴�䣬���� mip ����ķֱ�����ÿ�� 128��128�����ڴ����������˵�����Ѿ��㹻�ˣ�������������д����⻬���ϣ����������ϵķ��䣩��������Ҫ��߷ֱ��ʡ�
����һ�ڽ̳��У�����ʹ�������������ɾ��ȷֲ��ڰ��� �ϵĲ����������ԶԻ�����ͼ���о������Ȼ��������ǳ������ڷ��նȣ������ھ��淴��Ч���ϲ���淴�������ڱ���Ĵֲڶȣ�������߿��ܱȽ���ɢ��Ҳ���ܱȽϽ��ܣ�����һ����Χ���ŷ������������Ǳ��漫�ȴֲڣ�
���п��ܳ���ķ���⹹�ɵ���״��Ϊ���沨�ꡣ���Ŵֲڶȵ����ӣ����沨��Ĵ�С���ӣ���������ⷽ��ͬ����״�ᷢ���仯����ˣ����沨�����״�߶������ڲ��ʡ� ��΢����ģ�����������ⷽ�����沨��ָ��΢ƽ��İ������ķ��䷽�򡣿��ǵ�������������ջᷴ�䵽һ�����ڰ������ľ��沨���ڣ�����ʱ�����Ƶķ�ʽѡȡ����������������ģ���Ϊ�󲿷���������������˷ѵ��ˣ�������̳�Ϊ��Ҫ�Բ�����

���ؿ�����ֺ���Ҫ�Բ���
Ϊ�˳�������Ҫ�Բ�������������Ҫ�˽�һ����ѧ�ṹ����Ϊ���ؿ�����֡����ؿ��������Ҫ��ͳ�ƺ͸������۵���ϡ����ؿ�����԰���������ɢ�ؽ���˿�ͳ�����⣬�����ؿ��������ˡ�
���磬��������Ҫ����һ���������й����ƽ����ߡ�Ϊ�˵õ����������Բ���ÿ�����񲢶����ǵ������ƽ����������õ�����Ҫ��ȷ�д𰸡����ǣ����ڴ���������˺�ãã�������������ʵ����Ҫ����̫�ྫ����ʱ�䡣
��һ�ַ�����ѡ��һ��С�ö����ȫ�������ƫ�����˿��Ӽ����������ǵ���߲��Խ����ƽ��������ֻ���� 100 �ˣ���Ȼ�𰸲��Ǿ��Ծ�ȷ������õ�һ����Խӽ�����Ĵ𰸣�������۱������������ɡ����ǵ��뷨�ǣ���������˿��в���һ���С��������������ģ��������Խӽ���ʵ�𰸣������������� �����Ӷ����ӽӽ���
���ؿ��޻��ֽ����ڴ������ɵĻ����ϣ���������ͬ�ķ����������֡���Ϊ���п��ܵģ������������޵ģ�����ֵ �����֣����Ǽ򵥵ش������������ѡ���� ���ɲ���ֵ����ƽ�������� �����ӣ����ǵĽ����Խ��Խ�ӽ����ֵľ�ȷ�����
Ϊ�����������֣������� �� �ϲ��� ����������������Ǽ���һ�𲢳�������������ȡƽ���� ��������ܶȺ��� (probability density function)�����ĺ������ض������������������Ϸ����ĸ��ʡ�
���漰���ؿ������ʱ��ĳЩ�������ܱ������������и��ߵ����ɸ��ʡ������Ϊʲô�����κ�һ������ؿ�����ƣ����Ƕ������ pdf ������ֵ���Ի���Բ������ʡ���ĿǰΪֹ������ÿ����Ҫ������ֵ�ʱ�����ɵ��������Ǿ��ȷֲ��ģ�������ȫ��ȡ���ĿǰΪֹ�����ǵĹ�������ƫ�ģ�����ζ���������������Ĳ������ӣ��������ս����������ֵľ�ȷ�⡣
���ǣ�ĳЩ���ؿ����������ƫ�ģ�����ζ�����ɵ�������������ȫ����ģ����Ǽ������ض���ֵ������Щ��ƫ�����ؿ��������и���������ٶȣ����ǻ��Ը�����ٶ���������ȷ�⣬������������ƫ�ԣ�������Զ������������ȷ�⡣
ͨ����˵������һ�����Խ��ܵ����Է������������ڼ����ͼ��ѧ�С���ΪֻҪ������Ӿ��Ͽ��Խ��ܣ���������ľ�ȷ�ԾͲ�̫��Ҫ���������ǽ����ᵽһ�֣���ƫ�ģ���Ҫ�Բ����������ɵ�����ƫ���ض��ķ�������������£����ǻὫÿ���������Ի������Ӧ�� pdf ����͡�
���ؿ�������ڼ����ͼ��ѧ�зǳ��ձ飬��Ϊ����һ���Ը�Ч����ɢ��ʽ�������Ļ�������ƶ��ҷǳ�ֱ�۵ķ��������κ����/������в�������������� �����ڸ����/������������� �����������Ȩ��ÿ�����������ս���Ĺ��ײ���͡�
Ĭ������£�ÿ�β�������������Ϥ����ȫ��α��������������ð�������е�ĳЩ���ԣ����ǿ���������Ȼ���������������һЩ��Ȥ���ʵ��������������磬���ǿ��Զ�һ����Ϊ�Ͳ������еĶ����������ؿ�����֣����������ɵ���Ȼ������������������ֲ�������

��ʹ�õͲ��������������ؿ�����������ʱ���ù��̳�Ϊ�����ؿ�����֡������ؿ��巽�����и���������ٶȣ���ʹ�����������ܷ��ص�Ӧ�ú����á�
���������»�õ��й����ؿ��壨Monte Carlo���������ؿ��壨Quasi-Monte Carlo�����ֵ�֪ʶ�����ǿ���ʹ��һ����Ȥ����������ø���������ٶȣ��������Ҫ�Բ�����������ǰ���Ѿ��ᵽ�����������ھ��淴�������£�����Ĺ������������ھ��沨���У�����Ĵ�Сȡ���ڱ���Ĵֲڶȡ���Ȼ���沨������κΣ��⣩������ɵ������뾵������޹أ���˽����������ھ��沨����������������ģ������������ؿ����������ƫ�
��������˵���������Ҫ�Բ����ĺ��ģ�ֻ��ĳЩ�������ɲ���������������Χ��΢������������ֲܴڶ����ơ�ͨ���������ؿ��������Ͳ����������ϣ���ʹ����Ҫ�Բ���ƫ�����������ķ��������ǿ��Ի�úܸߵ������ٶȡ���Ϊ���������ٶȸ��죬����Ҫ�ﵽ�㹻�Ľ��ƶȣ���������Ҫ���������١���ˣ�������Ϸ���������������ͼ��Ӧ�ó���ʵʱ��⾵����֣���Ȼ��Ԥ����������Ҫ���öࡣ

�Ͳ�������
�ڱ��̳��У����ǽ�ʹ����Ҫ�Բ�����Ԥ�����ӷ��䷽�̵ľ��淴�䲿�֣��ò������������ؿ��巽������������ĵͲ������С����ǽ�ʹ�õ����б���Ϊ Hammersley ���У�Holger Dammertz ����ϸ����������Hammersley �����ǻ��� Van Der Corput ���У��������ǰ�ʮ�������ֵĶ����Ʊ�ʾ����ת��С�����ұ߶��á�
����һЩ����ļ��ɣ����ǿ�������ɫ�������зǳ���Ч������ Van Der Corput ���У����ǽ���������� Hammersley ���У�����������Ϊ N����������Ϊ i��
    float RadicalInverse_VdC(uint bits)
    {
        bits = (bits << 16u) | (bits >> 16u);
        bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
        bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
        bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
        bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
        return float(bits) * 2.3283064365386963e-10; // / 0x100000000
    }
    // ----------------------------------------------------------------------------
    vec2 Hammersley(uint i, uint N)
    {
        return vec2(float(i)/float(N), RadicalInverse_VdC(i));
    }
GLSL �� Hammersley �������Ի�ȡ��СΪ N ���������еĵͲ������� i��

����λ����� Hammersley ����
�������� OpenGL �����������֧��λ�����������WebGL��OpenGL ES 2.0��������������£��������Ҫ������λ�����������汾 Van Der Corput ���У�
    float VanDerCorpus(uint n, uint base)
    {
        float invBase = 1.0 / float(base);
        float denom   = 1.0;
        float result  = 0.0;

        for(uint i = 0u; i < 32u; ++i)
        {
            if(n > 0u)
            {
                denom   = mod(float(n), 2.0);
                result += denom * invBase;
                invBase = invBase / 2.0;
                n       = uint(float(n) / 2.0);
            }
        }

        return result;
    }
    // ----------------------------------------------------------------------------
    vec2 HammersleyNoBitOps(uint i, uint N)
    {
        return vec2(float(i)/float(N), VanDerCorpus(i, 2u));
    }

GGX ��Ҫ�Բ���
�б��ھ��Ȼ�����أ��������ؿ��壩�ڻ��ְ��� �����������������ǵĲ�������ݴֲڶȣ�ƫ��΢����İ������ĺ�۷��䷽�򡣲������̽�������֮ǰ�����Ĺ������ƣ���ʼһ����ѭ��������һ��������Ͳ��죩����ֵ���ø�����ֵ�����߿ռ����������������������������任������ռ䲢�Գ����ķ���Ȳ�������֮ͬ�����ڣ���������ʹ�õͲ�������ֵ��Ϊ���������ɲ���������
    const uint SAMPLE_COUNT = 4096u;
    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        vec2 Xi = Hammersley(i, SAMPLE_COUNT);
    }
���⣬Ҫ��������������������ҪһЩ���������ƫ�Ʋ�����������ʹ�䳯���ض��ֲڶȵľ��沨�귽�����ǿ��������۽̳�������ʹ�� NDF������ GGX NDF ��ϵ� Epic Games ���������β��������Ĵ����У�
    vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
    {
        float a = roughness*roughness;

        float phi = 2.0 * PI * Xi.x;
        float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
        float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

        // from spherical coordinates to cartesian coordinates
        vec3 H;
        H.x = cos(phi) * sinTheta;
        H.y = sin(phi) * sinTheta;
        H.z = cosTheta;

        // from tangent-space vector to world-space sample vector
        vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
        vec3 tangent   = normalize(cross(up, N));
        vec3 bitangent = cross(N, tangent);

        vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
        return normalize(sampleVec);
    }
�����ض��Ĵֲڶ�����͵Ͳ�������ֵ Xi�����ǻ����һ����������������������Χ����Ԥ����΢����İ�������ע�⣬���ݵ�ʿ��� PBR ���о���Epic Games ʹ����ƽ���ֲڶ��Ի�ø��õ��Ӿ�Ч����
ʹ�õͲ��� Hammersley ���к�����������������ɷ��������ǿ����������Ԥ�˲��������ɫ����
    #version 330 core
    out vec4 FragColor;
    in vec3 localPos;

    uniform samplerCube environmentMap;
    uniform float roughness;

    const float PI = 3.14159265359;

    float RadicalInverse_VdC(uint bits);
    vec2 Hammersley(uint i, uint N);
    vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness);

    void main()
    {
        vec3 N = normalize(localPos);
        vec3 R = N;
        vec3 V = R;

        const uint SAMPLE_COUNT = 1024u;
        float totalWeight = 0.0;
        vec3 prefilteredColor = vec3(0.0);
        for(uint i = 0u; i < SAMPLE_COUNT; ++i)
        {
            vec2 Xi = Hammersley(i, SAMPLE_COUNT);
            vec3 H  = ImportanceSampleGGX(Xi, N, roughness);
            vec3 L  = normalize(2.0 * dot(V, H) * H - V);

            float NdotL = max(dot(N, L), 0.0);
            if(NdotL > 0.0)
            {
                prefilteredColor += texture(environmentMap, L).rgb * NdotL;
                totalWeight      += NdotL;
            }
        }
        prefilteredColor = prefilteredColor / totalWeight;

        FragColor = vec4(prefilteredColor, 1.0);
    }
����Ĵֲڶ�����Ԥ���˵���������ͼ�� mipmap ����仯����0.0��1.0�������Ǹ��ݾݴֲڶ�Ԥ���˻�����ͼ���ѽ������ prefilteredColor ����� prefilteredColor ���Բ���Ȩ���ܺͣ����ж����ս��Ӱ���С��NdotL ��С���Ĳ�������Ȩ��Ҳ��С��

����Ԥ���� mipmap ����
ʣ��Ҫ���ľ����� OpenGL �ڶ�� mipmap �������Բ�ͬ�Ĵֲڶ�ֵԤ���˻�����ͼ�������ʼ�ķ��նȽ̳���Ϊ������ʵ���Ϻܼ򵥣�
    prefilterShader.use();
    prefilterShader.setInt("environmentMap", 0);
    prefilterShader.setMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        unsigned int mipWidth  = 128 * std::pow(0.5, mip);
        unsigned int mipHeight = 128 * std::pow(0.5, mip);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        prefilterShader.setFloat("roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
            prefilterShader.setMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                   GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderCube();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
������������ڷ��ն���ͼ���������������ǽ�֡���������ŵ��ʵ��� mipmap �ߴ磬 mip ����ÿ����һ�����ߴ���СΪһ�롣���⣬������ glFramebufferTexture2D �����һ��������ָ��Ҫ��Ⱦ��Ŀ�� mip ����Ȼ��ҪԤ���˵Ĵֲڶȴ���Ԥ������ɫ����
�������ǻ�õ�һ�ž����ʵ�Ԥ���˵Ļ�����ͼ�����ʸ���ͼʱָ���� mip �ȼ�Խ�ߣ���õķ����Խģ���������������պ���ɫ������ʾ����Ԥ���˵Ļ�����������ͼ����������ɫ����ǿ�������һ�� mip �������ϲ�����������ʾ��
    vec3 envColor = textureLod(environmentMap, WorldPos, 1.2).rgb;

Ԥ���˾����α��
��ǰ��Ԥ������ͼ�����ڴ���������������������������������������Ԥ���˾��ֱ����ص���Ⱦ���⡣�ҽ��������г������һЩ���⣬�Լ�����޸����ǡ�

�ߴֲڶȵ���������ͼ�ӷ�
�ھ��дֲڱ���ı����϶�Ԥ������ͼ������Ҳ�͵�ͬ���ڽϵ͵� mip �����϶�Ԥ������ͼ�������ڶ���������ͼ���в���ʱ��Ĭ������£�OpenGL��������������֮��������Բ�ֵ�����ڽϵ͵� mip ������и��͵ķֱ��ʣ�����Ԥ������ͼ�����������Ĳ��������������ȱ��������������֮����˲�������͸����ԣ�
���˵��ǣ�OpenGL �������� GL_TEXTURE_CUBE_MAP_SEAMLESS����Ϊ�����ṩ����������ͼ����֮�������ȷ���˵�ѡ�
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

Ԥ���˾��������
���ھ��淴���й�ǿ�ȵı仯�󣬸�Ƶϸ�ڶ࣬���ԶԾ��淴����о����Ҫ����������������ȷ��ӳ HDR ��������Ļ��ұ仯�������Ѿ������˴����Ĳ�����������ĳЩ�����£���ĳЩ�ϴֲڵ� mip �����Ͽ�����Ȼ��������������������Χ���ֵ�״ͼ��
һ�ֽ�������ǽ�һ��������������������ĳЩ����»��ǲ�������һ�ַ����� Chetan Jags ���������ǿ�����Ԥ���˾��ʱ����ֱ�Ӳ���������ͼ�����ǻ��ڻ��ֵ� PDF �ʹֲڶȲ���������ͼ�� mipmap ���Լ���α��
    float D   = DistributionGGX(NdotH, roughness);
    float pdf = (D * NdotH / (4.0 * HdotV)) + 0.0001;

    float resolution = 512.0; // resolution of source cubemap (per face)
    float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
    float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

    float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);
��ȻҪ���� mipmap ����Ҫ�����ڻ�����ͼ�Ͽ��������Թ��ˣ�
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
������������ͼ�Ļ���������� OpenGL ���� mipmap��
    // convert HDR equirectangular environment map to cubemap equivalent
    [...]
    // then generate mipmaps
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
�������Ч���ǳ��ã�����ȥ��Ԥ������ͼ�нϴֲڱ����ϵĴ��������ȫ�����㡣

Ԥ���� BRDF
BRDF �����ɫ���� 2D ƽ����ִ�м��㣬ֱ��ʹ���� 2D ����������Ϊ������루NdotV �� roughness����������Ԥ�˲����ľ������������ƣ���֮ͬ�����ڣ������ڸ��� BRDF �ļ��κ����� Fresnel-Schlick �������������������
    vec2 IntegrateBRDF(float NdotV, float roughness)
    {
        vec3 V;
        V.x = sqrt(1.0 - NdotV*NdotV);
        V.y = 0.0;
        V.z = NdotV;

        float A = 0.0;
        float B = 0.0;

        vec3 N = vec3(0.0, 0.0, 1.0);

        const uint SAMPLE_COUNT = 1024u;
        for(uint i = 0u; i < SAMPLE_COUNT; ++i)
        {
            vec2 Xi = Hammersley(i, SAMPLE_COUNT);
            vec3 H  = ImportanceSampleGGX(Xi, N, roughness);
            vec3 L  = normalize(2.0 * dot(V, H) * H - V);

            float NdotL = max(L.z, 0.0);
            float NdotH = max(H.z, 0.0);
            float VdotH = max(dot(V, H), 0.0);

            if(NdotL > 0.0)
            {
                float G = GeometrySmith(N, V, L, roughness);
                float G_Vis = (G * VdotH) / (NdotH * NdotV);
                float Fc = pow(1.0 - VdotH, 5.0);

                A += (1.0 - Fc) * G_Vis;
                B += Fc * G_Vis;
            }
        }
        A /= float(SAMPLE_COUNT);
        B /= float(SAMPLE_COUNT);
        return vec2(A, B);
    }
    // ----------------------------------------------------------------------------
    void main()
    {
        vec2 integratedBRDF = IntegrateBRDF(TexCoords.x, TexCoords.y);
        FragColor = integratedBRDF;
    }
����������BRDF ��������Ǵ���ѧ�������ֱ��ת�������ǽ��Ƕ� �ʹֲڶ���Ϊ���룬����Ҫ�Բ������������������������������Ͻ�� BRDF �ķ���������������д���Ȼ�����ÿ�������� ��ϵ����ƫ����ȡƽ��ֵ��
����ܻ��������۽̳��е�һ��ϸ�ڣ��� IBL һ��ʹ��ʱ��BRDF �ļ��������в�ͬ����Ϊ �����ĺ������в�ͬ��
���� BRDF ����Ǿ��� IBL ���ֵ�һ���֣��������Ҫ�� Schlick-GGX ���κ�����ʹ�� ��
    float GeometrySchlickGGX(float NdotV, float roughness)
    {
        float a = roughness;
        float k = (a * a) / 2.0;

        float nom   = NdotV;
        float denom = NdotV * (1.0 - k) + k;

        return nom / denom;
    }
    // ----------------------------------------------------------------------------
    float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
    {
        float NdotV = max(dot(N, V), 0.0);
        float NdotL = max(dot(N, L), 0.0);
        float ggx2 = GeometrySchlickGGX(NdotV, roughness);
        float ggx1 = GeometrySchlickGGX(NdotL, roughness);

        return ggx1 * ggx2;
    }
��ע�⣬��Ȼ ���Ǵ� a ��������ģ�������� a ���� roughness ��ƽ��������ͬ����� a ���������������������������Ǽ�װƽ�����ˡ��Ҳ�ȷ�����������Ƿ��� Epic Games ���ʿ��ԭʼ���Ĳ�һ�£�����ֱ�ӽ� roughness ���� a �õ��� BRDF ������ͼ�� Epic Games �İ汾��ȫһ�¡�
���Ϊ�˴洢 BRDF ��������������Ҫ����һ�� 512 �� 512 �ֱ��ʵ� 2D ����
    unsigned int brdfLUTTexture;
    glGenTextures(1, &brdfLUTTexture);

    // pre-allocate enough memory for the LUT texture.
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
��ע�⣬����ʹ�õ��� Epic Games �Ƽ���16λ���ȸ����ʽ��������ģʽ����Ϊ GL_CLAMP_TO_EDGE �Է�ֹ��Ե������α�� Ȼ�����Ǹ���ͬһ��֡���������󣬲��� NDC (��ע��Normalized Device Coordinates) ��Ļ�ռ��ı��������д���ɫ����
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

    glViewport(0, 0, 512, 512);
    brdfShader.use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    RenderQuad();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

��� IBL ����
Ϊ��ʹ���䷽�̵ļ�Ӿ��淴�䲿����ȷ���У�������Ҫ���ָ���ͽ��Ʒ����������ַ����һ�𡣵�һ���ǽ�Ԥ����Ĺ������������� PBR ��ɫ���������棺
    uniform samplerCube prefilterMap;
    uniform sampler2D   brdfLUT;
���ȣ�ʹ�÷�����������Ԥ���˵Ļ�����ͼ����ȡ����ļ�Ӿ��淴�䡣��ע�⣬���ǻ���ݱ���ֲڶ��ں��ʵ� mip �����������ʹ���ֲڵı��������ģ���ľ��淴�䡣
    void main()
    {
        [...]
        vec3 R = reflect(-V, N);

        const float MAX_REFLECTION_LOD = 4.0;
        vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;
        [...]
    }
��Ԥ���˲����У����ǽ���������ͼ������ 5 �� mip ����0��4�����˴���Ϊ MAX_REFLECTION_LOD����ȷ�������һ��û�����ݵ� mip ��������� Ȼ����������֪�Ĳ��ʴֲڶȺ�����-���߼н���Ϊ���룬���� BRDF LUT��
    vec3 F        = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    vec2 envBRDF  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);
�������Ǿʹ� BRDF LUT �л���� ��ϵ����ƫ�ƣ��������Ǿ�ֱ���ü�ӹ�������� F ���档���������� IBL ���䷽����ߵ�Ԥ���˲��ֽ�����������ؽ��������ƻ��֣�����specular��
������ PBR IBL �����
    vec3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse    = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;
    vec2 envBRDF  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

    vec3 ambient = (kD * diffuse + specular) * ao;
��ע�⣬specular û�г��� kS����Ϊ�Ѿ��˹��˷�����ϵ���� ���ڣ���һϵ�дֲڶȺͽ����ȸ�����������д˴��룬�������ڿ��������յ� PBR ��Ⱦ���п�������ʵ��ɫ


*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void renderSphere();
void renderCube();

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
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
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
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
    glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.

    // build and compile shaders
    // -------------------------
    Shader pbrShader("res/Shaders/Shader_PBR/2/pbr.vert", "res/Shaders/Shader_PBR/2/pbr.frag");
    Shader equirectangularToCubemapShader("res/Shaders/Shader_PBR/2/cubemap.vert", "res/Shaders/Shader_PBR/2/equirectangular_to_cubemap.frag");
    Shader irradianceShader("res/Shaders/Shader_PBR/2/cubemap.vert", "res/Shaders/Shader_PBR/2/irradiance_convolution.frag");
    Shader backgroundShader("res/Shaders/Shader_PBR/2/background.vert", "res/Shaders/Shader_PBR/2/background.frag");


    pbrShader.use();
    pbrShader.setInt("irradianceMap", 0);
    pbrShader.setVec3("albedo", 0.5f, 0.0f, 0.0f);
    pbrShader.setFloat("ao", 1.0f);

    backgroundShader.use();
    backgroundShader.setInt("environmentMap", 0);


    // lights
    // ------
    glm::vec3 lightPositions[] = {
        glm::vec3(-10.0f,  10.0f, 10.0f),
        glm::vec3(10.0f,  10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f),
        glm::vec3(10.0f, -10.0f, 10.0f),
    };
    glm::vec3 lightColors[] = {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f)
    };
    int nrRows = 7;
    int nrColumns = 7;
    float spacing = 2.5;

    // pbr: setup framebuffer
    // ----------------------
    unsigned int captureFBO;
    unsigned int captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    // pbr: load the HDR environment map
    // ---------------------------------
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    float* data = stbi_loadf("res/box_hdr/Brooklyn_Bridge_Planks/Brooklyn_Bridge_Planks_2k.hdr", &width, &height, &nrComponents, 0);
    //float* data = stbi_loadf("res/box_hdr/Milkyway/Milkyway_small.hdr", &width, &height, &nrComponents, 0);
    //float* data = stbi_loadf("res/box_hdr/Basketball_Court/BasketballCourt_3k.hdr", &width, &height, &nrComponents, 0);
    //float* data = stbi_loadf("res/box_hdr/Alexs_Apartment/Alexs_Apt_2k.hdr", &width, &height, &nrComponents, 0);
    unsigned int hdrTexture;
    if (data)
    {
        glGenTextures(1, &hdrTexture);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load HDR image." << std::endl;
    }

    // pbr: setup cubemap to render to and attach to framebuffer
    // ---------------------------------------------------------
    unsigned int envCubemap;
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
    // ----------------------------------------------------------------------------------------------
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    // pbr: convert HDR equirectangular environment map to cubemap equivalent
    // ----------------------------------------------------------------------
    equirectangularToCubemapShader.use();
    equirectangularToCubemapShader.setInt("equirectangularMap", 0);
    equirectangularToCubemapShader.setMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);

    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        equirectangularToCubemapShader.setMat4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
    // --------------------------------------------------------------------------------
    unsigned int irradianceMap;
    glGenTextures(1, &irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
    // -----------------------------------------------------------------------------
    irradianceShader.use();
    irradianceShader.setInt("environmentMap", 0);
    irradianceShader.setMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

    glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        irradianceShader.setMat4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // initialize static shader uniforms before rendering
    // --------------------------------------------------
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    pbrShader.use();
    pbrShader.setMat4("projection", projection);
    backgroundShader.use();
    backgroundShader.setMat4("projection", projection);

    // then before rendering, configure the viewport to the original framebuffer's screen dimensions
    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

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
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render scene, supplying the convoluted irradiance map to the final shader.
        // ------------------------------------------------------------------------------------------
        pbrShader.use();
        glm::mat4 view = camera.GetViewMatrix();
        pbrShader.setMat4("view", view);
        pbrShader.setVec3("camPos", camera.Position);

        // bind pre-computed IBL data
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);

        // render rows*column number of spheres with varying metallic/roughness values scaled by rows and columns respectively
        glm::mat4 model = glm::mat4(1.0f);
        for (int row = 0; row < nrRows; ++row)
        {
            pbrShader.setFloat("metallic", (float)row / (float)nrRows);
            for (int col = 0; col < nrColumns; ++col)
            {
                // we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
                // on direct lighting.
                pbrShader.setFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(
                    (float)(col - (nrColumns / 2)) * spacing,
                    (float)(row - (nrRows / 2)) * spacing,
                    -2.0f
                ));
                pbrShader.setMat4("model", model);
                renderSphere();
            }
        }


        // render light source (simply re-render sphere at light positions)
        // this looks a bit off as we use the same shader, but it'll make their positions obvious and 
        // keeps the codeprint small.
        for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
        {
            glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
            newPos = lightPositions[i];
            pbrShader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
            pbrShader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

            model = glm::mat4(1.0f);
            model = glm::translate(model, newPos);
            model = glm::scale(model, glm::vec3(0.5f));
            pbrShader.setMat4("model", model);
            renderSphere();
        }

        // render skybox (render as last to prevent overdraw)
        backgroundShader.use();
        backgroundShader.setMat4("view", view);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        //glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); // display irradiance map
        renderCube();


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
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

// renders (and builds at first invocation) a sphere
// -------------------------------------------------
unsigned int sphereVAO = 0;
unsigned int indexCount;
void renderSphere()
{
    if (sphereVAO == 0)
    {
        glGenVertexArrays(1, &sphereVAO);

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = static_cast<unsigned int>(indices.size());

        std::vector<float> data;
        for (unsigned int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
        }
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        unsigned int stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    }

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
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