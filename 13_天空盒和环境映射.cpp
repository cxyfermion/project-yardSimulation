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
���ǽ����۵��ǽ���������������ӳ�䵽һ�������ϵ�һ���������ͣ���������ͼ(Cube Map)��

������������ͼ
��������ͼ�Ǻ���������һ���ģ���������봴��һ����������ͼ�Ļ���������Ҫ����һ������������󶨵�����Ŀ���ϣ�֮������������������������Ҫ�󶨵�GL_TEXTURE_CUBE_MAP��
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
��Ϊ��������ͼ������6������ÿ����һ����������Ҫ����glTexImage2D����6�Σ�������֮ǰ�̳��к����ơ�����һ�����ǽ�����Ŀ��(target)��������Ϊ��������ͼ��һ���ض����棬����OpenGL�����ڶ���������ͼ����һ���洴�����������ζ��������Ҫ����������ͼ��ÿһ���涼����һ��glTexImage2D��
����������6���棬OpenGL�������ṩ��6�����������Ŀ�꣬ר�Ŷ�Ӧ��������ͼ��һ���档
	����Ŀ��							��λ
	GL_TEXTURE_CUBE_MAP_POSITIVE_X	��
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X	��
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y	��
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	��
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z	��
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	ǰ
��OpenGL�ĺܶ�ö��(Enum)һ�������Ǳ����intֵ�����Ե����ģ��������������һ������λ�õ��������vector�����ǾͿ��Դ�GL_TEXTURE_CUBE_MAP_POSITIVE_X��ʼ�������ǣ���ÿ�������ж�ö��ֵ��1����������������Ŀ�꣺
	int width, height, nrChannels;
	unsigned char *data;
	for(unsigned int i = 0; i < textures_faces.size(); i++)
	{
		data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrChannels, 0);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
		);
	}
����������һ������textures_faces��vector������������������ͼ�������������·�������Ա��е�˳�����С��⽫Ϊ��ǰ�󶨵���������ͼ�е�ÿ��������һ������
��Ϊ��������ͼ����������ûʲô��ͬ������Ҳ��Ҫ�趨���Ļ��ƺ͹��˷�ʽ��
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
��Ҫ��GL_TEXTURE_WRAP_R�ŵ�����������Ϊ�����R���������˻��Ʒ�ʽ������Ӧ��������ĵ�����ά�ȣ���λ�õ�zһ���������ǽ����Ʒ�ʽ����ΪGL_CLAMP_TO_EDGE��������Ϊ���ô���������֮�������������ܲ��ܻ���һ���棨����һЩӲ�����ƣ�������ͨ��ʹ��GL_CLAMP_TO_EDGE��OpenGL�������Ƕ�������֮�������ʱ����Զ�������ǵı߽�ֵ��
��Ƭ����ɫ���У�����ʹ����һ����ͬ���͵Ĳ�������samplerCube�����ǽ�ʹ��texture����ʹ�������в�������������ǽ�ʹ��һ��vec3�ķ�������������vec2��ʹ����������ͼ��Ƭ����ɫ�������������ģ�
	in vec3 textureDir; // ����3D��������ķ�������
	uniform samplerCube cubemap; // ��������ͼ�����������
	void main()
	{
		FragColor = texture(cubemap, textureDir);
	}

��պ�
��պ���һ�����������������ģ��������壬��������Χ������6��ͼ���������Ϊ������һ����ʵ�ʴ�ö�Ļ������С���Ϸ��ʹ����պе�������Ⱥɽ�����ƻ��ǿա�
������պ�ʱ��������Ҫ������Ϊ�����еĵ�һ����Ⱦ�����壬���ҽ������д�롣��������պоͻ���Զ����������������ı����ˡ�
	glDepthMask(GL_FALSE);
	skyboxShader.use();
	// ... ���ù۲��ͶӰ����
	glBindVertexArray(skyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
	// ... ����ʣ�µĳ���
����ܻ��ǵ��ڻ�������С���У�����ͨ��ȡ4x4�������Ͻǵ�3x3�������Ƴ��任�����λ�Ʋ��֡����ǿ��Խ��۲����ת��Ϊ3x3�����Ƴ�λ�ƣ����ٽ���ת����4x4�������ﵽ���Ƶ�Ч����
	glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));

