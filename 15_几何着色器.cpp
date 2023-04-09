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

#define SCR_WIDTH 1000
#define SCR_HEIGHT 800
#define GLSL_VERSION "#version 330\n"

/*
在顶点和片段着色器之间有一个可选的几何着色器(Geometry Shader)，几何着色器的输入是一个图元（如点或三角形）的一组顶点。几何着色器可以在顶点发送到下一着色器阶段之前对它们随意变换。然而，几何着色器最有趣的地方在于，它能够将（这一组）顶点变换为完全不同的图元，并且还能生成比原来更多的顶点。
我们直接先看一个几何着色器的例子：
	#version 330 core
	layout (points) in;
	layout (line_strip, max_vertices = 2) out;

	void main() {
		gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
		EmitVertex();

		gl_Position = gl_in[0].gl_Position + vec4( 0.1, 0.0, 0.0, 0.0);
		EmitVertex();

		EndPrimitive();
	}
在几何着色器的顶部，我们需要声明从顶点着色器输入的图元类型。这需要在in关键字前声明一个布局修饰符(Layout Qualifier)。这个输入布局修饰符可以从顶点着色器接收下列任何一个图元值：
	points：绘制GL_POINTS图元时（1）。
	lines：绘制GL_LINES或GL_LINE_STRIP时（2）
	lines_adjacency：GL_LINES_ADJACENCY或GL_LINE_STRIP_ADJACENCY（4）
	triangles：GL_TRIANGLES、GL_TRIANGLE_STRIP或GL_TRIANGLE_FAN（3）
	triangles_adjacency：GL_TRIANGLES_ADJACENCY或GL_TRIANGLE_STRIP_ADJACENCY（6）
以上是能提供给glDrawArrays渲染函数的几乎所有图元了。如果我们想要将顶点绘制为GL_TRIANGLES，我们就要将输入修饰符设置为triangles。括号内的数字表示的是一个图元所包含的最小顶点数。
接下来，我们还需要指定几何着色器输出的图元类型，这需要在out关键字前面加一个布局修饰符。和输入布局修饰符一样，输出布局修饰符也可以接受几个图元值：
	points
	line_strip
	triangle_strip
有了这3个输出修饰符，我们就可以使用输入图元创建几乎任意的形状了。要生成一个三角形的话，我们将输出定义为triangle_strip，并输出3个顶点。
几何着色器同时希望我们设置一个它最大能够输出的顶点数量（如果你超过了这个值，OpenGL将不会绘制多出的顶点），这个也可以在out关键字的布局修饰符中设置。在这个例子中，我们将输出一个line_strip，并将最大顶点数设置为2个。
如果你不知道什么是线条(Line Strip)：线条连接了一组点，形成一条连续的线，它最少要由两个点来组成。在渲染函数中每多加一个点，就会在这个点与前一个点之间形成一条新的线。
如果使用的是上面定义的着色器，那么这将只能输出一条线段，因为最大顶点数等于2。

为了生成更有意义的结果，我们需要某种方式来获取前一着色器阶段的输出。GLSL提供给我们一个内建(Built-in)变量，在内部看起来（可能）是这样的：
	in gl_Vertex
	{
		vec4  gl_Position;
		float gl_PointSize;
		float gl_ClipDistance[];
	} gl_in[];
这里，它被声明为一个接口块（Interface Block，我们在上一节已经讨论过），它包含了几个很有意思的变量，其中最有趣的一个是gl_Position，它是和顶点着色器输出非常相似的一个向量。
要注意的是，它被声明为一个数组，因为大多数的渲染图元包含多于1个的顶点，而几何着色器的输入是一个图元的所有顶点。
有了之前顶点着色器阶段的顶点数据，我们就可以使用2个几何着色器函数，EmitVertex和EndPrimitive，来生成新的数据了。几何着色器希望你能够生成并输出至少一个定义为输出的图元。在我们的例子中，我们需要至少生成一个线条图元。
	gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4( 0.1, 0.0, 0.0, 0.0);
	EmitVertex();
	EndPrimitive();
每次我们调用EmitVertex时，gl_Position中的向量会被添加到图元中来。当EndPrimitive被调用时，所有发射出的(Emitted)顶点都会合成为指定的输出渲染图元。在一个或多个EmitVertex调用之后重复调用EndPrimitive能够生成多个图元。
在这个例子中，我们发射了两个顶点，它们从原始顶点位置平移了一段距离，之后调用了EndPrimitive，将这两个顶点合成为一个包含两个顶点的线条。
它接受一个点图元作为输入，以这个点为中心，创建一条水平的线图元。
这个输出是通过调用下面的渲染函数来生成的
	glDrawArrays(GL_POINTS, 0, 4);

使用几何着色器
我们只会在标准化设备坐标的z平面上绘制四个点。这些点的坐标是：
	float points[] = {
		-0.5f,  0.5f, // 左上
		 0.5f,  0.5f, // 右上
		 0.5f, -0.5f, // 右下
		-0.5f, -0.5f  // 左下
	};
顶点着色器只需要在z平面绘制点就可以了，所以我们将使用一个最基本顶点着色器：
	#version 330 core
	layout (location = 0) in vec2 aPos;
	void main()
	{
		gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
	}
直接在片段着色器中硬编码，将所有的点都输出为绿色：
	#version 330 core
	out vec4 FragColor;
	void main()
	{
		FragColor = vec4(0.0, 1.0, 0.0, 1.0);
	}
为点的顶点数据生成一个VAO和一个VBO，然后使用glDrawArrays进行绘制：
	shader.use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, 4);
结果是在黑暗的场景中有四个（很难看见的）绿点

我们将会创建一个传递(Pass-through)几何着色器，它会接收一个点图元，并直接将它传递(Pass)到下一个着色器：
	#version 330 core
	layout (points) in;
	layout (points, max_vertices = 1) out;
	void main() {
		gl_Position = gl_in[0].gl_Position;
		EmitVertex();
		EndPrimitive();
	}
现在这个几何着色器应该很容易理解了，它只是将它接收到的顶点位置不作修改直接发射出去，并生成一个点图元。
和顶点与片段着色器一样，几何着色器也需要编译和链接，但这次在创建着色器时我们将会使用GL_GEOMETRY_SHADER作为着色器类型：
	geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometryShader, 1, &gShaderCode, NULL);
	glCompileShader(geometryShader);
	...
	glAttachShader(program, geometryShader);
	glLinkProgram(program);
着色器编译的代码和顶点与片段着色器代码都是一样的。记得要检查编译和链接错误！

绘制五边形
我们可以将几何着色器的输出设置为triangle_strip，并绘制三个三角形
OpenGL中，三角形带(Triangle Strip)是绘制三角形更高效的方式，它使用顶点更少。在第一个三角形绘制完之后，每个后续顶点将会在上一个三角形边上生成另一个三角形：每3个临近的顶点将会形成一个三角形。
变为几何着色器是这样的：
	#version 330 core
	layout (points) in;
	layout (triangle_strip, max_vertices = 5) out;
	void build_house(vec4 position)
	{
		gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:左下
		EmitVertex();
		gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:右下
		EmitVertex();
		gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:左上
		EmitVertex();
		gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:右上
		EmitVertex();
		gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:顶部
		EmitVertex();
		EndPrimitive();
	}
	void main() {
		build_house(gl_in[0].gl_Position);
	}
理解方式：绘制模式为triangle_strip，即用上一个绘制完的三角形的邻近边加上下一个顶点绘制下一个三角形，
几何着色器中第1,2,3顶点的位移数据获取后开始绘制第一个三角形（左下角的三角形），第四个顶点获取后在第一个三角形的右上角绘制第二个三角形（2,3点的边），
最后获取顶点5，与3,4边绘制第三个三角形

这个几何着色器生成了5个顶点，每个顶点都是原始点的位置加上一个偏移量，来组成一个大的三角形带。最终的图元会被光栅化，然后片段着色器会处理整个三角形带，最终在每个绘制的点处生成一个绿色房子
你可以看到，每个房子实际上是由3个三角形组成的——全部都是使用空间中一点来绘制的。这些绿房子看起来是有点无聊，所以我们会再给每个房子分配一个不同的颜色。为了实现这个，我们需要在顶点着色器中添加一个额外的顶点属性，表示颜色信息，将它传递至几何着色器，并再次发送到片段着色器中。

下面是更新后的顶点数据：
	float points[] = {
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // 左上
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // 右上
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 右下
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // 左下
	};
然后我们更新顶点着色器，使用一个接口块将颜色属性发送到几何着色器中：
	#version 330 core
	layout (location = 0) in vec2 aPos;
	layout (location = 1) in vec3 aColor;
	out VS_OUT {
		vec3 color;
	} vs_out;
	void main()
	{
		gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
		vs_out.color = aColor;
	}
因为几何着色器是作用于输入的一组顶点的，从顶点着色器发来输入数据总是会以数组的形式表示出来，即便我们现在只有一个顶点。
我们并不是必须要用接口块来向几何着色器传递数据。如果顶点着色器发送的颜色向量是out vec3 vColor，我们也可以这样写：
	in vec3 vColor[];
然而，接口块在几何着色器这样的着色器中会更容易处理一点。实际上，几何着色器的输入能够变得非常大，将它们合并为一个大的接口块数组会更符合逻辑一点。

接下来我们还需要为下个片段着色器阶段声明一个输出颜色向量：
	out vec3 fColor;
因为片段着色器只需要一个（插值的）颜色，发送多个颜色并没有什么意义。所以，fColor向量就不是一个数组，而是一个单独的向量。当发射一个顶点的时候，每个顶点将会使用最后在fColor中储存的值，来用于片段着色器的运行。
对我们的房子来说，我们只需要在第一个顶点发射之前，使用顶点着色器中的颜色填充fColor一次就可以了。
仅仅是为了有趣，我们也可以假装这是冬天，将最后一个顶点的颜色设置为白色，给屋顶落上一些雪。
	fColor = gs_in[0].color;
	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:左下
	EmitVertex();
	gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:右下
	EmitVertex();
	gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:左上
	EmitVertex();
	gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:右上
	EmitVertex();
	gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:顶部
	fColor = vec3(1.0, 1.0, 1.0);
	EmitVertex();
	EndPrimitive();

爆破物体
当我们说爆破一个物体时，我们并不是指要将宝贵的顶点集给炸掉，我们是要将每个三角形沿着法向量的方向移动一小段时间。效果就是，整个物体看起来像是沿着每个三角形的法线向量爆炸一样。
这样的几何着色器效果的一个好处就是，无论物体有多复杂，它都能够应用上去。

因为我们想要沿着三角形的法向量位移每个顶点，我们首先需要计算这个法向量。我们所要做的是计算垂直于三角形表面的向量，仅使用我们能够访问的3个顶点。你可能还记得在变换小节中，我们使用叉乘来获取垂直于其它两个向量的一个向量。如果我们能够获取两个平行于三角形表面的向量a和b，我们就能够对这两个向量进行叉乘来获取法向量了。下面这个几何着色器函数做的正是这个，来使用3个输入顶点坐标来获取法向量：
	vec3 GetNormal()
	{
	   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	   return normalize(cross(a, b));
	}
这里我们使用减法获取了两个平行于三角形表面的向量a和b。因为两个向量相减能够得到这两个向量之间的差值，并且三个点都位于三角平面上，对任意两个向量相减都能够得到一个平行于平面的向量。注意，如果我们交换了cross函数中a和b的位置，我们会得到一个指向相反方向的法向量——这里的顺序很重要！
既然知道了如何计算法向量了，我们就能够创建一个explode函数了，它使用法向量和顶点位置向量作为参数。这个函数会返回一个新的向量，它是位置向量沿着法线向量进行位移之后的结果：
	vec4 explode(vec4 position, vec3 normal)
	{
		float magnitude = 2.0;
		vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
		return position + vec4(direction, 0.0);
	}
函数本身应该不是非常复杂。sin函数接收一个time参数，它根据时间返回一个-1.0到1.0之间的值。因为我们不想让物体向内爆炸(Implode)，我们将sin值变换到了[0, 1]的范围内。最终的结果会乘以normal向量，并且最终的direction向量会被加到位置向量上。

当使用我们的模型加载器绘制一个模型时，爆破(Explode)效果的完整几何着色器是这样的：
	#version 330 core
	layout (triangles) in;
	layout (triangle_strip, max_vertices = 3) out;
	in VS_OUT {
		vec2 texCoords;
	} gs_in[];
	out vec2 TexCoords;
	uniform float time;

	vec4 explode(vec4 position, vec3 normal) { ... }
	vec3 GetNormal() { ... }

	void main() {
		vec3 normal = GetNormal();
		gl_Position = explode(gl_in[0].gl_Position, normal);
		TexCoords = gs_in[0].texCoords;
		EmitVertex();
		gl_Position = explode(gl_in[1].gl_Position, normal);
		TexCoords = gs_in[1].texCoords;
		EmitVertex();
		gl_Position = explode(gl_in[2].gl_Position, normal);
		TexCoords = gs_in[2].texCoords;
		EmitVertex();
		EndPrimitive();
	}

法向量可视化
在这一部分中，我们将使用几何着色器来实现一个真正有用的例子：显示任意物体的法向量。当编写光照着色器时，你可能会最终会得到一些奇怪的视觉输出，但又很难确定导致问题的原因。光照错误很常见的原因就是法向量错误，这可能是由于不正确加载顶点数据、错误地将它们定义为顶点属性或在着色器中不正确地管理所导致的。
我们想要的是使用某种方式来检测提供的法向量是正确的。检测法向量是否正确的一个很好的方式就是对它们进行可视化，几何着色器正是实现这一目的非常有用的工具。
思路是这样的：我们首先不使用几何着色器正常绘制场景。然后再次绘制场景，但这次只显示通过几何着色器生成法向量。几何着色器接收一个三角形图元，并沿着法向量生成三条线——每个顶点一个法向量。伪代码看起来会像是这样：
	shader.use();
	DrawScene();
	normalDisplayShader.use();
	DrawScene();
这次在几何着色器中，我们会使用模型提供的顶点法线，而不是自己生成，为了适配（观察和模型矩阵的）缩放和旋转，我们在将法线变换到观察空间坐标之前，先使用法线矩阵变换一次（几何着色器接受的位置向量是观察空间坐标，所以我们应该将法向量变换到相同的空间中）。
这可以在顶点着色器中完成：
	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aNormal;

	out VS_OUT {
		vec3 normal;
	} vs_out;

	uniform mat4 view;
	uniform mat4 model;

	void main()
	{
		gl_Position = view * model * vec4(aPos, 1.0);
		mat3 normalMatrix = mat3(transpose(inverse(view * model)));
		vs_out.normal = normalize(vec3(vec4(normalMatrix * aNormal, 0.0)));
	}
变换后的观察空间法向量会以接口块的形式传递到下个着色器阶段。接下来，几何着色器会接收每一个顶点（包括一个位置向量和一个法向量），并在每个位置向量处绘制一个法线向量：
	#version 330 core
	layout (triangles) in;
	layout (line_strip, max_vertices = 6) out;
	in VS_OUT {
		vec3 normal;
	} gs_in[];
	const float MAGNITUDE = 0.4;
	uniform mat4 projection;
	void GenerateLine(int index)
	{
		gl_Position = projection * gl_in[index].gl_Position;
		EmitVertex();
		gl_Position = projection * (gl_in[index].gl_Position +
									vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
		EmitVertex();
		EndPrimitive();
	}
	void main()
	{
		GenerateLine(0); // 第一个顶点法线
		GenerateLine(1); // 第二个顶点法线
		GenerateLine(2); // 第三个顶点法线
	}
像这样的几何着色器应该很容易理解了。注意我们将法向量乘以了一个MAGNITUDE向量，来限制显示出的法向量大小（否则它们就有点大了）。
因为法线的可视化通常都是用于调试目的，我们可以使用片段着色器，将它们显示为单色的线（如果你愿意也可以是非常好看的线）

你可以想象到，这样的几何着色器也经常用于给物体添加毛发(Fur)。


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

float cubeVertices[] = {
	// positions         
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
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

	//只在按下鼠标右键时允许变换视角
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
	//glfwSetCursorPosCallback(window, mouse_cursor_posotion);
	//glfwSetScrollCallback(window, mouse_scroll_posotion);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD." << std::endl;
	}

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	Shader shader("res/Shaders/Shader_advancedOpenGL/6/几何着色器.vert", "res/Shaders/Shader_advancedOpenGL/6/几何着色器.frag", "res/Shaders/Shader_advancedOpenGL/6/几何着色器.geom");

	float points[] = {
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
	};

	/*imGUI*/
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	ImGui::StyleColorsDark();

	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);

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
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// draw points
		shader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, 4);

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

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}