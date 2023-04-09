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
#include"res/GL_heads/Model.hpp"


#define SCR_WIDTH 1440
#define SCR_HEIGHT 810
#define GLSL_VERSION "#version 330\n"

/*
ʵ����
��������һ�������˺ܶ�ģ�͵ĳ��������󲿷ֵ�ģ�Ͱ�������ͬһ�鶥�����ݣ�ֻ�������е��ǲ�ͬ������ռ�任������һ�������ݵĳ�����ÿ���ݶ���һ���������������ε�Сģ�͡�����ܻ���Ҫ���ƺܶ���ݣ�������ÿ֡������ܻ���Ҫ��Ⱦ��ǧ����������ݡ���Ϊÿһ���ݽ������ɼ��������ι��ɣ���Ⱦ������˲����ɵģ�����ǧ����Ⱦ��������ȴ�Ἣ���Ӱ�����ܡ�
���������Ҫ��Ⱦ��������ʱ�����뿴��������������
	for(unsigned int i = 0; i < amount_of_models_to_draw; i++)
	{
		DoSomePreparations(); // ��VAO������������uniform��
		glDrawArrays(GL_TRIANGLES, 0, amount_of_vertices);
	}
�������������ģ�͵Ĵ���ʵ��(Instance)����ܿ�ͻ���Ϊ���Ƶ��ù�����ﵽ����ƿ��������ƶ��㱾����ȣ�ʹ��glDrawArrays��glDrawElements��������GPUȥ������Ķ������ݻ����ĸ�������ܣ���ΪOpenGL�ڻ��ƶ�������֮ǰ��Ҫ���ܶ�׼���������������GPU�ô��ĸ������ȡ���ݣ�����Ѱ�Ҷ������ԣ�������Щ��������Ի�����CPU��GPU����(CPU to GPU Bus)�Ͻ��еģ������ԣ�������Ⱦ����ǳ��죬����GPUȥ��Ⱦȴδ�ء�

ʵ����(Instancing)
��������ܹ�������һ���Է��͸�GPU��Ȼ��ʹ��һ�����ƺ�����OpenGL������Щ���ݻ��ƶ�����壬�ͻ�������ˡ������ʵ����(Instancing)��
ʵ����������ܹ�������ʹ��һ����Ⱦ���������ƶ�����壬����ʡÿ�λ�������ʱCPU -> GPU��ͨ�ţ���ֻ��Ҫһ�μ��ɡ������ʹ��ʵ������Ⱦ������ֻ��Ҫ��glDrawArrays��glDrawElements����Ⱦ���÷ֱ��ΪglDrawArraysInstanced��glDrawElementsInstanced�Ϳ����ˡ�
��Щ��Ⱦ������ʵ�����汾��Ҫһ������Ĳ���������ʵ������(Instance Count)�����ܹ�����������Ҫ��Ⱦ��ʵ����������������ֻ��Ҫ����������ݷ��͵�GPUһ�Σ�Ȼ��ʹ��һ�κ������ø���GPU��Ӧ����λ�����Щʵ����GPU����ֱ����Ⱦ��Щʵ���������ò��ϵ���CPU����ͨ�š�
�����������û��ʲô�á���Ⱦͬһ������һǧ�ζ����ǲ�û��ʲô�ô���ÿ�����嶼����ȫ��ͬ�ģ����һ���ͬһ��λ�á�

gl_InstanceID
GLSL�ڶ�����ɫ����Ƕ������һ���ڽ�������gl_InstanceID
��ʹ��ʵ������Ⱦ����ʱ��gl_InstanceID���0��ʼ����ÿ��ʵ������Ⱦʱ����1������˵������������Ⱦ��43��ʵ������ô������ɫ��������gl_InstanceID������42��
��Ϊÿ��ʵ������Ψһ��ID�����ǿ��Խ���һ�����飬��ID��λ��ֵ��Ӧ��������ÿ��ʵ������������Ĳ�ͬλ�á�

���ǽ����ڱ�׼���豸����ϵ��ʹ��һ����Ⱦ���ã�����100��2D�ı��Ρ�
������ɫ��
	#version 330 core
	layout (location = 0) in vec2 aPos;
	layout (location = 1) in vec3 aColor;
	out vec3 fColor;
	uniform vec2 offsets[100];
	void main()
	{
		vec2 offset = offsets[gl_InstanceID];
		gl_Position = vec4(aPos + offset, 0.0, 1.0);
		fColor = aColor;
	}
�������Ƕ�����һ������offsets�����飬������100��ƫ���������ڶ�����ɫ���У����ǻ�ʹ��gl_InstanceID������offsets���飬��ȡÿ��ʵ����ƫ���������������Ҫʵ��������100���ı��Σ���ʹ�����������ɫ�����Ǿ��ܵõ�100��λ�ڲ�ͬλ�õ��ı��Ρ�
��ǰ��������Ҫ������Щƫ��λ�ã����ǻ��ڽ�����Ⱦѭ��֮ǰʹ��һ��Ƕ��forѭ�����㣺
	glm::vec2 translations[100];
	int index = 0;
	float offset = 0.1f;
	for(int y = -10; y < 10; y += 2)
	{
		for(int x = -10; x < 10; x += 2)
		{
			glm::vec2 translation;
			translation.x = (float)x / 10.0f + offset;
			translation.y = (float)y / 10.0f + offset;
			translations[index++] = translation;
		}
	}
������Ǵ���100��λ����������ʾ10x10�����ϵ�����λ�á���������translations����֮�⣬���ǻ���Ҫ������ת�Ƶ�������ɫ����uniform�����У�
	shader.use();
	for(unsigned int i = 0; i < 100; i++)
	{
		stringstream ss;
		string index;
		ss << i;
		index = ss.str();
		shader.setVec2(("offsets[" + index + "]").c_str(), translations[i]);
	}
����һ�δ����У����ǽ�forѭ���ļ�����iת��Ϊһ��string�����ǿ�����������̬����λ��ֵ���ַ���������uniformλ��ֵ�������������������ǻ��offsets uniform�����е�ÿһ�����ö�Ӧ��λ��������
�������е�׼�������������ˣ����ǿ��Կ�ʼ��Ⱦ�ı����ˡ�����ʵ������Ⱦ������ʹ��glDrawArraysInstanced��glDrawElementsInstanced����Ϊ����ʹ�õĲ����������壬���ǻ����glDrawArrays�汾�ĺ�����
	glBindVertexArray(quadVAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
glDrawArraysInstanced�Ĳ�����glDrawArrays��ȫһ�������������˸���������������Ҫ���Ƶ�ʵ����������Ϊ������Ҫ��10x10��������ʾ100���ı��Σ����ǽ�������Ϊ100.���д���֮����Ӧ���ܵõ���Ϥ��100����ʵ��ı��Ρ�

ʵ��������
��Ȼ֮ǰ��ʵ����Ŀǰ��������ܹ����������������������Ҫ��ȾԶ����100��ʵ����ʱ������ʵ�ǳ��ձ飩���������ջᳬ������ܹ���������ɫ����uniform���ݴ�С���ޡ�
����һ�����淽����ʵ��������(Instanced Array)����������Ϊһ���������ԣ��ܹ������Ǵ����������ݣ������ڶ�����ɫ����Ⱦһ���µ�ʵ��ʱ�Ż���¡�
ʹ�ö�������ʱ��������ɫ����ÿ�����ж�����GLSL��ȡ��һ�������ڵ�ǰ��������ԡ��������ǽ��������Զ���Ϊһ��ʵ��������ʱ��������ɫ����ֻ��Ҫ��ÿ��ʵ����������ÿ�����㣬���¶������Ե������ˡ����������Ƕ��𶥵������ʹ����ͨ�Ķ������ԣ�������ʵ��������ʹ��ʵ�������顣

Ϊ�˸���һ��ʵ������������ӣ����ǽ�ʹ��֮ǰ�����ӣ�����ƫ����uniform��������Ϊһ��ʵ�������顣������Ҫ�ڶ�����ɫ���������һ���������ԣ�
	#version 330 core
	layout (location = 0) in vec2 aPos;
	layout (location = 1) in vec3 aColor;
	layout (location = 2) in vec2 aOffset;
	out vec3 fColor;

	void main()
	{
		gl_Position = vec4(aPos + aOffset, 0.0, 1.0);
		fColor = aColor;
	}
���ǲ���ʹ��gl_InstanceID�����ڲ���Ҫ����һ��uniform������ܹ�ֱ��ʹ��offset�����ˡ�
��Ϊʵ���������position��color����һ�������Ƕ������ԣ����ǻ���Ҫ���������ݴ��ڶ��㻺������У�����������������ָ�롣�������Ƚ�����һ���ֵģ�translations����浽һ���µĻ�������У�
	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
֮�����ǻ���Ҫ�������Ķ�������ָ�룬�����ö������ԣ�
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1);
���һ�У����ǵ�����glVertexAttribDivisor���������������OpenGL��ʲôʱ����¶������Ե���������һ�����ݡ����ĵ�һ����������Ҫ�Ķ������ԣ��ڶ������������Գ���(Attribute Divisor)��
Ĭ������£����Գ�����0������OpenGL������Ҫ�ڶ�����ɫ����ÿ�ε���ʱ���¶������ԡ���������Ϊ1ʱ�����Ǹ���OpenGL����ϣ������Ⱦһ����ʵ����ʱ����¶������ԡ�������Ϊ2ʱ������ϣ��ÿ2��ʵ������һ�����ԣ��Դ����ơ����ǽ����Գ�������Ϊ1�����ڸ���OpenGL������λ��ֵ2�Ķ���������һ��ʵ�������顣

Ϊ�˸���Ȥһ�㣬����Ҳ����ʹ��gl_InstanceID�������ϵ���������С�ı��Σ�
void main()
{
	vec2 pos = aPos * (gl_InstanceID / 100.0);
	gl_Position = vec4(pos + aOffset, 0.0, 1.0);
	fColor = aColor;
}

С���Ǵ�
��������һ������������������һ��������ǣ���λ��С���Ǵ������롣������С���Ǵ����ܰ�����ǧ������ҿ飬�ںܲ�����Կ���Ҳ���������������Ⱦ��ʵ������Ⱦ���������������ĳ�������Ϊ���е�С���Ƕ�����ʹ��һ��ģ������ʾ��ÿ��С���ǿ�����ʹ�ò�ͬ�ı任��������������ı仯��
�ں��ʵĻ����£�ʵ������Ⱦ�ܹ���������Կ�����Ⱦ���������ǳ������ԭ��ʵ������Ⱦͨ����������Ⱦ�ݡ�ֲ�������ӣ��Լ����������ĳ�����������ֻҪ�������кܶ��ظ�����״�����ܹ�ʹ��ʵ������Ⱦ��������ܡ�
���ͼ��Ⱦ��10���С����
��ĳЩ�����ϣ�10���С���ǿ��ܻ�̫���ˣ����Գ����޸����ֵ��ֱ���ﵽһ�����ܽ��ܵ�֡�ʡ�


�����
��ѧϰ��Ⱦ����;�У�����ܻ�ʱ��ʱ����ģ�ͱ�Ե�о�ݵ��������Щ��ݱ�Ե(Jagged Edges)�Ĳ����͹�դ������������ת��ΪƬ�εķ�ʽ�йء�
���ܹ���������γɱ�Ե�����ء��������󱻳�֮Ϊ����(Aliasing)���кܶ��ֿ���ݣ�Anti-aliasing��Ҳ����Ϊ���������ļ����ܹ��������ǻ����������󣬴Ӷ�������ƽ���ı�Ե��
�ʼ������һ�ֽ��������������(Super Sample Anti-aliasing, SSAA)�ļ���������ʹ�ñ������ֱ��ʸ��ߵķֱ��ʣ���������������Ⱦ��������ͼ�������֡�����и���ʱ���ֱ��ʻᱻ�²���(Downsample)�������ķֱ��ʡ���Щ����ķֱ��ʻᱻ������ֹ��ݱ�Ե�Ĳ�������Ȼ��ȷʵ�ܹ�������������⣬��������������ƽʱҪ���Ƹ����Ƭ�Σ���Ҳ������ܴ�����ܿ��������������ֻӵ���˶��ݵĻԻ͡�
Ȼ������������Ļ�����Ҳ�����˸�Ϊ�ִ��ļ������������ز��������(Multisample Anti-aliasing, MSAA)���������SSAA����������ȴ�Ը��Ӹ�Ч�ķ�ʽʵ���˿���ݡ�

���ز���
Ϊ�����ʲô�Ƕ��ز���(Multisampling)���Լ�������ν���������ģ������б�Ҫ����������˽�OpenGL��դ���Ĺ�����ʽ��
��դ����λ�����մ�����Ķ���֮��Ƭ����ɫ��֮ǰ�����������е��㷨����̵��ܺ͡���դ���Ὣһ��ͼԪ�����ж�����Ϊ���룬������ת��Ϊһϵ�е�Ƭ�Ρ��������������Ͽ���ȡ����ֵ����Ƭ�β��У���Ϊ�����������㴰�ڵķֱ��ʡ�����������Ƭ��֮�伸����ԶҲ������һ��һ��ӳ�䣬���Թ�դ��������ĳ�ַ�ʽ������ÿ�������������ڵ�Ƭ��/��Ļ���ꡣ
�������ǿ��Կ���һ����Ļ���ص�����ÿ�����ص����İ�����һ��������(Sample Point)�����ᱻ������������������Ƿ��ڸ���ĳ�����ء�ͼ�к�ɫ�Ĳ����㱻���������ڸǣ���ÿһ����ס�����ش���������һ��Ƭ�Ρ���Ȼ�����α�Ե��һЩ����Ҳ��ס��ĳЩ��Ļ���أ�������Щ���صĲ����㲢û�б��������ڲ����ڸǣ��������ǲ����ܵ�Ƭ����ɫ����Ӱ�졣
������Ļ�������������ƣ���Щ��Ե�������ܹ�����Ⱦ����������Щ�򲻻ᡣ�����������ʹ���˲��⻬�ı�Ե����ȾͼԪ������֮ǰ���۵��ľ�ݱ�Ե��
���ز������������ǽ���һ�Ĳ������Ϊ��������㣨��Ҳ�������Ƶ������������ǲ���ʹ���������ĵĵ�һ�����㣬ȡ����֮�������ض�ͼ�����е�4���Ӳ�����(Subsample)�����ǽ�����Щ�Ӳ��������������ص��ڸǶȡ���Ȼ����Ҳ��ζ����ɫ����Ĵ�С�������Ӳ���������Ӷ����ӡ�
���������������������ģ�����Ĳ������ܴ�������ȷ���ڸ��ʡ�
�����￪ʼ���ز����ͱ����Ȥ�����ˡ�����֪��������ֻ�ڸ���2���Ӳ����㣬������һ���Ǿ���������ص���ɫ����Ĳ�������ǣ����Ƕ�ÿ�����ڸ�ס���Ӳ���������һ��Ƭ����ɫ�������ÿ�����������Ӳ��������ɫƽ��һ�¡�����������У�������Ҫ�������Ӳ������϶Ա���ֵ�Ķ���������������Ƭ����ɫ���������������ɫ��������Щ�������С������˵��ǣ��Ⲣ�����������ķ�ʽ����Ϊ�Ȿ����˵������Ҫ���и���ε�Ƭ����ɫ�����������ؽ������ܡ�
MSAA�����Ĺ�����ʽ�ǣ������������ڸ��˶��ٸ��Ӳ����㣬��ÿ��ͼԪ�У�ÿ������ֻ����һ��Ƭ����ɫ����Ƭ����ɫ����ʹ�õĶ������ݻ��ֵ��ÿ�����ص����ģ����õ��Ľ����ɫ�ᱻ������ÿ�����ڸ�ס���Ӳ������С�����ɫ�������������ͼԪ��������ɫ����ʱ�����е���Щ��ɫ������ÿ�������ڲ�ƽ��������Ϊ��ͼ��4����������ֻ��2�����ڸ�ס�ˣ�������ص���ɫ��������������ɫ�������������������ɫ������������ɫ����ƽ��ֵ�������γ�һ�ֵ���ɫ��

OpenGL�е�MSAA
���������Ҫ��OpenGL��ʹ��MSAA�����Ǳ���Ҫʹ��һ������ÿ�������д洢����1����ɫֵ����ɫ���壨��Ϊ���ز�����Ҫ����Ϊÿ�������㶼����һ����ɫ�������ԣ�������Ҫһ���µĻ������ͣ����洢�ض������Ķ��ز������������������ز�������(Multisample Buffer)��
������Ĵ���ϵͳ��Ӧ���ṩ��һ�����ز������壬���Դ���Ĭ�ϵ���ɫ���塣GLFWͬ����������������ܣ�������Ҫ����ֻ����ʾ(Hint) GLFW������ϣ��ʹ��һ������N�������Ķ��ز������塣������ڴ�������֮ǰ����glfwWindowHint����ɡ�
	glfwWindowHint(GLFW_SAMPLES, 4);
�����ٵ���glfwCreateWindow������Ⱦ����ʱ��ÿ����Ļ����ͻ�ʹ��һ������4���Ӳ��������ɫ�����ˡ�GLFW���Զ�����һ��ÿ����4���Ӳ��������Ⱥ��������塣��Ҳ��ζ�����л���Ĵ�С��������4����
���������Ѿ���GLFW�����˶��ز������壬���ǻ���Ҫ����glEnable������GL_MULTISAMPLE�������ö��ز������ڴ����OpenGL�������ϣ����ز�������Ĭ�����õģ�����������ÿ��ܻ��е���࣬����ʽ�ص���һ�»������һ�㡣�����Ӳ�����ʲôOpenGL��ʵ�ֶ��ܹ��������ö��ز����ˡ�
	glEnable(GL_MULTISAMPLE);
ֻҪĬ�ϵ�֡�������˶��ز�������ĸ�����������Ҫ����ֻ�ǵ���glEnable�����ö��ز�������Ϊ���ز������㷨����OpenGL�����Ĺ�դ����ʵ���ˣ����ǲ���Ҫ�ٶ���ʲô���������������Ⱦ����һ��ʼ���Ǹ���ɫ�������壬����Ӧ���ܿ�����ƽ���ı�Ե

����MSAA
����GLFW�����˴������ز������壬����MSAA�ǳ��򵥡�Ȼ�������������Ҫʹ�������Լ���֡����������������Ⱦ����ô���Ǿͱ���Ҫ�Լ��������ɶ��ز��������ˡ�
�����ַ�ʽ���Դ������ز������壬������Ϊ֡����ĸ���������������Ⱦ���帽��

���ز���������
Ϊ�˴���һ��֧�ִ��������������������ʹ��glTexImage2DMultisample�����glTexImage2D����������Ŀ����GL_TEXTURE_2D_MULTISAPLE��
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
���ĵڶ����������õ���������ӵ�е�����������������һ������ΪGL_TRUE��ͼ�񽫻��ÿ������ʹ����ͬ������λ���Լ���ͬ�������Ӳ����������
����ʹ��glFramebufferTexture2D�����ز��������ӵ�֡�����ϣ���������������ʹ�õ���GL_TEXTURE_2D_MULTISAMPLE��
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tex, 0);
��ǰ�󶨵�֡�������ھ�����һ������ͼ����ʽ�Ķ��ز�����ɫ���塣

���ز�����Ⱦ�������
���������ƣ�����һ�����ز�����Ⱦ������󲢲��ѡ�������Ҫ����ֻ����ָ������ǰ�󶨵ģ���Ⱦ������ڴ�洢ʱ����glRenderbufferStorage�ĵ��ø�ΪglRenderbufferStorageMultisample�Ϳ����ˡ�
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
�����У���Ⱦ��������Ĳ������ǽ��趨Ϊ�������������ڵ�ǰ����������4��

��Ⱦ�����ز���֡����
��Ⱦ�����ز���֡�������Ĺ��̶����Զ��ġ�ֻҪ������֡�����ʱ�����κζ�������դ���ͻḺ�����еĶ��ز������㡣�������ջ�õ�һ�����ز�����ɫ�����Լ�/����Ⱥ�ģ�建�塣��Ϊ���ز���������һ���ر����ǲ���ֱ�ӽ����ǵĻ���ͼ�������������㣬��������ɫ���ж����ǽ��в�����
һ�����ز�����ͼ���������ͨͼ��������Ϣ��������Ҫ��������С���߻�ԭ(Resolve)ͼ�񡣶��ز���֡����Ļ�ԭͨ����ͨ��glBlitFramebuffer����ɣ����ܹ���һ��֡�����е�ĳ�������Ƶ���һ��֡�����У����ҽ����ز������廹ԭ��
glBlitFramebuffer�Ὣһ����4����Ļ�ռ������������Դ�����Ƶ�һ��ͬ����4����Ļ�ռ������������Ŀ�������С�
����ܼǵ���֡����̳��У������ǰ󶨵�GL_FRAMEBUFFERʱ��������ͬʱ���˶�ȡ�ͻ��Ƶ�֡����Ŀ�ꡣ����Ҳ���Խ�֡����ֿ�����GL_READ_FRAMEBUFFER��GL_DRAW_FRAMEBUFFER��
glBlitFramebuffer���������������Ŀ�꣬�����ĸ���Դ֡���壬�ĸ���Ŀ��֡���塣�����������ǿ��Խ�ͼ��λ�鴫��(Blit)��Ĭ�ϵ�֡�����У������ز�����֡���崫�͵���Ļ�ϡ�
	glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampledFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

�Զ��忹����㷨
��һ�����ز���������ͼ�񲻽��л�ԭֱ�Ӵ�����ɫ��Ҳ�ǿ��еġ�GLSL�ṩ��������ѡ��������ܹ�������ͼ���ÿ�����������в������������ǿ��Դ��������Լ��Ŀ�����㷨���ڴ��͵�ͼ��Ӧ����ͨ��������ô����
Ҫ���ȡÿ������������ɫֵ������Ҫ������uniform����������Ϊsampler2DMS��������ƽ��ʹ�õ�sampler2D��
	uniform sampler2DMS screenTextureMS;
ʹ��texelFetch�������ܹ���ȡÿ������������ɫֵ�ˣ�
	vec4 colorSample = texelFetch(screenTextureMS, TexCoords, 3);  // ��4��������


*/

