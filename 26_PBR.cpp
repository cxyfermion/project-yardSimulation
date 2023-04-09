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
����
PBR�������ø�ͨ��һЩ�ĳƺ���ָ�����������Ⱦ(Physically Based Rendering)����ָ����һЩ�ڲ�ͬ�̶��϶���������ʵ���������ԭ�������Ļ������������ɵ���Ⱦ�����ļ��ϡ�����Ϊ�����������ȾĿ�ı���Ϊ��ʹ��һ�ָ���������ѧ���ɵķ�ʽ��ģ����ߣ����������Ⱦ��ʽ������ԭ����Phong����Blinn-Phong�����㷨��������Ͽ�����Ҫ����ʵһЩ��
���˿���������Щ���⣬���������������ʷǳ��ӽ���������ǣ�����������ʦ�ǣ�����ֱ�����������Ϊ��������д������ʣ��������������ӵ��޸���������ù���Ч������ȥ������ʹ�û�����������ķ�������д���ʻ���һ������ĺô������ǲ��۹���������Σ���Щ���ʿ���ȥ��������ȷ�ģ����ڷ�PBR����Ⱦ���ߵ�����Щ�����Ͳ�����ô��ʵ�ˡ�
��Ȼ��ˣ������������Ⱦ��Ȼֻ�ǶԻ�������ԭ�����ʵ�����һ�ֽ��ƣ���Ҳ����Ϊʲô������Ϊ�����������ɫ(Physically based Shading) ����������ɫ(Physical Shading)��ԭ���ж�һ��PBR����ģ���Ƿ��ǻ�������ģ��������������������������õ��ģ����Ǻܿ�ͻ��˽����ǵģ�
    ����΢ƽ��(Microfacet)�ı���ģ�͡�
    �����غ㡣
    Ӧ�û��������BRDF��
����ε�PBRϵ�н̳�֮�У����ǽ�����ص���������ɵ�ʿ��(Disney)���̽�ֲ���Epic Games����Ӧ����ʵʱ��Ⱦ��PBR���������ǻ��ڽ����ʵع�����(Metallic Workflow)�ķ����зǳ��걸�����׼�¼���㷺Ӧ���ڸ������е�����֮�в������ŷǳ����˾�̾���Ӿ�Ч����

΢ƽ��ģ��
���е�PBR����������΢ƽ�����ۡ�����������Ϊ���ﵽ΢�۳߶�֮���κ�ƽ�涼�����ñ���Ϊ΢ƽ��(Microfacets)��ϸС������������档����ƽ��ֲڳ̶ȵĲ�ͬ����ЩϸС�����ȡ�����п����൱��һ��
������Ч�����ǣ�һ��ƽ��Խ�Ǵֲڣ����ƽ���ϵ�΢ƽ������о�Խ���ҡ���Щ΢С������������ȡ�����е�Ӱ����ǣ���������ָ�����/���淴��ʱ��������߸�����������ȫ��ͬ�ķ���ɢ(Scatter)�����������������ֲ���Χ���㷺�ľ��淴�䡣����֮�෴���ǣ�����һ���⻬��ƽ�棬���ߴ����ϻ����������ͬһ�������䣬��ɸ�С�������ķ���

��΢�۳߶��£�û���κ�ƽ������ȫ�⻬�ġ�Ȼ��������Щ΢ƽ���Ѿ�΢С���޷������صؼ�������������֣�������Ǽ���һ���ֲڶ�(Roughness)������Ȼ����ͳ��ѧ�ķ���������΢ƽ��Ĵֲڳ̶ȡ�
���ǿ��Ի���һ��ƽ��Ĵֲڶ���������ڶ�΢ƽ���У�����������ĳ������h����ı������������h����λ�ڹ�������l����������v֮��İ������(Halfway Vector)������������֮ǰ�ĸ߼����ս̳���̸�����м����������ļ��㷽�����£�
    h = (l + v) / ||(l + v)||
΢ƽ��ĳ��������������ķ���Խ��һ�£����淴���Ч����Խ��ǿ��Խ��������ͨ��ʹ��һ������0��1֮��ĴֲڶȲ��������Ǿ��ܸ��Եع���΢ƽ���ȡ�������

�����غ�
΢ƽ����Ʒ�ʹ��������һ����ʽ�������غ�(Energy Conservation)��������ߵ�������Զ���ܳ���������ߵ���������������⣩������ͼ���ǿ��Կ��������Ŵֲڶȵ����������淴����������ӣ����Ǿ��淴�������ȴ���½������ÿ�����صľ��淴��ǿ�ȶ�һ�������ܷ��������Ĵ�С������ô�ֲڵ�ƽ��ͻ������������������������Υ���������غ㶨�ɡ���Ҳ����Ϊʲô�������ǿ�����һ�����⻬ƽ��ľ��淴���ǿ�Ҷ��ֲ�ƽ��ķ�����谵��
Ϊ�����������غ㶨�ɣ�������Ҫ���������;��淴���������ȷ�����֡���һ��������ײ��һ�������ʱ�����ͻ�����һ�����䲿�ֺ�һ�����䲿�֡����䲿�־��ǻ�ֱ�ӷ��俪��������ƽ����ǲ��ֹ��ߣ�Ҳ����������˵�ľ�����ա������䲿�־������µĻ������沢�����յ��ǲ��ֹ��ߣ�Ҳ����������˵����������ա�
���ﻹ��һЩϸ����Ҫ������Ϊ�����߽Ӵ���һ�������ʱ��������ǲ��������ͱ����յġ�ͨ������ѧ���ǿ��Ե�֪������ʵ���Ͽ��Ա���Ϊ��һ��û�кľ��Ͳ�ͣ��ǰ�˶�����������������ͨ����ײ�ķ�ʽ������������ÿһ�ֲ��϶���������΢С����������ɣ���Щ���Ӷ�������ͼ��ʾһ������߷�����ײ����Щ������ÿ�ε���ײ�ж��������չ�����Я����һ���ֻ�����ȫ������������ת���Ϊ������
һ����˵������ȫ���������ᱻ���գ�������Ҳ��������ţ������ϣ�����ķ���ɢ��Ȼ���ٺ�������������ײֱ��������ȫ�ľ������ٴ��뿪������档�����������������󽫻�Эͬ���ɸñ���ģ������䣩��ɫ�������ڻ����������Ⱦ֮�����ǽ����˼򻯣������ƽ���ϵ�ÿһ�����е�����ⶼ�ᱻ��ȫ���ն�����ɢ����
����һЩ����Ϊ�α���ɢ��(Subsurface Scattering)��������ɫ��������������⿼���˽�ȥ������������������һЩ����Ƥ��������ʯ���������������ʵ��Ӿ�Ч����������������Ĵ��������ܵ��½���
���ڽ���(Metallic)���棬�����۵������������ʱ����һ��ϸ����Ҫע�⡣��������Թ�ķ�Ӧ��ǽ�����Ҳ����Ϊ�����(Dielectrics)����������ǲ�ͬ�ġ�������ӵķ���������ԭ������ͬ�ģ��������е�����ⶼ�ᱻֱ�����ն�����ɢ����ֻ���·�������˵���淴��⡣�༴��˵����������ֻ����ʾ���淴����ɫ����������ʾ����������ɫ�����ڽ���������֮������������Ե������������������PBR��Ⱦ�����б������������ǽ������µĺ����һ����ϸ̽��������⡣
������������֮����������ʹ���ǵõ�����һ�����������غ�ľ�����ۣ����������������Ƕ���֮���ǻ���Ĺ�ϵ�����ۺ��ֹ��ߣ��䱻���ʱ�����������������޷��ٱ��������ա���ˣ�������������������µĽ������֮�е��������þ������Ǽ����귴��֮�����µ�������
���ǰ��������غ�Ĺ�ϵ�����ȼ��㾵�淴�䲿�֣�����ֵ����������߱������������ռ�İٷֱȡ�Ȼ������ⲿ�־Ϳ���ֱ���ɾ��淴�䲿�ּ���ó���
    float kS = calculateSpecularComponent(...); // ����/���� ����
    float kD = 1.0 - ks;                        // ����/������ ����
