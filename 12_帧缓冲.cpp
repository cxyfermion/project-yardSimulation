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
#include<map>
#include <vector>

#define SCR_WIDTH 1000
#define SCR_HEIGHT 800
#define GLSL_VERSION "#version 330\n"

/*
��ĿǰΪֹ�������Ѿ�ʹ���˺ܶ���Ļ�����ˣ�����д����ɫֵ����ɫ���塢����д�������Ϣ����Ȼ�����������Ǹ���һЩ���������ض�Ƭ�ε�ģ�建�塣��Щ��������������֡����(Framebuffer)�������������ڴ��С�OpenGL�������Ƕ��������Լ���֡���壬Ҳ����˵�����ܹ����������Լ�����ɫ���壬��������Ȼ����ģ�建�塣
����Ŀǰ���������в���������Ĭ��֡�������Ⱦ�����Ͻ��еġ�Ĭ�ϵ�֡���������㴴�����ڵ�ʱ�����ɺ����õģ�GLFW������������Щ�������������Լ���֡���壬���Ǿ��ܹ��и��෽ʽ����Ⱦ�ˡ�
����ܲ��ܺܿ����֡�����Ӧ�ã�����Ⱦ��ĳ�������ͬ��֡�����ܹ��������ڳ����м������ƾ��ӵĶ��������������ܿ�ĺ��ڴ���Ч�����������ǻ�����������ι����ģ�֮�����ǽ���ʵ����Щ�ſ�ĺ��ڴ���Ч����
��OpenGL�е���������һ�������ǻ�ʹ��һ������glGenFramebuffers�ĺ���������һ��֡�������(Framebuffer Object, FBO)��
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
���ִ�����ʹ�ö���ķ�ʽ�����Ѿ������ܶ���ˣ���������ʹ�ú���Ҳ�������Ķ������ơ��������Ǵ���һ��֡������󣬽�����Ϊ�����(Active)֡���壬��һЩ������֮����֡���塣����ʹ��glBindFramebuffer����֡���塣
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
�ڰ󶨵�GL_FRAMEBUFFERĿ��֮�����еĶ�ȡ��д��֡����Ĳ�������Ӱ�쵱ǰ�󶨵�֡���塣����Ҳ����ʹ��GL_READ_FRAMEBUFFER��GL_DRAW_FRAMEBUFFER����һ��֡����ֱ�󶨵���ȡĿ���д��Ŀ�ꡣ�󶨵�GL_READ_FRAMEBUFFER��֡���彫��ʹ������������glReadPixels�Ķ�ȡ�����У����󶨵�GL_DRAW_FRAMEBUFFER��֡���彫�ᱻ������Ⱦ�������д�������Ŀ�ꡣ�󲿷�����㶼����Ҫ�������ǣ�ͨ������ʹ��GL_FRAMEBUFFER���󶨵������ϡ�
���ҵ��ǣ��������ڻ�����ʹ�����ǵ�֡���壬��Ϊ����������(Complete)��һ��������֡������Ҫ�������µ�������
1.��������һ�����壨��ɫ����Ȼ�ģ�建�壩��
2.������һ����ɫ����(Attachment)��
3.���еĸ����������������ģ��������ڴ棩��
4.ÿ�����嶼Ӧ������ͬ����������

������������п���֪����������ҪΪ֡���崴��һЩ�����������������ӵ�֡�����ϡ���������е�����֮�����ǿ�����GL_FRAMEBUFFERΪ��������glCheckFramebufferStatus�����֡�����Ƿ��������������⵱ǰ�󶨵�֡���壬�����ع淶����Щֵ������֮һ����������ص���GL_FRAMEBUFFER_COMPLETE��֡��������������ˡ�
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
֮�����е���Ⱦ����������Ⱦ����ǰ��֡����ĸ����С��������ǵ�֡���岻��Ĭ��֡���壬��Ⱦָ�����Դ��ڵ��Ӿ�������κ�Ӱ�졣�������ԭ����Ⱦ��һ����ͬ��֡���屻����������Ⱦ(Off-screen Rendering)��Ҫ��֤���е���Ⱦ�����������������Ӿ�Ч����������Ҫ�ٴμ���Ĭ��֡���壬�����󶨵�0��
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
��������е�֡�������֮�󣬲�Ҫ����ɾ�����֡�������
	glDeleteFramebuffers(1, &fbo);
�������Լ��ִ��֮ǰ��������Ҫ��֡���帽��һ��������������һ���ڴ�λ�ã����ܹ���Ϊ֡�����һ�����壬���Խ�������Ϊһ��ͼ�񡣵�����һ��������ʱ������������ѡ��������Ⱦ�������(Renderbuffer Object)��

������
����һ�������ӵ�֡�����ʱ�����е���Ⱦָ���д�뵽��������У���������һ����ͨ����ɫ/��Ȼ�ģ�建��һ����ʹ��������ŵ��ǣ�������Ⱦ�����Ľ�����ᱻ������һ������ͼ���У�����֮���������ɫ���кܷ����ʹ������
Ϊ֡���崴��һ������ʹ���һ����ͨ�������ࣺ
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
��Ҫ��������ǣ����ǽ�ά������Ϊ����Ļ��С�������ⲻ�Ǳ���ģ����������Ǹ������data����������NULL����������������ǽ����������ڴ��û�����������������������������Ⱦ��֡����֮�������С�ͬ��ע�����ǲ������Ļ��Ʒ�ʽ��༶��Զ���������ڴ��������¶�������Ҫ���ǡ�
ע�⣺������뽫�����Ļ��Ⱦ��һ����С�����������ϣ�����Ҫ������Ⱦ�����֡����֮ǰ���ٴε���glViewport��ʹ���������ά����Ϊ����������ֻ��һС���ֵ��������Ļ�ᱻ��Ⱦ����������ϡ�

���������Ѿ�������һ�������ˣ�Ҫ�������һ���¾��ǽ������ӵ�֡�������ˣ�
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
glFrameBufferTexture2D�����µĲ�����
	target��֡�����Ŀ�꣨���ơ���ȡ�������߽��У�
	attachment��������Ҫ���ӵĸ������͡���ǰ�������ڸ���һ����ɫ������ע������0��ζ�����ǿ��Ը��Ӷ����ɫ���������ǽ���֮��Ľ̳����ᵽ��
	textarget����ϣ�����ӵ���������
	texture��Ҫ���ӵ�������
	level���༶��Զ����ļ������ǽ�������Ϊ0��
������ɫ����֮�⣬���ǻ����Ը���һ����Ⱥ�ģ�建������֡��������С�Ҫ������Ȼ���Ļ������ǽ�������������ΪGL_DEPTH_ATTACHMENT��ע������ĸ�ʽ(Format)���ڲ���ʽ(Internalformat)���ͽ���ΪGL_DEPTH_COMPONENT������ӳ��Ȼ���Ĵ����ʽ��Ҫ����ģ�建��Ļ�����Ҫ���ڶ�����������ΪGL_STENCIL_ATTACHMENT����������ĸ�ʽ�趨ΪGL_STENCIL_INDEX��

Ҳ���Խ���Ȼ����ģ�建�帽��Ϊһ�����������������ÿ32λ��ֵ������24λ�������Ϣ��8λ��ģ����Ϣ��Ҫ����Ⱥ�ģ�建�帽��Ϊһ������Ļ�������ʹ��GL_DEPTH_STENCIL_ATTACHMENT���ͣ�����������ĸ�ʽ�����������ϲ�����Ⱥ�ģ��ֵ����һ����Ⱥ�ģ�建�帽��Ϊһ������֡��������ӿ����������ҵ���
glTexImage2D(
  GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0,
  GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
);
...
glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);

��Ⱦ������󸽼�
��Ⱦ�������(Renderbuffer Object)��������֮�����뵽OpenGL�У���Ϊһ�����õ�֡���帽�����͵ģ������ڹ�ȥ������Ψһ���õĸ�����������ͼ��һ������Ⱦ���������һ�������Ļ��壬��һϵ�е��ֽڡ����������صȡ���Ⱦ������󸽼ӵĺô��ǣ����Ὣ���ݴ���ΪOpenGLԭ������Ⱦ��ʽ������Ϊ������Ⱦ��֡�����Ż����ġ�
��Ⱦ�������ֱ�ӽ����е���Ⱦ���ݴ��浽���Ļ����У��������κ���������ʽ��ת����������Ϊһ������Ŀ�д������ʡ�Ȼ������Ⱦ�������ͨ������ֻд�ģ������㲻�ܶ�ȡ���ǣ�����ʹ��������ʣ�����Ȼ����Ȼ�����ܹ�ʹ��glReadPixels����ȡ�������ӵ�ǰ�󶨵�֡���壬�����Ǹ��������з����ض���������ء�

��Ϊ���������Ѿ���ԭ���ĸ�ʽ�ˣ���д����߸����������ݵ�����������ʱ�Ƿǳ���ġ����ԣ��������������Ĳ�����ʹ����Ⱦ�������ʱ��ǳ��졣������ÿ����Ⱦ�������ʹ�õ�glfwSwapBuffers��Ҳ����ͨ����Ⱦ�������ʵ�֣�ֻ��Ҫд��һ����Ⱦ����ͼ�񣬲�����󽻻�������һ����Ⱦ����Ϳ����ˡ���Ⱦ�����������ֲ����ǳ�������
����һ����Ⱦ�������Ĵ����֡����Ĵ�������ƣ�
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
���ƣ�������Ҫ�������Ⱦ���������֮�����е���Ⱦ�������Ӱ�쵱ǰ��rbo��
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
������Ⱦ�������ͨ������ֻд�ģ����ǻᾭ��������Ⱥ�ģ�帽������Ϊ�󲿷�ʱ�����Ƕ�����Ҫ����Ⱥ�ģ�建���ж�ȡֵ��ֻ������Ⱥ�ģ����ԡ�������Ҫ��Ⱥ�ģ��ֵ���ڲ��ԣ�������Ҫ�����ǽ��в�����������Ⱦ�������ǳ��ʺ����ǡ������ǲ���Ҫ����Щ�����в�����ʱ��ͨ������ѡ����Ⱦ���������Ϊ������Ż�һ�㡣
����һ����Ⱥ�ģ����Ⱦ����������ͨ������glRenderbufferStorage��������ɣ�
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
����һ����Ⱦ������������������ƣ���ͬ�������������ר�ű������Ϊ֡���帽��ʹ�õģ�����������������ͨ�����ݻ���(General Purpose Data Buffer)����������ѡ��GL_DEPTH24_STENCIL8��Ϊ�ڲ���ʽ������װ��24λ����Ⱥ�8λ��ģ�建�塣
���һ���¾��Ǹ��������Ⱦ�������
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
��Ⱦ���������Ϊ���֡��������ṩһЩ�Ż�����֪��ʲôʱ��ʹ����Ⱦ�������ʲôʱ��ʹ�������Ǻ���Ҫ�ġ�ͨ���Ĺ����ǣ�����㲻��Ҫ��һ�������в������ݣ���ô���������ʹ����Ⱦ�������������ǵ�ѡ���������Ҫ�ӻ����в�����ɫ�����ֵ�����ݣ���ô��Ӧ��ѡ�������������ܷ�������������ǳ����Ӱ��ġ�
Ҫע��һЩ���顣��һ����������ʹ�õ�ÿ��֡���嶼�����Լ�һ�׻��壬����ϣ�����ú��ʵ�λ������glClear�������Щ���塣
���չʾ�����Ӿ������������Ȳ���������ȫһ���ģ����������Ⱦ��һ���򵥵��ı����ϡ��������ʹ���߿�ģʽ��Ⱦ�������ͻ��ú����ԣ�������Ĭ�ϵ�֡������ֻ������һ���򵥵��ı��Ρ�

���������ʲô�ô��أ���Ϊ�����ܹ���һ������ͼ��ķ�ʽ��������Ⱦ�����е�ÿ�����أ����ǿ�����Ƭ����ɫ���д������ǳ���Ȥ��Ч������Щ��ȤЧ��ͳ��Ϊ���ڴ���(Post-processing)Ч����

���ڴ���
	��Ȼ��������������Ⱦ����һ�������ϣ����ǿ��Լ򵥵�ͨ���޸��������ݴ�����һЩ�ǳ�����˼��Ч��������һ�����У����ǽ�������չʾһЩ���еĺ��ڴ���Ч����������������ʹ�ô������������Լ���Ч����
����
���ǽ������Ļ������ȡ��ɫֵ��Ȼ����1.0��ȥ�����������з��ࣺ
	void main()
	{
		FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
	}
	�Ҷ�
	����һ������Ȥ��Ч���ǣ��Ƴ������г��˺ڰ׻��������е���ɫ��������ͼ��ҶȻ�(Grayscale)���ܼ򵥵�ʵ�ַ�ʽ�ǣ�ȡ���е���ɫ������������ƽ������
		void main()
		{
			FragColor = texture(screenTexture, TexCoords);
			float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
			FragColor = vec4(average, average, average, 1.0);
		}
	���Ѿ��ܴ���ܺõĽ���ˣ������ۻ����ɫ��������һЩ��������ɫ����ô���У�����Ϊ�˻�ȡ�����ϸ���ȷ��Ч����������Ҫʹ�ü�Ȩ��(Weighted)ͨ����
		void main()
		{
			FragColor = texture(screenTexture, TexCoords);
			float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
			FragColor = vec4(average, average, average, 1.0);
		}

��Ч��
��һ������ͼ���������ڴ��������һ���ô��ǣ����ǿ��Դ�����������ط�������ɫֵ������˵���ǿ����ڵ�ǰ�����������ΧȡһС�����򣬶Ե�ǰ����ֵ��Χ�Ķ������ֵ���в��������ǿ��Խ�����Ǵ�����������˼��Ч����
��(Kernel)����������(Convolution Matrix)����һ����������ֵ���飬��������Ϊ��ǰ�����أ����������ĺ�ֵ������Χ������ֵ�����������ӱ��һ��ֵ�����ԣ��������������ڶԵ�ǰ������Χ�������������һ��С��ƫ�����������ݺ˽�����ϲ���
���Ǻ��ڴ���һ���ǳ����õĹ��ߣ�����ʹ�ú�ʵ���������ܼ򵥣�����Ҳ���ҵ��ܶ����ӡ�������Ҫ��΢�޸�һ��Ƭ����ɫ���������ܹ�֧�ֺˡ����Ǽ���ʹ�õĺ˶���3x3�ˣ�ʵ���ϴ󲿷ֺ˶��ǣ���
	const float offset = 1.0 / 300.0;
	void main()
	{
		vec2 offsets[9] = vec2[](
			vec2(-offset,  offset), // ����
			vec2( 0.0f,    offset), // ����
			vec2( offset,  offset), // ����
			vec2(-offset,  0.0f),   // ��
			vec2( 0.0f,    0.0f),   // ��
			vec2( offset,  0.0f),   // ��
			vec2(-offset, -offset), // ����
			vec2( 0.0f,   -offset), // ����
			vec2( offset, -offset)  // ����
		);

		float kernel[9] = float[](
			-1, -1, -1,
			-1,  9, -1,
			-1, -1, -1
		);

		vec3 sampleTex[9];
		for(int i = 0; i < 9; i++)
		{
			sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
		}
		vec3 col = vec3(0.0);
		for(int i = 0; i < 9; i++)
			col += sampleTex[i] * kernel[i];

		FragColor = vec4(col, 1.0);
	}
	��Ƭ����ɫ���У���������Ϊ��Χ���������괴����һ��9��vec2ƫ���������顣ƫ������һ������������԰������ϲ���Զ�������֮�����Ƕ���һ���ˣ��������������һ����(Sharpen)�ˣ����������Χ���������أ���ÿ����ɫֵ������ڲ���ʱ���ǽ�ÿ��ƫ�����ӵ���ǰ���������ϣ���ȡ��Ҫ����������֮����Щ����ֵ���Լ�Ȩ�ĺ�ֵ���������Ǽӵ�һ��

ģ��
	����ģ��(Blur)Ч���ĺ���������
	float kernel[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	);
	��������ֵ�ĺ���16������ֱ�ӷ��غϲ��Ĳ�����ɫ�������ǳ�������ɫ������������Ҫ���˵�ÿ��ֵ������16�����յĺ����齫���ǣ�

��Ե���
	����ı�Ե���(Edge-detection)�˺��񻯺˷ǳ����ƣ�
	float kernel[9] = float[](
		1.0, 1.0, 1.0,
		1.0, -8.0, 1.0,
		1.0, 1.0, 1.0
	);

����ܲ�����֣�����Photoshop������ͼ���޸Ĺ���/�˾�ʹ�õ�Ҳ�������ĺˡ���Ϊ�Կ�����Ƭ�ε�ʱ�����ż�ǿ�Ĳ��д������������ǿ��Ժ����ɵ���ʵʱ������������ض�ͼ����д�������ͼ��༭������ͼ�����ʱ����������ʹ���Կ���
ע�⣬���ڶ���Ļ����ı�Ե���в�����ʱ�����ڻ��������������Χ��8�����ؽ��в�������ʵ��ȡ������֮������ء����ڻ��Ʒ�ʽĬ����GL_REPEAT��������û�����õ������ȡ��������Ļ��һ�ߵ����أ�����һ�ߵ����ر���Ӧ�ö��������ز���Ӱ�죬��Ϳ��ܻ�����Ļ��Ե��������ֵ����ơ�Ϊ��������һ���⣬���ǿ��Խ���Ļ����Ļ��Ʒ�ʽ������ΪGL_CLAMP_TO_EDGE����������ȡ�������������ʱ�����ܹ��ظ���Ե������������ȷ�ع������յ�ֵ�ˡ�




*/