/* matrix */
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

/* vector */
glm::vec2 rec_pos = glm::vec2(0.0f);
glm::vec3 lightPos = glm::vec3(1.2f, 1.3f, -1.5f);

/* camera attributes */
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
static float alpha = 0.4f;
bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//VBO
float vertices[] =
{
	//positions				color				texCoords		normals����Դ���㣩
	//����
	-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.5f,			0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.5f,			0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
	//�ײ�
	-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, -1.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
	-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,		0.0f, 1.0f,		0.0f, 0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
	//����
	-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 1.0f,		0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
	//����
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f,			0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f,			0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 1.0f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	//����
	-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,			0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,			0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	//ǰ��
	-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 1.0f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
};

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
};

glm::vec3 pointLightPositions[] =
{
	glm::vec3(0.7f, 0.2f, 2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(4.0f, 2.0f, -12.0f),
	glm::vec3(0.0f, 0.0f, -3.0f)
};


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (alpha < 1.0)
		{
			alpha += 0.01f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (alpha > 0)
		{
			alpha -= 0.01f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		rec_pos.x -= 0.04f;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		rec_pos.x += 0.04f;
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		rec_pos.y += 0.04f;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		rec_pos.y -= 0.04f;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKayboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKayboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKayboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKayboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		camera.ProcessKayboard(UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		camera.ProcessKayboard(DOWN, deltaTime);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_cursor_posotion(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}
	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;
	lastX = (float)xpos;
	lastY = (float)ypos;

	//ֻ�ڰ�������Ҽ�ʱ����任�ӽ�
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_TRUE)
	{
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

void mouse_scroll_posotion(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}

unsigned int load_texture(const char* texture_path)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	stbi_set_flip_vertically_on_load(true);
	/*filter options*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(texture_path, &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
		{
			format = GL_RED;
		}
		if (nrChannels == 3)
		{
			format = GL_RGB;
		}
		else if (nrChannels == 4)
		{
			format = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_cursor_posotion);
	glfwSetScrollCallback(window, mouse_scroll_posotion);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD." << std::endl;
	}

	//���Ч����ʾ
	glEnable(GL_DEPTH_TEST);
	//������ز�������
	glEnable(GL_MULTISAMPLE);

	/*Model*/
	stbi_set_flip_vertically_on_load(true);
	Model rock("res/object/rock/rock.obj");
	Model planet("res/object/planet/planet.obj");

	/*Shader*/
	Shader asteroidShader("res/Shaders/Shader_advancedOpenGL/8/asteroids.vert", "res/Shaders/Shader_advancedOpenGL/8/asteroids.frag");
	Shader planetShader("res/Shaders/Shader_advancedOpenGL/8/planet.vert", "res/Shaders/Shader_advancedOpenGL/8/planet.frag");

	// generate a large list of semi-random model transformation matrices
	unsigned int amount = 10000;
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
	float radius = 150.0;
	float offset = 25.0f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale: Scale between 0.05 and 0.25f
		float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = static_cast<float>((rand() % 360));
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}

	// configure instanced array
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	// set transformation matrices as an instance vertex attribute (with divisor 1)
	// note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
	// normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
	for (unsigned int i = 0; i < rock.meshes.size(); i++)
	{
		unsigned int VAO = rock.meshes[i].VAO;
		glBindVertexArray(VAO);
		// set attribute pointers for matrix (4 times vec4)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		float time = (float)glfwGetTime();
		float xValue = (float)(0.5 * std::sin(time) + 0.5);
		float yValue = (float)(0.5 * std::cos(time) + 0.5);
		float camX = std::sin(time) * 5.0f;
		float camZ = std::cos(time) * 5.0f;
		deltaTime = time - lastFrame;
		lastFrame = time;

		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// configure transformation matrices
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		asteroidShader.use();
		asteroidShader.setMat4("projection", projection);
		asteroidShader.setMat4("view", view);
		planetShader.use();
		planetShader.setMat4("projection", projection);
		planetShader.setMat4("view", view);

		// draw planet
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		planetShader.setMat4("model", model);
		planet.Draw(planetShader);

		// draw meteorites
		asteroidShader.use();
		asteroidShader.setInt("texture_diffuse1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rock.textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
		for (unsigned int i = 0; i < rock.meshes.size(); i++)
		{
			glBindVertexArray(rock.meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(rock.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, amount);
			glBindVertexArray(0);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}