�������Ǿ��������������غ㶨�ɵ�ǰ����֪��������ߵķ��䲿�������䲿����ռ�������ˡ��������ַ�������/�������뷴��/���淴����ռ�ķݶ���ᳬ��1.0����˾��ܱ�֤���ǵ������ܺ���Զ���ᳬ��������ߵ�����������Щ����������ǰ��Ĺ��ս̳���û�п��ǵ����⡣

�����ʷ���
����������������һ�ֱ���Ϊ��Ⱦ����(Render Equation)�Ķ���������ĳЩ�����������������������һ������ķ���ʽ�������������ӵ�е�����ģ�����Ӿ�Ч����õ�ģ�͡������������Ⱦ���ᶨ��ѭ����һ�ֱ���Ϊ�����ʷ���(The Reflectance Equation)����Ⱦ���̵��ػ��汾��Ҫ��ȷ�����PBR������Ҫ��һ�����Ҫ����͸������ⷴ���ʷ���
���Ǳ���Ҫ��΢����һЩ�������ѧ(Radiometry)�����ݡ��������ѧ��һ������������ų����䣨�����ɼ��⣩���ֶΡ��кܶ��ַ������(radiometric quantities)�������������������ĳ�������ϵĹ⣬�������ǽ�ֻ���������кͷ����ʷ����йص�һ�֡�������Ϊ������(Radiance)����������L����ʾ�������ʱ������������Ե�һ�����ϵĹ��ߵĴ�С����ǿ�ȡ�

����ͨ��������ͨ������ʾ����һ����Դ�������������������Ϊ��λ�������ɶ��ֲ�ͬ���������������϶��ɵģ���ÿ�ֲ�������һ���ض��ģ��ɼ��ģ���ɫ��ء����һ����Դ������������������Ա����������Դ���������и��ֲ�����һ����������������390nm��700nm�����ף��Ĺⱻ��Ϊ�Ǵ��ڿɼ�������У�Ҳ����˵���������ۿɼ��Ĳ�����
����ͨ�������������ɲ�ͬ�������ɵĺ������������ֱ�ӽ����ֶԲ�ͬ�����ļ�����Ϊ������������ͼ����һЩ����ʵ�ʣ��������ͨ����ֱ��ʹ�ò�����ǿ�ȶ���ʹ����ԭɫ���룬Ҳ����RGB�����߰�ͨ���ĳƺ�����ɫ������Ϊ����ͨ����ʾ�ļ򻯡����ױ���ȷʵ�����һЩ��Ϣ�ϵ���ʧ������������Ӿ�Ч���ϵ�Ӱ��������Ժ��ԡ�

����ǣ�������æر�ʾ��������Ϊ��������Ͷ�䵽��λ�����ϵ�һ������Ĵ�С���������Ͷ�䵽�����λ�����ϵĽ��������ͱ���Ϊ�����(Solid Angle)������԰�����������Ϊһ����������ķ���
���԰��Լ������Ϊһ��վ�ڵ�λ��������ĵĹ۲��ߣ�����ͶӰ�ķ��򿴡����ͶӰ�����Ĵ�С��������ǡ�

����ǿ�ȣ�����ǿ��(Radiant Intensity)��ʾ�����ڵ�λ�����ϣ�һ����Դ��ÿ��λ�������Ͷ�͵ķ���ͨ����������˵������һ��ȫ���Դ�����з�����ȵķ�������������ǿ�Ⱦ��ܰ����Ǽ��������һ����λ���������ǣ��ڵ�������С

�������Ƿ������ѧ�ϱ�ʾһ������ƽ���Ϲ��������������������ܵ�����(Incident)���������䣩������ƽ�淨�߼�ļнǦȵ�����ֵcos�ȵ�Ӱ�죺��ֱ�ӷ��䵽ƽ���ϵĳ̶�Խ��ʱ�����߾�Խ��������������ȫ��ֱ��ƽ��ʱǿ����ߡ����������ǰ��Ļ������ս̳��ж�����������յĸ������ƣ�����cos�Ⱦ�ֱ�Ӷ�Ӧ�ڹ��ߵķ���������ƽ�淨�����ĵ��
    float cosTheta = dot(lightDir, N);