�Ż�
Ŀǰ������������Ⱦ��պУ�֮������Ⱦ�����е��������塣�������ܹ������������Ƿǳ���Ч�������������Ⱦ��պУ����Ǿͻ����Ļ�ϵ�ÿһ����������һ��Ƭ����ɫ��������ֻ��һС���ֵ���պ������ǿɼ��ġ�����ʹ����ǰ��Ȳ���(Early Depth Testing)���ɶ�������Ƭ���ܹ���ʡ���Ǻܶ౦��Ĵ���
���ԣ����ǽ��������Ⱦ��պУ��Ի����΢�����������������ӵĻ�����Ȼ���ͻ������������������ֵ�ˣ�����ֻ��Ҫ����ǰ��Ȳ���ͨ���ĵط���Ⱦ��պе�Ƭ�ξͿ����ˣ��ܴ�̶��ϼ�����Ƭ����ɫ���ĵ��á�
�����ǣ���պкܿ��ܻ���Ⱦ��������������֮�ϣ���Ϊ��ֻ��һ��1x1x1�������壨��ע����ζ�ž���������ľ���Ҳֻ��1������ͨ���󲿷ֵ���Ȳ��ԡ�
������Ȳ�����������Ⱦ���ǽ����������Ϊ��պн��Ḵд�����е��������塣������Ҫ��ƭ��Ȼ��壬������Ϊ��պ������������ֵ1.0��ֻҪ��ǰ����һ�����壬��Ȳ��Ծͻ�ʧ�ܡ�
������ϵͳС��������˵����͸�ӳ������ڶ�����ɫ������֮��ִ�еģ���gl_Position��xyz�������w�����������ִ���Ȳ���С����֪������������z�������ڶ�������ֵ��ʹ����Щ��Ϣ�����ǿ��Խ����λ�õ�z������������w��������z������Զ����1.0�������ӵĻ�����͸�ӳ���ִ��֮��z�������Ϊw / w = 1.0��
	void main()
	{
		TexCoords = aPos;
		vec4 pos = projection * view * vec4(aPos, 1.0);
		gl_Position = pos.xyww;
	}
���յı�׼���豸���꽫��Զ����һ������1.0��zֵ���������ֵ�����������պ�ֻ����û�пɼ�����ĵط���Ⱦ�ˣ�ֻ����������ͨ����Ȳ��ԣ��������еĶ���������պ�ǰ�棩��
���ǻ�Ҫ�ı�һ����Ⱥ�����������Ĭ�ϵ�GL_LESS��ΪGL_LEQUAL����Ȼ��彫���������պе�1.0ֵ������������Ҫ��֤��պ���ֵС�ڻ������Ȼ��������С��ʱͨ����Ȳ��ԡ�

����ӳ��
�������ڽ���������ӳ�䵽��һ������������ˣ������������Ϣ�Ĳ�����ֻ����պС�ͨ��ʹ�û�������������ͼ�����ǿ��Ը����巴�����������ԡ�����ʹ�û�����������ͼ�ļ�����������ӳ��(Environment Mapping)�����������е������Ƿ���(Reflection)������(Refraction)��
����
����������Ա���Ϊ���壨�������һ���֣���������Χ�����������ݹ۲��ߵ��ӽǣ��������ɫ�����ٵ������Ļ��������Ӿ���һ�����������壺������ݹ۲��ߵ��ӽǷ�������Χ�Ļ�����
���Ǹ��ݹ۲췽������I������ķ�����N�������㷴������R�����ǿ���ʹ��GLSL�ڽ���reflect��������������������������յ�R����������Ϊ����/������������ͼ�ķ������������ػ�������ɫֵ�����յĽ�������忴������������պС�
��Ϊ�����Ѿ��ڳ��������ú���պ��ˣ���������Ч����������ѡ����ǽ���ı����ӵ�Ƭ����ɫ�����������з����ԣ�
	#version 330 core
	out vec4 FragColor;
	in vec3 Normal;
	in vec3 Position;
	uniform vec3 cameraPos;
	uniform samplerCube skybox;
	void main()
	{
		vec3 I = normalize(Position - cameraPos);
		vec3 R = reflect(I, normalize(Normal));
		FragColor = vec4(texture(skybox, R).rgb, 1.0);
	}