/* matrix */
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

/* vector */
glm::vec2 rec_pos = glm::vec2(0.0f);

/* camera attributes */
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
static float alpha = 0.4f;
bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

/*
	Remember: to specify vertices in a counter-clockwise winding order you need to visualize the triangle
	as if you're in front of the triangle and from that point of view, is where you set their order.

	To define the order of a triangle on the right side of the cube for example, you'd imagine yourself looking
	straight at the right side of the cube, and then visualize the triangle and make sure their order is specified
	in a counter-clockwise order. This takes some practice, but try visualizing this yourself and see that this
	is correct.
*/
float cubeVertices[] = {
	// Back face
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
	// Front face
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
	// Left face
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
	// Right face
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
	 // Bottom face
	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
	  0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
	 // Top face
	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
	 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
};

float planeVertices[] = {
	// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};

float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
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

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	Shader shader("res/Shaders/Shader_advancedOpenGL/3/framebuffers.vert", "res/Shaders/Shader_advancedOpenGL/3/framebuffers.frag");
	Shader screenShader("res/Shaders/Shader_advancedOpenGL/3/framebuffers_screen.vert", "res/Shaders/Shader_advancedOpenGL/3/framebuffers_screen.frag");

	/*imGUI*/
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	ImGui::StyleColorsDark();

	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	// plane VAO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	// screen quad VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	unsigned int cubeTexture = load_texture("res/textures/container.png");
	unsigned int floorTexture = load_texture("res/textures/marble.png");

	shader.use();
	shader.setInt("texture1", 0);

	screenShader.use();
	screenShader.setInt("screenTexture", 0);

	// framebuffer configuration
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ����ģʽ��������Ƿ�������������
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

		/*start new frame for imGUI*/
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// render
		// bind to framebuffer and draw scene as we normally would to color texture 
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

		// make sure we clear the framebuffer's content
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		// cubes
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// floor
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		shader.setMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		/*��ԭ���������Ƶ�����ȫ������֡����������������glFramebufferTexture2D����������fbo���������*/
		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader.use();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);

		/*ImGui*/
		float FPS = ImGui::GetIO().Framerate;
		ImGui::Begin("Gui");
		ImGui::Text("FPS = %f", FPS);
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &quadVBO);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}