�����ʷ��̺����ã���Ϊ���Ѵ󲿷����Ǹ���Ȥ���������������˽�ȥ��������ǰ�����Ǧغ����A����������С�ģ���ô���Ǿ����÷���������ʾ�������ߴ����ռ��е�һ�����ͨ�������ʹ���ǿ��Լ���ó������ڵ�����Ƭ�Σ����ϵĵ������ߵķ����ʣ�����ʵ���ϰ�����Ǧ�ת��Ϊ����������Ȼ�����Aת��Ϊ��p���������Ǿ���ֱ�������ǵ���ɫ����ʹ�÷����������㵥�����߶�ÿ��Ƭ�ε������ˡ�

��ʵ�ϣ����漰��������ʱ������ͨ�����ĵ�������Ͷ�䵽��p�ϵĹ��ߵ��ܺͣ�������;ͳ�Ϊ�����նȻ��߷��ն�(Irradiance)��

Ϊ�˼���ĳЩ�����ֵ�����������ڰ�������������м���ĳһ�������ʱ�����ǻ���Ҫ�õ�һ�ֳ�Ϊ����(Integral)����ѧ�ֶΣ�Ҳ���Ƿ����ʹ�ʽ�еķ��šң�������������˰����������������䷽���ϵ�d��i�����������ֵ����һ���������ߵ���������ļ�����Ҫô�ǽ�����Ҫô������ֵ�⡣
������Ⱦ���̺ͷ����ʷ��̶�û�н����⣬���ǽ�������ɢ�ķ��������������ֵ���ֵ�⡣��������ת��Ϊ���ڰ��������а�һ���Ĳ����������ʷ��̷�ɢ��⣬Ȼ���ٰ��ղ�����С�����õ��Ľ��ƽ���������ַ�������Ϊ������(Riemann sum) �����ǿ���������Ĵ�����Ե���ʾһ�£�
    int steps = 100;
    float sum = 0.0f;
    vec3 P    = ...;
    vec3 Wo   = ...;
    vec3 N    = ...;
    float dW  = 1.0f / steps;
    for(int i = 0; i < steps; ++i)
    {
        vec3 Wi = getNextIncomingLightDir(i);
        sum += Fr(p, Wi, Wo) * L(p, Wi) * dot(N, Wi) * dW;
    }
ͨ������dW����������ɢ���ֽ������ţ�������͵��ڻ��ֺ������������������������������ÿ����ɢ�����������ŵ�dW������Ϊ���Ƿ����ʷ����е�d��i������ѧ�ϣ�����������ֵ�d��i��ʾ����һ�������ķ��ţ�������ʹ�õ�dW�ڴ����к�����û��ֱ�ӵ���ϵ����Ϊ����������������е���ɢ������������˵��Ϊ�˿��԰�������⡣���μǣ�ʹ����ɢ�����õ����Ǻ����������һ������ֵ��ϸ�ĵĶ��߿����Ѿ�ע�⵽�ˣ����ǿ���ͨ��������ɢ���ֵ���������������͵�׼ȷ��(Accuracy)��
�����ʷ��̸������ڰ��������ڣ���ײ���˵�p�ϵ��������䷽���i �ϵĹ��ߵķ����ʣ����ܵ�fr��Լ����Ȼ�󷵻ع۲췽���Ϸ�����Lo��������������Ϥ�����������������ʿ����ɹ�Դ����ã����⻹��������һ��������ͼ�������������䷽���ϵķ����ʣ����ǽ���δ����IBL�̳����������������

����Ψһʣ�µ�δ֪���ž���fr�ˣ�������ΪBRDF������˫����ֲ�����(Bidirectional Reflective Distribution Function) �����������ǻ��ڱ����������������������ʽ������Ż��߼�Ȩ��

BRDF
BRDF������˵˫����ֲ����������������䣨�⣩�����i�����䣨�۲죩�����o��ƽ�淨��n�Լ�һ��������ʾ΢ƽ��ֲڳ̶ȵĲ���a��Ϊ���������������BRDF���Խ��Ƶ����ÿ�����߶�һ�������˲������Ե�ƽ�������շ�������Ĺ����������Ĺ��׳̶ȡ�������˵�����һ��ƽ��ӵ����ȫ�⻬�ı��棨���羵�棩����ô�������е�������ߦ�i������һ�����⣩����BRDF�������᷵��0.0 ��ֻ��һ���������ߦ�oӵ����ͬ�������䣩�ǶȵĹ��߻�õ�1.0�������ֵ��
BRDF��������֮ǰ��̽�ֹ���΢ƽ�����������Ƶ���ò��ʵķ������������ԡ�����һ��BRDF��Ϊ��ʵ������ѧ�ϵĿ��Ŷȣ����������������غ㶨�ɣ�Ҳ����˵������ߵ��ܺ���Զ���ܳ���������ߵ��������ϸ�����˵��ͬ�����æ�i�ͦ�o��Ϊ��������� Blinn-Phong����ģ��Ҳ����Ϊ��һ��BRDF��Ȼ������Blinn-Phongģ�Ͳ�û����ѭ�����غ㶨�ɣ������������Ϊ�ǻ����������Ⱦ�������Ѿ��кܺü���BRDF���ܽ��Ƶĵó����������ڹ�ķ�Ӧ�����Ǽ�������ʵʱ��Ⱦ����ʹ�õĶ���һ�ֱ���ΪCook-Torrance BRDFģ�͡�
Cook-Torrance BRDF����������;��淴����������

Ŀǰ��������಻ͬ���͵�ģ����ʵ��BRDF�������䲿�֣���࿴��ȥ���൱��ʵ��������Ӧ�����㿪��Ҳ�ǳ��İ��󡣲�������Epic��˾�����Ľ��ۣ�Lambertian������ģ���Ѿ��㹻Ӧ�������ʵʱ��Ⱦ����;�ˡ�

Cook-Torrance BRDF�ľ��淴�䲿�ְ������������������ĸ���ֻ���һ����׼������ ����ĸD��F��G�ֱ������һ�����͵ĺ��������������ֱ��������Ƶļ�������淴�����Ե�һ���ض����֡����������ֱ�Ϊ���߷ֲ�����(Normal Distribution Function)������������(Fresnel Rquation)�ͼ��κ���(Geometry Function)��
    ���߷ֲ��������������ܵ�����ֲڶȵ�Ӱ���£���������������һ�µ�΢ƽ���������������������΢ƽ�����Ҫ������
    ���κ�����������΢ƽ���Գ���Ӱ�����ԡ���һ��ƽ����ԱȽϴֲڵ�ʱ��ƽ������ϵ�΢ƽ���п��ܵ�ס������΢ƽ��Ӷ����ٱ���������Ĺ��ߡ�
    ���������̣��������������������ڲ�ͬ�ı�����±���������Ĺ�����ռ�ı��ʡ�
���ϵ�ÿһ�ֺ�����������������Ӧ����������ģ�������ᷢ������ʵ����Ӧ������Ƶ�ÿ�ֺ������в�ֹһ����ʽ�������еķǳ���ʵ���е������ܸ�Ч������԰����Լ�����������ѡ���Լ���Ҫ�ĺ�����ʵ�ַ�����Ӣ����Ϸ��˾��Brian Karis������Щ�����Ķ��ֽ���ʵ�ַ�ʽ�����˴������о������ǽ������Epic Games��Unreal Engine 4����ʹ�õĺ���������Dʹ��Trowbridge-Reitz GGX��Fʹ��Fresnel-Schlick����(Fresnel-Schlick Approximation)����Gʹ��Smith��s Schlick-GGX��

���߷ֲ�����
���߷ֲ�����D����ͳ��ѧ�Ͻ��Ƶر�ʾ����ĳЩ����̣�����hȡ��һ�µ�΢ƽ��ı��ʡ�������˵�������������h��������ǵ�΢ƽ������35%������hȡ��һ�£����߷ֲ���������˵NDF���᷵��0.35��Ŀǰ�кܶ���NDF�����Դ�ͳ��ѧ��������΢ƽ�������ȡ��ȣ�ֻҪ����һЩ�ֲڶȵĲ������������Ͻ�Ҫ�õ�����Trowbridge-Reitz GGX
ʹ��GLSL�����д��Trowbridge-Reitz GGX���߷ֲ�����������������ӵģ�
    float D_GGX_TR(vec3 N, vec3 H, float a)
    {
        float a2     = a*a;
        float NdotH  = max(dot(N, H), 0.0);
        float NdotH2 = NdotH*NdotH;

        float nom    = a2;
        float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
        denom        = PI * denom * denom;

        return nom / denom;
    }