�����ȼ����˹۲�/�������������I����ʹ���������㷴������R��֮�����ǽ�ʹ��R������պ���������ͼ�в�����ע�⣬��������������Ƭ�εĲ�ֵNormal��Position����������������Ҫ����һ�¶�����ɫ����
	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aNormal;
	out vec3 Normal;
	out vec3 Position;
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
	void main()
	{
		Normal = mat3(transpose(inverse(model))) * aNormal;
		Position = vec3(model * vec4(aPos, 1.0));
		gl_Position = projection * view * model * vec4(aPos, 1.0);
	}
��������ʹ����һ�����������������ǽ��ٴ�ʹ�÷��߾���(Normal Matrix)���任���ǡ�Position���������һ������ռ��λ��������������ɫ�������Position�����������Ƭ����ɫ���ڼ���۲췽��������
��Ϊ����ʹ���˷��ߣ��㻹��Ҫ����һ�¶������ݣ�����������ָ�롣��Ҫ�ǵ�ȥ����cameraPos���uniform��
������ʵ�д󲿷ֵ�ģ�Ͷ���������ȫ�����ԡ����ǿ������뷴����ͼ(Reflection Map)������ģ�͸����ϸ�ڡ���������;������ͼһ����������ͼҲ�ǿ��Բ���������ͼ����������Ƭ�εķ����ԡ�ͨ��ʹ�÷�����ͼ�����ǿ���֪��ģ�͵���Щ���ָ���ʲôǿ����ʾ���䡣

����
����ӳ�����һ����ʽ�����䣬���ͷ�������ơ������ǹ������ڴ������ʵĸı�������ķ���仯���ڳ�������ˮ������������������������䣬���߲���ֱֱ�ش���������������һ�㡣����İ�ֻ�첲���ˮ��۲�����ľ�������Ч����
������ͨ��˹��������(Snell��s Law)��������
ͬ����������һ���۲�����I��һ��������N�����������������R���Կ������۲������ķ�����΢�����ˡ����ۺ������R������������������ͼ�в�����
�������ʹ��GLSL���ڽ�refract����������ʵ�֣�����Ҫһ����������һ���۲췽�����������֮���������(Refractive Index)��
�����ʾ����˲����й��������ĳ̶ȣ�ÿ�����ʶ����Լ��������ʡ�һЩ����������ʿ������±����ҵ���
	����	������
	����	1.00
	ˮ		1.33
	��		1.309
	����	1.52
	��ʯ	2.42
����ʹ����Щ������������⴫�������ֲ��ʼ�ı�ֵ�������ǵ������У�����/���ߴӿ������벣����������Ǽ��������ǲ����Ƶģ������Ա�ֵΪ1.00/1.52=0.658��
�����Ѿ�������������ͼ���ṩ�˶������ݺͷ��ߣ��������������λ�õ�uniform��ΨһҪ�޸ĵľ���Ƭ����ɫ����
	void main()
	{
		float ratio = 1.00 / 1.52;
		vec3 I = normalize(Position - cameraPos);
		vec3 R = refract(I, normalize(Normal), ratio);
		FragColor = vec4(texture(skybox, R).rgb, 1.0);
	}
�������������˹��ա����䡢����Ͷ����ƶ�����ȷ��ϣ�����Դ������ǳ�Ư����ˮ��ע�⣬���Ҫ���������Ͼ�ȷ�Ľ�������ǻ���Ҫ�ڹ����뿪�����ʱ���ٴ����䣬��������ʹ�õ�ֻ�ǵ�������(Single-side Refraction)�������Դ󲿷ֳ��϶���û����ġ�

��̬������ͼ
��������ʹ�õĶ��Ǿ�̬ͼ����������Ϊ��պУ��������ܲ�������û���ڳ����а������ƶ������塣����һֱ��û��ע�⵽��һ�㣬��Ϊ����ֻʹ����һ�����塣���������һ������һ�������壬��Χ���ж�����壬�����пɼ���ֻ����պУ��������������ǳ�����Ψһһ������һ����
ͨ��ʹ��֡���壬�����ܹ�Ϊ�����6����ͬ�Ƕȴ�������������������ÿ����Ⱦ�����н����Ǵ��浽һ����������ͼ�С�֮�����ǾͿ���ʹ���������̬���ɵģ���������ͼ������������ʵ�ģ�������������ģ��������������ˡ���ͽ�����̬����ӳ��(Dynamic Environment Mapping)����Ϊ���Ƕ�̬������������Χ����������ͼ������������������ͼ��
��Ȼ���������ܰ���������һ���ܴ��ȱ�㣺������ҪΪʹ�û�����ͼ��������Ⱦ����6�Σ����ǶԳ����Ƿǳ�������ܿ������ִ��ĳ���ͨ���ᾡ����ʹ����պУ����ڿ��ܵ�ʱ��ʹ��Ԥ�������������ͼ��ֻҪ�����ܲ���һ�㶯̬������ͼ��Ч������Ȼ��̬������ͼ��һ���ܰ��ļ���������Ҫ���ڲ��������ܵ��������������������Ҫ�ǳ���ļ��ɵġ�
Assimp�ڴ������ʽ�ж���̫ϲ��������ͼ������������Ҫ��ƭһ��������������ͼ����Ϊ��������ͼ��������ڼ��ز��ʵ�ʱ�򽫷�����ͼ��������������ΪaiTextureType_AMBIENT��
����ģ�ͼ�����������Ѿ�����ɫ����ռ����3������Ԫ�ˣ�����Ҫ����պа󶨵���4������Ԫ�ϣ���Ϊ����Ҫ��ͬһ����ɫ���ж���պв�����



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
//��ͨ���Ӷ�������
//float cubeVertices[] = {
//	// Back face
//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
//	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
//	// Front face
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
//	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
//	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
//	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
//	// Left face
//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
//	// Right face
//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
//	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
//	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
//	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
//	 // Bottom face
//	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
//	  0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
//	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
//	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
//	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
//	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
//	 // Top face
//	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
//	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
//	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
//	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
//	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
//	 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
//};

//�������Ӷ�������
float cubeVertices[] = {
	// positions          // normals
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

float planeVertices[] = {
	// positions          // texture Coords
	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
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
	//stbi_set_flip_vertically_on_load(true);
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

unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
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
	Shader shader("res/Shaders/Shader_advancedOpenGL/4/cubemaps.vert", "res/Shaders/Shader_advancedOpenGL/4/cubemaps.frag");
	Shader skyboxShader("res/Shaders/Shader_advancedOpenGL/4/skybox.vert", "res/Shaders/Shader_advancedOpenGL/4/skybox.frag");

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
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glBindVertexArray(0);
	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// screen quad VAO

	unsigned int cubeTexture = load_texture("res/textures/container.png");
	std::vector<std::string> faces
	{
		"res/skybox/right.jpg",
		"res/skybox/left.jpg",
		"res/skybox/top.jpg",
		"res/skybox/bottom.jpg",
		"res/skybox/front.jpg",
		"res/skybox/back.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);

	shader.use();
	shader.setInt("texture1", 0);

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

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
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw scene as normal
		shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setVec3("cameraPos", camera.Position);		// cubes
		//glBindVertexArray(cubeVAO);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, cubeTexture);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);
		// cubes
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default
		
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
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &skyboxVBO);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}