���κ���
��NDF���ƣ����κ�������һ�����ϵĴֲڶȲ�����Ϊ����������ֲڶȽϸߵı�����΢ƽ����໥�ڱεĸ��ʾ�Խ�ߡ����ǽ�Ҫʹ�õļ��κ�����GGX��Schlick-Beckmann���ƵĽ���壬����ֳ�ΪSchlick-GGX
ʹ��GLSL��д�ļ��κ����������£�
    float GeometrySchlickGGX(float NdotV, float k)
    {
        float nom   = NdotV;
        float denom = NdotV * (1.0 - k) + k;

        return nom / denom;
    }

    float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
    {
        float NdotV = max(dot(N, V), 0.0);
        float NdotL = max(dot(N, L), 0.0);
        float ggx1 = GeometrySchlickGGX(NdotV, k);
        float ggx2 = GeometrySchlickGGX(NdotL, k);

        return ggx1 * ggx2;
    }
����������
������������ΪFreh-nel�������������Ǳ�����Ĺ��߶Աȹ��߱�����Ĳ�����ռ�ı��ʣ�������ʻ��������ǹ۲�ĽǶȲ�ͬ����ͬ����������ײ��һ�������ʱ�򣬷��������̻���ݹ۲�Ƕȸ������Ǳ�����Ĺ�����ռ�İٷֱȡ��������������ʺ������غ�ԭ�����ǿ���ֱ�ӵó����߱�����Ĳ����Լ�����ʣ���������
����ֱ�۲��ʱ���κ�������߲��ʱ��涼��һ������������(Base Reflectivity)�����������һ���ĽǶ���ƽ���Ͽ���ʱ�����з��ⶼ��������������������Լ�����һ�£��ô�ֱ���ӽǹ۲����Լ���ľ��/�������棬��ʱһ��ֻ��������ķ����ԡ����������ӽ���90�ȣ���ע��Ӧ����ָ�ͷ��ߵļнǣ��ĽǶȹ۲�Ļ�����ͻ������ԵĶࡣ����������90���ӽǹ۲죬���е�ƽ����������˵������ȫ�ķ�����ߡ���������������������������������˷���������֮�С�
���������̿�����Fresnel-Schlick���Ʒ���ý��ƽ�

������������ڵ���ʱ��������е�������������ν�Ľ����������ĸ��Ҳ����������Ҫ����ʹ��һ������Ϊ������(Metalness)�Ĳ����������д������ʡ���������������һ�����ʱ����ǽ������Ƿǽ����ġ�
ͨ��Ԥ�ȼ��������뵼���F0
ֵ�����ǿ��Զ��������͵ı���ʹ����ͬ��Fresnel-Schlick���ƣ���������ǽ�������Ļ�����Ҫ�Ի������������ɫ�ʡ�����һ���ǰ�����������ʵ�ֵģ�
    vec3 F0 = vec3(0.04);
    F0      = mix(F0, surfaceColor.rgb, metalness);
Fresnel Schlick���ƿ����ô����ʾΪ��
    vec3 fresnelSchlick(float cosTheta, vec3 F0)
    {
        return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
    }
����cosTheta�Ǳ��淨����n��۲췽��v�ĵ�˵Ľ����

Cook-Torrance�����ʷ���

��дPBR����
�����ʣ�������(Albedo)����Ϊÿһ������������(Texel)���������أ�ָ��������ɫ���߻��������ʡ��������֮ǰʹ�ù��������������൱���ƣ���ͬ�������й�����Ϣ������һ����������ȡ�ġ������������ͼ���г�������һЩϸС����Ӱ������ɫ�����ƣ����������������ǲ�������Щ�����ġ���Ӧ��ֻ�����������ɫ��������������ϵ������
���ߣ�������ͼ���������֮ǰ�ڷ�����ͼ�̳�����ʹ�õ���ͼ����ȫһ���ġ�������ͼʹ���ǿ�����Ƭ�ε�ָ�����صķ��ߣ���Ϊ��������������ƽ�ļ���
�����ȣ�����(Metallic)��ͼ������ص�ָ���������ǲ��ǽ����ʵصġ�����PBR�������õĲ�ͬ������ʦ�Ǽȿ��Խ������ȱ�дΪ�Ҷ�ֵ�ֿ��Ա�дΪ1��0�����Ķ�Ԫֵ��
�ֲڶȣ��ֲڶ�(Roughness)��ͼ����������Ϊ��λָ��ĳ�������ж�ֲڡ����������Ĵֲڶ���ֵ��Ӱ��һ�������΢ƽ��ͳ��ѧ�ϵ�ȡ��ȡ�һ���Ƚϴֲڵı����õ���������ģ���ľ��淴�䣨�߹⣩����һ���ȽϹ⻬�ı������õ����ж������ľ��淴�䡣ĳЩPBR����Ԥ����õ��Ƕ�ĳЩ����ʦ��˵����ֱ�۵Ĺ⻬��(Smoothness)��ͼ���Ǵֲڶ���ͼ��������Щ��ֵ�ڲ���֮ʱ�������ã�1.0 �C �⻬�ȣ�ת�����˴ֲڶȡ�
AO���������ڱ�(Ambient Occlusion)��ͼ����˵AO��ͼΪ�������ΧǱ�ڵļ���ͼ��ָ����һ���������Ӱ���ӡ��������������һ��ש����棬�����������ϵ�ש���ѷ첿��Ӧ��û���κ���Ӱ��Ϣ��Ȼ��AO��ͼ������Щ���߽������ݳ����İ�ɫ��Եָ���������ڹ��յĽ�β�׶����뻷���ڱο������Ե������㳡�����Ӿ�Ч��������/����Ļ����ڱ���ͼҪôͨ���ֶ����ɣ�Ҫô��3D��ģ����Զ����ɡ�

����
�����һ��ʼ�����Ǿͼ�����Դ���ܹ���˥��������ǿ�Ȼ����ž���䰵����Ӱ�죬��ô�������ǰѹ�Դ�����ģ�������ߵķ���������һ���ģ���ȥ�����cos�ȶԷ����ʵ�Ӱ��֮�⣩�� ��������Ϊ�������Ǵ��ĸ��Ƕȹ۲��������Դ�ܾ�����ͬ�ķ���ǿ�ȣ����ǿ�����Ч�ؽ������ǿ�Ƚ�ģΪ�����ͨ��: һ������������23.47,21.31,20.79����
Ȼ����������Ҳ��Ҫ��λ��p��Ϊ���룬����������ʵ�ĵ��Դ�����ܹ���˥��Ӱ��һ�������Դ�ķ���ǿ��Ӧ�ø��ݵ�p���ڵ�λ�ú͹�Դ��λ���Լ�����֮��ľ������һЩ���š� ��ˣ�����ԭʼ�ķ��䷽�̣����ǻ���ݱ��淨����n������Ƕ�wi�����Ź�Դ�ķ���ǿ�ȡ�
    vec3  lightColor  = vec3(23.47, 21.31, 20.79);
    vec3  wi          = normalize(lightPos - fragPos);
    float cosTheta    = max(dot(N, Wi), 0.0);
    float attenuation = calculateAttenuation(fragPos, lightPos);
    float radiance    = lightColor * attenuation * cosTheta;
�����������͵Ĵӵ��㷢�����Ĺ�Դ�������Ƶؼ���������ʡ����磬�����(directional light)ӵ�к㶨��wi��������˥�����ӣ���һ���۹�ƹ�Դ��û�к㶨�ķ���ǿ�ȣ������ǿ���Ǹ��ݾ۹�Ƶķ������������ŵġ�

���Կռ��HDR��Ⱦ
ֱ�����ڣ����Ǽ�������м��㶼�����Ե���ɫ�ռ��н��еģ����������Ҫ����ɫ�������٤������� �����Կռ��м�������Ƿǳ���Ҫ�ģ���ΪPBRҪ���������붼�����Եģ�����������������Ǿͻ�õ��������Ĺ��ա����⣬����ϣ�����й��յ����붼�����ܵĽӽ������������ϵ�ȡֵ���������ǵķ����ʻ���˵��ɫֵ�ͻ���ɫ�����бȽϴ�ı仯�ռ䡣Lo��Ϊ������ܻ���úܿ�(����1)��������ΪĬ�ϵ�LDR�����ȡֵ���ضϡ�������٤�����֮ǰ���ǲ���ɫ��ӳ��ʹLo��LDR��ֵӳ��ΪHDR��ֵ��
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

������ֱ�ӹ���PBR��ɫ��
    #version 330 core
    out vec4 FragColor;
    in vec2 TexCoords;
    in vec3 WorldPos;
    in vec3 Normal;

    // material parameters
    uniform vec3  albedo;
    uniform float metallic;
    uniform float roughness;
    uniform float ao;

    // lights
    uniform vec3 lightPositions[4];
    uniform vec3 lightColors[4];

    uniform vec3 camPos;

    const float PI = 3.14159265359;

    float DistributionGGX(vec3 N, vec3 H, float roughness);
    float GeometrySchlickGGX(float NdotV, float roughness);
    float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
    vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);

    void main()
    {
        vec3 N = normalize(Normal);
        vec3 V = normalize(camPos - WorldPos);

        vec3 F0 = vec3(0.04);
        F0 = mix(F0, albedo, metallic);

        // reflectance equation
        vec3 Lo = vec3(0.0);
        for(int i = 0; i < 4; ++i)
        {
            // calculate per-light radiance
            vec3 L = normalize(lightPositions[i] - WorldPos);
            vec3 H = normalize(V + L);
            float distance    = length(lightPositions[i] - WorldPos);
            float attenuation = 1.0 / (distance * distance);
            vec3 radiance     = lightColors[i] * attenuation;

            // cook-torrance brdf
            float NDF = DistributionGGX(N, H, roughness);
            float G   = GeometrySmith(N, V, L, roughness);
            vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

            vec3 kS = F;
            vec3 kD = vec3(1.0) - kS;
            kD *= 1.0 - metallic;

            vec3 nominator    = NDF * G * F;
            float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
            vec3 specular     = nominator / denominator;

            // add to outgoing radiance Lo
            float NdotL = max(dot(N, L), 0.0);
            Lo += (kD * albedo / PI + specular) * radiance * NdotL;
        }

        vec3 ambient = vec3(0.03) * albedo * ao;
        vec3 color = ambient + Lo;

        color = color / (color + vec3(1.0));
        color = pow(color, vec3(1.0/2.2));

        FragColor = vec4(color, 1.0);
    }



*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
void renderSphere();

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

    // build and compile shaders
    // -------------------------
    Shader shader("res/Shaders/Shader_PBR/1/pbr.vert", "res/Shaders/Shader_PBR/1/pbr.frag");

    shader.use();
    shader.setVec3("albedo", 0.5f, 0.0f, 0.0f);
    shader.setFloat("ao", 1.0f);

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

    // initialize static shader uniforms before rendering
    // --------------------------------------------------
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    shader.use();
    shader.setMat4("projection", projection);

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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);
        shader.setVec3("camPos", camera.Position);

        // render rows*column number of spheres with varying metallic/roughness values scaled by rows and columns respectively
        glm::mat4 model = glm::mat4(1.0f);
        for (int row = 0; row < nrRows; ++row)
        {
            shader.setFloat("metallic", (float)row / (float)nrRows);
            for (int col = 0; col < nrColumns; ++col)
            {
                // we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
                // on direct lighting.
                shader.setFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(
                    (col - (nrColumns / 2)) * spacing,
                    (row - (nrRows / 2)) * spacing,
                    0.0f
                ));
                shader.setMat4("model", model);
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
            shader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
            shader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

            model = glm::mat4(1.0f);
            model = glm::translate(model, newPos);
            model = glm::scale(model, glm::vec3(0.5f));
            shader.setMat4("model", model);
            renderSphere();
        }

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

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}