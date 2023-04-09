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
GLSL内建函数
radians(x)：角度转弧度
degrees(x)：弧度转角度
sin(x)：正弦函数，传入值为弧度。相同的还有cos余弦函数、tan正切函数、asin反正弦、acos反余弦、atan反正切
pow(x,y)：xy
exp(x)：ex
exp2(x)：2x
log(x)：logex
log2(x)：log2x
sqrt(x)：根号x
inversesqr(x)：1根号x
abs(x)：取x的绝对值
sign(x)：x>0返回1.0，x<0返回-1.0，否则返回0.0
ceil(x)：返回大于或者等于x的整数
floor(x)：返回小于或者等于x的整数
fract(x)：返回x-floor(x)的值
mod(x,y)：取模（求余）
min(x,y)：获取xy中小的那个
max(x,y)：获取xy中大的那个
mix(x,y,a)：返回x∗(1−a)+y∗a
step(x,a)：x< a返回0.0，否则返回1.0
smoothstep(x,y,a)：a < x返回0.0，a>y返回1.0，否则返回0.0-1.0之间平滑的Hermite插值。
dFdx(p)：p在x方向上的偏导数
dFdy(p)：p在y方向上的偏导数
fwidth(p)：p在x和y方向上的偏导数的绝对值之和
几何函数
length(x)：计算向量x的长度
distance(x,y)：返回向量xy之间的距离
dot(x,y)：返回向量xy的点积
cross(x,y)：返回向量xy的差积
normalize(x)：返回与x向量方向相同，长度为1的向量
矩阵函数
matrixCompMult(x,y)：将矩阵相乘
lessThan(x,y)：返回向量xy的各个分量执行x< y的结果，类似的有greaterThan,equal,notEqual
lessThanEqual(x,y)：返回向量xy的各个分量执行x<= y的结果，类似的有类似的有greaterThanEqual
any(bvec x)：x有一个元素为true，则为true
all(bvec x)：x所有元素为true，则返回true，否则返回false
not(bvec x)：x所有分量执行逻辑非运算
纹理采样函数
纹理采样函数有texture2D、texture2DProj、texture2DLod、texture2DProjLod、textureCube、textureCubeLod及texture3D、texture3DProj、texture3DLod、texture3DProjLod等。

高级数据
我们在OpenGL中大量使用缓冲来储存数据已经有很长时间了。操作缓冲其实还有更有意思的方式，而且使用纹理将大量数据传入着色器也有更有趣的方法。这一节中，我们将讨论一些更有意思的缓冲函数，以及我们该如何使用纹理对象来储存大量的数据
OpenGL中的缓冲只是一个管理特定内存块的对象，没有其它更多的功能了。在我们将它绑定到一个缓冲目标(Buffer Target)时，我们才赋予了其意义。当我们绑定一个缓冲到GL_ARRAY_BUFFER时，它就是一个顶点数组缓冲，但我们也可以很容易地将其绑定到GL_ELEMENT_ARRAY_BUFFER。OpenGL内部会为每个目标储存一个缓冲，并且会根据目标的不同，以不同的方式处理缓冲。
到目前为止，我们一直是调用glBufferData函数来填充缓冲对象所管理的内存，这个函数会分配一块内存，并将数据添加到这块内存中。如果我们将它的data参数设置为NULL，那么这个函数将只会分配内存，但不进行填充。这在我们需要预留(Reserve)特定大小的内存，之后回到这个缓冲一点一点填充的时候会很有用。
除了使用一次函数调用填充整个缓冲之外，我们也可以使用glBufferSubData，填充缓冲的特定区域。这个函数需要一个缓冲目标、一个偏移量、数据的大小和数据本身作为它的参数。这个函数不同的地方在于，我们可以提供一个偏移量，指定从何处开始填充这个缓冲。这能够让我们插入或者更新缓冲内存的某一部分。要注意的是，缓冲需要有足够的已分配内存，所以对一个缓冲调用glBufferSubData之前必须要先调用glBufferData。
	glBufferSubData(GL_ARRAY_BUFFER, 24, sizeof(data), &data); // 范围： [24, 24 + sizeof(data)]
将数据导入缓冲的另外一种方法是，请求缓冲内存的指针，直接将数据复制到缓冲当中。通过调用glMapBuffer函数，OpenGL会返回当前绑定缓冲的内存指针，供我们操作：
	float data[] = {
	  0.5f, 1.0f, -0.35f
	  ...
	};
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// 获取指针
	void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	// 复制数据到内存
	memcpy(ptr, data, sizeof(data));
	// 记得告诉OpenGL我们不再需要这个指针了
	glUnmapBuffer(GL_ARRAY_BUFFER);
当我们使用glUnmapBuffer函数，告诉OpenGL我们已经完成指针操作之后，OpenGL就会知道你已经完成了。在解除映射(Unmapping)之后，指针将会不再可用，并且如果OpenGL能够成功将您的数据映射到缓冲中，这个函数将会返回GL_TRUE。
如果要直接映射数据到缓冲，而不事先将其存储到临时内存中，glMapBuffer这个函数会很有用。比如说，你可以从文件中读取数据，并直接将它们复制到缓冲内存中。

分批顶点属性
通过使用glVertexAttribPointer，我们能够指定顶点数组缓冲内容的属性布局。在顶点数组缓冲中，我们对属性进行了交错(Interleave)处理，也就是说，我们将每一个顶点的位置、法线和/或纹理坐标紧密放置在一起。既然我们现在已经对缓冲有了更多的了解，我们可以采取另一种方式。
我们可以做的是，将每一种属性类型的向量数据打包(Batch)为一个大的区块，而不是对它们进行交错储存。与交错布局123123123123不同，我们将采用分批(Batched)的方式111122223333。
当从文件中加载顶点数据的时候，你通常获取到的是一个位置数组、一个法线数组和/或一个纹理坐标数组。我们需要花点力气才能将这些数组转化为一个大的交错数据数组。使用分批的方式会是更简单的解决方案，我们可以很容易使用glBufferSubData函数实现：
	float positions[] = { ... };
	float normals[] = { ... };
	float tex[] = { ... };
	// 填充缓冲
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), &positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(normals), &normals);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(normals), sizeof(tex), &tex);
这样子我们就能直接将属性数组作为一个整体传递给缓冲，而不需要事先处理它们了。我们仍可以将它们合并为一个大的数组，再使用glBufferData来填充缓冲，但对于这种工作，使用glBufferSubData会更合适一点。
我们还需要更新顶点属性指针来反映这些改变：
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(positions)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(positions) + sizeof(normals)));
注意stride参数等于顶点属性的大小，因为下一个顶点属性向量能在3个（或2个）分量之后找到。

复制缓冲
当你的缓冲已经填充好数据之后，你可能会想与其它的缓冲共享其中的数据，或者想要将缓冲的内容复制到另一个缓冲当中。glCopyBufferSubData能够让我们相对容易地从一个缓冲中复制数据到另一个缓冲中。这个函数的原型如下：
void glCopyBufferSubData(GLenum readtarget, GLenum writetarget, GLintptr readoffset,
						 GLintptr writeoffset, GLsizeiptr size);
readtarget和writetarget参数需要填入复制源和复制目标的缓冲目标。比如说，我们可以将VERTEX_ARRAY_BUFFER缓冲复制到VERTEX_ELEMENT_ARRAY_BUFFER缓冲，分别将这些缓冲目标设置为读和写的目标。当前绑定到这些缓冲目标的缓冲将会被影响到。
但如果我们想读写数据的两个不同缓冲都为顶点数组缓冲该怎么办呢？我们不能同时将两个缓冲绑定到同一个缓冲目标上。正是出于这个原因，OpenGL提供给我们另外两个缓冲目标，叫做GL_COPY_READ_BUFFER和GL_COPY_WRITE_BUFFER。我们接下来就可以将需要的缓冲绑定到这两个缓冲目标上，并将这两个目标作为readtarget和writetarget参数。
接下来glCopyBufferSubData会从readtarget中读取size大小的数据，并将其写入writetarget缓冲的writeoffset偏移量处。下面这个例子展示了如何复制两个顶点数组缓冲：
	float vertexData[] = { ... };
	glBindBuffer(GL_COPY_READ_BUFFER, vbo1);
	glBindBuffer(GL_COPY_WRITE_BUFFER, vbo2);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(vertexData));
我们也可以只将writetarget缓冲绑定为新的缓冲目标类型之一：
	float vertexData[] = { ... };
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBindBuffer(GL_COPY_WRITE_BUFFER, vbo2);
	glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(vertexData));
有了这些关于如何操作缓冲的额外知识，我们已经能够以更有意思的方式使用它们了。当你越深入OpenGL时，这些新的缓冲方法将会变得更加有用。在下一节中，在我们讨论Uniform缓冲对象(Uniform Buffer Object)时，我们将会充分利用glBufferSubData。

高级GLSL
GLSL的内建变量
	https://www.khronos.org/opengl/wiki/Built-in_Variable_(GLSL)
着色器都是最简化的，如果需要当前着色器以外地方的数据的话，我们必须要将数据传进来。我们已经学会使用顶点属性、uniform和采样器来完成这一任务了。
然而，除此之外，GLSL还定义了另外几个以gl_为前缀的变量，它们能提供给我们更多的方式来读取/写入数据。
我们已经在前面教程中接触过其中的两个了：顶点着色器的输出向量gl_Position，和片段着色器的gl_FragCoord。

顶点着色器变量
	我们已经见过gl_Position了，它是顶点着色器的裁剪空间输出位置向量。如果你想在屏幕上显示任何东西，在顶点着色器中设置gl_Position是必须的步骤。这已经是它的全部功能了。
gl_PointSize
	我们能够选用的其中一个图元是GL_POINTS，如果使用它的话，每一个顶点都是一个图元，都会被渲染为一个点。我们可以通过OpenGL的glPointSize函数来设置渲染出来的点的大小，但我们也可以在顶点着色器中修改这个值。
	GLSL定义了一个叫做gl_PointSize输出变量，它是一个float变量，你可以使用它来设置点的宽高（像素）。在顶点着色器中修改点的大小的话，你就能对每个顶点设置不同的值了。
	在顶点着色器中修改点大小的功能默认是禁用的，如果你需要启用它的话，你需要启用OpenGL的GL_PROGRAM_POINT_SIZE：
		glEnable(GL_PROGRAM_POINT_SIZE);
	一个简单的例子就是将点的大小设置为裁剪空间位置的z值，也就是顶点距观察者的距离。点的大小会随着观察者距顶点距离变远而增大。
		void main()
		{
			gl_Position = projection * view * model * vec4(aPos, 1.0);
			gl_PointSize = gl_Position.z;
		}
	你可以想到，对每个顶点使用不同的点大小，会在粒子生成之类的技术中很有意思。
gl_VertexID
	gl_Position和gl_PointSize都是输出变量，因为它们的值是作为顶点着色器的输出被读取的。我们可以对它们进行写入，来改变结果。顶点着色器还为我们提供了一个有趣的输入变量，我们只能对它进行读取，它叫做gl_VertexID。
	整型变量gl_VertexID储存了正在绘制顶点的当前ID。当（使用glDrawElements）进行索引渲染的时候，这个变量会存储正在绘制顶点的当前索引。当（使用glDrawArrays）不使用索引进行绘制的时候，这个变量会储存从渲染调用开始的已处理顶点数量。

片段着色器变量
	在片段着色器中，我们也能访问到一些有趣的变量。GLSL提供给我们两个有趣的输入变量：gl_FragCoord和gl_FrontFacing。
gl_FragCoord
	在讨论深度测试的时候，我们已经见过gl_FragCoord很多次了，因为gl_FragCoord的z分量等于对应片段的深度值。然而，我们也能使用它的x和y分量来实现一些有趣的效果。
	gl_FragCoord的x和y分量是片段的窗口空间(Window-space)坐标，其原点为窗口的左下角。我们已经使用glViewport设定了一个800x600的窗口了，所以片段窗口空间坐标的x分量将在0到800之间，y分量在0到600之间。
	通过利用片段着色器，我们可以根据片段的窗口坐标，计算出不同的颜色。gl_FragCoord的一个常见用处是用于对比不同片段计算的视觉输出效果，这在技术演示中可以经常看到。
	比如说，我们能够将屏幕分成两部分，在窗口的左侧渲染一种输出，在窗口的右侧渲染另一种输出。下面这个例子片段着色器会根据窗口坐标输出不同的颜色：
		void main()
		{
			if(gl_FragCoord.x < 400)
				FragColor = vec4(1.0, 0.0, 0.0, 1.0);
			else
				FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		}
	因为窗口的宽度是800。当一个像素的x坐标小于400时，它一定在窗口的左侧，所以我们给它一个不同的颜色。
gl_FrontFacing
	片段着色器另外一个很有意思的输入变量是gl_FrontFacing。在面剔除教程中，我们提到OpenGL能够根据顶点的环绕顺序来决定一个面是正向还是背向面。如果我们不（启用GL_FACE_CULL来）使用面剔除，那么gl_FrontFacing将会告诉我们当前片段是属于正向面的一部分还是背向面的一部分。举例来说，我们能够对正向面计算出不同的颜色。
	gl_FrontFacing变量是一个bool，如果当前片段是正向面的一部分那么就是true，否则就是false。比如说，我们可以这样子创建一个立方体，在内部和外部使用不同的纹理：
		#version 330 core
		out vec4 FragColor;
		in vec2 TexCoords;
		uniform sampler2D frontTexture;
		uniform sampler2D backTexture;
		void main()
		{
			if(gl_FrontFacing)
				FragColor = texture(frontTexture, TexCoords);
			else
				FragColor = texture(backTexture, TexCoords);
		}
	如果我们往箱子里面看，就能看到使用的是不同的纹理。
	if(!gl_FrontFacing)
		discard;
	注意，如果你开启了面剔除，你就看不到箱子内部的面了，所以现在再使用gl_FrontFacing就没有意义了。
gl_FragDepth
	输入变量gl_FragCoord能让我们读取当前片段的窗口空间坐标，并获取它的深度值，但是它是一个只读(Read-only)变量。我们不能修改片段的窗口空间坐标，但实际上修改片段的深度值还是可能的。GLSL提供给我们一个叫做gl_FragDepth的输出变量，我们可以使用它来在着色器内设置片段的深度值。
	要想设置深度值，我们直接写入一个0.0到1.0之间的float值到输出变量就可以了：
		gl_FragDepth = 0.0; // 这个片段现在的深度值为 0.0
	如果着色器没有写入值到gl_FragDepth，它会自动取用gl_FragCoord.z的值。
	然而，由我们自己设置深度值有一个很大的缺点，只要我们在片段着色器中对gl_FragDepth进行写入，OpenGL就会（像深度测试小节中讨论的那样）禁用所有的提前深度测试(Early Depth Testing)。
	它被禁用的原因是，OpenGL无法在片段着色器运行之前得知片段将拥有的深度值，因为片段着色器可能会完全修改这个深度值。
	在写入gl_FragDepth时，你就需要考虑到它所带来的性能影响。然而，从OpenGL 4.2起，我们仍可以对两者进行一定的调和，在片段着色器的顶部使用深度条件(Depth Condition)重新声明gl_FragDepth变量：
		layout (depth_<condition>) out float gl_FragDepth;
	condition可以为下面的值：
		条件		描述
		any			默认值。提前深度测试是禁用的，你会损失很多性能
		greater		你只能让深度值比gl_FragCoord.z更大
		less			你只能让深度值比gl_FragCoord.z更小
		unchanged	如果你要写入gl_FragDepth，你将只能写入gl_FragCoord.z的值
	通过将深度条件设置为greater或者less，OpenGL就能假设你只会写入比当前片段深度值更大或者更小的值了。这样子的话，当深度值比片段的深度值要小的时候，OpenGL仍是能够进行提前深度测试的。
	下面这个例子中，我们对片段的深度值进行了递增，但仍然也保留了一些提前深度测试：
		#version 420 core // 注意GLSL的版本！
		out vec4 FragColor;
		layout (depth_greater) out float gl_FragDepth;
		void main()
		{
			FragColor = vec4(1.0);
			gl_FragDepth = gl_FragCoord.z + 0.1;
		}
	注意这个特性只在OpenGL 4.2版本或以上才提供。
接口块
	到目前为止，每当我们希望从顶点着色器向片段着色器发送数据时，我们都声明了几个对应的输入/输出变量。
	将它们一个一个声明是着色器间发送数据最简单的方式了，但当程序变得更大时，你希望发送的可能就不只是几个变量了，它还可能包括数组和结构体。
	为了帮助我们管理这些变量，GLSL为我们提供了一个叫做接口块(Interface Block)的东西，来方便我们组合这些变量。接口块的声明和struct的声明有点相像，不同的是，现在根据它是一个输入还是输出块(Block)，使用in或out关键字来定义的。
		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec2 aTexCoords;
		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;
		out VS_OUT
		{
			vec2 TexCoords;
		} vs_out;
		void main()
		{
			gl_Position = projection * view * model * vec4(aPos, 1.0);
			vs_out.TexCoords = aTexCoords;
		}
	这次我们声明了一个叫做vs_out的接口块，它打包了我们希望发送到下一个着色器中的所有输出变量。这只是一个很简单的例子，但你可以想象一下，它能够帮助你管理着色器的输入和输出。
	当我们希望将着色器的输入或输出打包为数组时，它也会非常有用，我们将在下一节讨论几何着色器(Geometry Shader)时见到。
	之后，我们还需要在下一个着色器，即片段着色器，中定义一个输入接口块。块名(Block Name)应该是和着色器中一样的（VS_OUT），但实例名(Instance Name)（顶点着色器中用的是vs_out）可以是随意的，但要避免使用误导性的名称，比如对实际上包含输入变量的接口块命名为vs_out。
		#version 330 core
		out vec4 FragColor;
		in VS_OUT
		{
			vec2 TexCoords;
		} fs_in;
		uniform sampler2D texture;
		void main()
		{
			FragColor = texture(texture, fs_in.TexCoords);
		}
	只要两个接口块的名字一样，它们对应的输入和输出将会匹配起来。这是帮助你管理代码的又一个有用特性，它在几何着色器这样穿插特定着色器阶段的场景下会很有用。
Uniform缓冲对象
	OpenGL为我们提供了一个叫做Uniform缓冲对象(Uniform Buffer Object)的工具，它允许我们定义一系列在多个着色器中相同的全局Uniform变量。当使用Uniform缓冲对象的时候，我们只需要设置相关的uniform一次。当然，我们仍需要手动设置每个着色器中不同的uniform。并且创建和配置Uniform缓冲对象会有一点繁琐。
	因为Uniform缓冲对象仍是一个缓冲，我们可以使用glGenBuffers来创建它，将它绑定到GL_UNIFORM_BUFFER缓冲目标，并将所有相关的uniform数据存入缓冲。在Uniform缓冲对象中储存数据是有一些规则的，我们将会在之后讨论它。首先，我们将使用一个简单的顶点着色器，将projection和view矩阵存储到所谓的Uniform块(Uniform Block)中：
		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (std140) uniform Matrices
		{
			mat4 projection;
			mat4 view;
		};
		uniform mat4 model;
		void main()
		{
			gl_Position = projection * view * model * vec4(aPos, 1.0);
		}
	在我们大多数的例子中，我们都会在每个渲染迭代中，对每个着色器设置projection和view Uniform矩阵。这是利用Uniform缓冲对象的一个非常完美的例子，因为现在我们只需要存储这些矩阵一次就可以了。
	这里，我们声明了一个叫做Matrices的Uniform块，它储存了两个4x4矩阵。Uniform块中的变量可以直接访问，不需要加块名作为前缀。
	你现在可能会在想layout (std140)这个语句是什么意思。它的意思是说，当前定义的Uniform块对它的内容使用一个特定的内存布局。这个语句设置了Uniform块布局(Uniform Block Layout)。
Uniform块布局
Uniform块的内容是储存在一个缓冲对象中的，它实际上只是一块预留内存。因为这块内存并不会保存它具体保存的是什么类型的数据，我们还需要告诉OpenGL内存的哪一部分对应着着色器中的哪一个uniform变量。
假设着色器中有以下的这个Uniform块：
	layout (std140) uniform ExampleBlock
	{
		float value;
		vec3  vector;
		mat4  matrix;
		float values[3];
		bool  boolean;
		int   integer;
	};
我们需要知道的是每个变量的大小（字节）和（从块起始位置的）偏移量，来让我们能够按顺序将它们放进缓冲中。每个元素的大小都是在OpenGL中有清楚地声明的，而且直接对应C++数据类型，其中向量和矩阵都是大的float数组。OpenGL没有声明的是这些变量间的间距(Spacing)。这允许硬件能够在它认为合适的位置放置变量。
比如说，一些硬件可能会将一个vec3放置在float边上。不是所有的硬件都能这样处理，可能会在附加这个float之前，先将vec3填充(Pad)为一个4个float的数组。这个特性本身很棒，但是会对我们造成麻烦。
默认情况下，GLSL会使用一个叫做共享(Shared)布局的Uniform内存布局，共享是因为一旦硬件定义了偏移量，它们在多个程序中是共享并一致的。使用共享布局时，GLSL是可以为了优化而对uniform变量的位置进行变动的，只要变量的顺序保持不变。因为我们无法知道每个uniform变量的偏移量，我们也就不知道如何准确地填充我们的Uniform缓冲了。我们能够使用像是glGetUniformIndices这样的函数来查询这个信息
虽然共享布局给了我们很多节省空间的优化，但是我们需要查询每个uniform变量的偏移量，这会产生非常多的工作量。通常的做法是，不使用共享布局，而是使用std140布局。std140布局声明了每个变量的偏移量都是由一系列规则所决定的，这显式地声明了每个变量类型的内存布局。由于这是显式提及的，我们可以手动计算出每个变量的偏移量。
每个变量都有一个基准对齐量(Base Alignment)，它等于一个变量在Uniform块中所占据的空间（包括填充量(Padding)），这个基准对齐量是使用std140布局的规则计算出来的。接下来，对每个变量，我们再计算它的对齐偏移量(Aligned Offset)，它是一个变量从块起始位置的字节偏移量。一个变量的对齐字节偏移量必须等于基准对齐量的倍数。
	类型					布局规则
	标量，比如int和bool	每个标量的基准对齐量为N。
	向量					2N或者4N。这意味着vec3的基准对齐量为4N。
	标量或向量的数组		每个元素的基准对齐量与vec4的相同。
	矩阵					储存为列向量的数组，每个向量的基准对齐量与vec4的相同。
	结构体					等于所有元素根据规则计算后的大小，但会填充到vec4大小的倍数。
和OpenGL大多数的规范一样，使用例子就能更容易地理解。我们会使用之前引入的那个叫做ExampleBlock的Uniform块，并使用std140布局计算出每个成员的对齐偏移量：
	layout (std140) uniform ExampleBlock
	{
						 // 基准对齐量       // 对齐偏移量
		float value;     // 4               // 0
		vec3 vector;     // 16              // 16  (必须是16的倍数，所以 4->16)
		mat4 matrix;     // 16              // 32  (列 0)
						 // 16              // 48  (列 1)
						 // 16              // 64  (列 2)
						 // 16              // 80  (列 3)
		float values[3]; // 16              // 96  (values[0])
						 // 16              // 112 (values[1])
						 // 16              // 128 (values[2])
		bool boolean;    // 4               // 144
		int integer;     // 4               // 148
	};
使用计算后的偏移量值，根据std140布局的规则，我们就能使用像是glBufferSubData的函数将变量数据按照偏移量填充进缓冲中了。虽然std140布局不是最高效的布局，但它保证了内存布局在每个声明了这个Uniform块的程序中是一致的。
通过在Uniform块定义之前添加layout (std140)语句，我们告诉OpenGL这个Uniform块使用的是std140布局。除此之外还可以选择两个布局，但它们都需要我们在填充缓冲之前先查询每个偏移量。我们已经见过shared布局了，剩下的一个布局是packed。当使用紧凑(Packed)布局时，是不能保证这个布局在每个程序中保持不变的（即非共享），因为它允许编译器去将uniform变量从Uniform块中优化掉，这在每个着色器中都可能是不同的。

使用Uniform缓冲
首先，我们需要调用glGenBuffers，创建一个Uniform缓冲对象。一旦我们有了一个缓冲对象，我们需要将它绑定到GL_UNIFORM_BUFFER目标，并调用glBufferData，分配足够的内存。
	unsigned int uboExampleBlock;
	glGenBuffers(1, &uboExampleBlock);
	glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
	glBufferData(GL_UNIFORM_BUFFER, 152, NULL, GL_STATIC_DRAW); // 分配152字节的内存
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
现在，每当我们需要对缓冲更新或者插入数据，我们都会绑定到uboExampleBlock，并使用glBufferSubData来更新它的内存。我们只需要更新这个Uniform缓冲一次，所有使用这个缓冲的着色器就都使用的是更新后的数据了。但是，如何才能让OpenGL知道哪个Uniform缓冲对应的是哪个Uniform块呢？
在OpenGL上下文中，定义了一些绑定点(Binding Point)，我们可以将一个Uniform缓冲链接至它。在创建Uniform缓冲之后，我们将它绑定到其中一个绑定点上，并将着色器中的Uniform块绑定到相同的绑定点，把它们连接到一起。下面的这个图示展示了这个：
你可以看到，我们可以绑定多个Uniform缓冲到不同的绑定点上。因为着色器A和着色器B都有一个链接到绑定点0的Uniform块，它们的Uniform块将会共享相同的uniform数据，uboMatrices，前提条件是两个着色器都定义了相同的Matrices Uniform块。
为了将Uniform块绑定到一个特定的绑定点中，我们需要调用glUniformBlockBinding函数，它的第一个参数是一个程序对象，之后是一个Uniform块索引和链接到的绑定点。Uniform块索引(Uniform Block Index)是着色器中已定义Uniform块的位置值索引。这可以通过调用glGetUniformBlockIndex来获取，它接受一个程序对象和Uniform块的名称。我们可以用以下方式将图示中的Lights Uniform块链接到绑定点2：
	unsigned int lights_index = glGetUniformBlockIndex(shaderA.ID, "Lights");
	glUniformBlockBinding(shaderA.ID, lights_index, 2);
注意我们需要对每个着色器重复这一步骤。
从OpenGL 4.2版本起，你也可以添加一个布局标识符，显式地将Uniform块的绑定点储存在着色器中，这样就不用再调用glGetUniformBlockIndex和glUniformBlockBinding了。下面的代码显式地设置了Lights Uniform块的绑定点。
	layout(std140, binding = 2) uniform Lights { ... };
接下来，我们还需要绑定Uniform缓冲对象到相同的绑定点上，这可以使用glBindBufferBase或glBindBufferRange来完成。
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboExampleBlock);
	// 或
	glBindBufferRange(GL_UNIFORM_BUFFER, 2, uboExampleBlock, 0, 152);
glBindbufferBase需要一个目标，一个绑定点索引和一个Uniform缓冲对象作为它的参数。这个函数将uboExampleBlock链接到绑定点2上，自此，绑定点的两端都链接上了。
你也可以使用glBindBufferRange函数，它需要一个附加的偏移量和大小参数，这样子你可以绑定Uniform缓冲的特定一部分到绑定点中。通过使用glBindBufferRange函数，你可以让多个不同的Uniform块绑定到同一个Uniform缓冲对象上。

我们可以开始向Uniform缓冲中添加数据了。只要我们需要，就可以使用glBufferSubData函数，用一个字节数组添加所有的数据，或者更新缓冲的一部分。要想更新uniform变量boolean，我们可以用以下方式更新Uniform缓冲对象：
	glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
	int b = true; // GLSL中的bool是4字节的，所以我们将它存为一个integer
	glBufferSubData(GL_UNIFORM_BUFFER, 144, 4, &b);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
同样的步骤也能应用到Uniform块中其它的uniform变量上，但需要使用不同的范围参数。

一个简单的例子
所以，我们来展示一个真正使用Uniform缓冲对象的例子。如果我们回头看看之前所有的代码例子，我们不断地在使用3个矩阵：投影、观察和模型矩阵。在所有的这些矩阵中，只有模型矩阵会频繁变动。如果我们有多个着色器使用了这同一组矩阵，那么使用Uniform缓冲对象可能会更好。
我们会将投影和模型矩阵存储到一个叫做Matrices的Uniform块中。我们不会将模型矩阵存在这里，因为模型矩阵在不同的着色器中会不断改变，所以使用Uniform缓冲对象并不会带来什么好处。
首先，我们将顶点着色器的Uniform块设置为绑定点0。注意我们需要对每个着色器都设置一遍。
	unsigned int uniformBlockIndexRed    = glGetUniformBlockIndex(shaderRed.ID, "Matrices");
	unsigned int uniformBlockIndexGreen  = glGetUniformBlockIndex(shaderGreen.ID, "Matrices");
	unsigned int uniformBlockIndexBlue   = glGetUniformBlockIndex(shaderBlue.ID, "Matrices");
	unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(shaderYellow.ID, "Matrices");
	glUniformBlockBinding(shaderRed.ID,    uniformBlockIndexRed, 0);
	glUniformBlockBinding(shaderGreen.ID,  uniformBlockIndexGreen, 0);
	glUniformBlockBinding(shaderBlue.ID,   uniformBlockIndexBlue, 0);
	glUniformBlockBinding(shaderYellow.ID, uniformBlockIndexYellow, 0);
接下来，我们创建Uniform缓冲对象本身，并将其绑定到绑定点0：
	unsigned int uboMatrices
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

这只是一个很简单的情景，我们可能会需要使用Uniform缓冲对象，但任何大型的渲染程序都可能同时激活有上百个着色器程序，这时候Uniform缓冲对象的优势就会很大地体现出来了。
Uniform缓冲对象比起独立的uniform有很多好处。第一，一次设置很多uniform会比一个一个设置多个uniform要快很多。
第二，比起在多个着色器中修改同样的uniform，在Uniform缓冲中修改一次会更容易一些。最后一个好处可能不会立即显现，如果使用Uniform缓冲对象的话，你可以在着色器中使用更多的uniform。
OpenGL限制了它能够处理的uniform数量，这可以通过GL_MAX_VERTEX_UNIFORM_COMPONENTS来查询。当使用Uniform缓冲对象时，最大的数量会更高。
所以，当你达到了uniform的最大数量时（比如再做骨骼动画(Skeletal Animation)的时候），你总是可以选择使用Uniform缓冲对象。



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

//普通箱子顶点坐标
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
	glfwSetCursorPosCallback(window, mouse_cursor_posotion);
	glfwSetScrollCallback(window, mouse_scroll_posotion);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD." << std::endl;
	}

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	Shader shaderRed("res/Shaders/Shader_advancedOpenGL/5/advanced_glsl.vert", "res/Shaders/Shader_advancedOpenGL/5/red.frag");
	Shader shaderGreen("res/Shaders/Shader_advancedOpenGL/5/advanced_glsl.vert", "res/Shaders/Shader_advancedOpenGL/5/green.frag");
	Shader shaderBlue("res/Shaders/Shader_advancedOpenGL/5/advanced_glsl.vert", "res/Shaders/Shader_advancedOpenGL/5/blue.frag");
	Shader shaderYellow("res/Shaders/Shader_advancedOpenGL/5/advanced_glsl.vert", "res/Shaders/Shader_advancedOpenGL/5/yellow.frag");

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// configure a uniform buffer object
	// first. We get the relevant block indices
	unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(shaderRed.ID, "Matrices");
	unsigned int uniformBlockIndexGreen = glGetUniformBlockIndex(shaderGreen.ID, "Matrices");
	unsigned int uniformBlockIndexBlue = glGetUniformBlockIndex(shaderBlue.ID, "Matrices");
	unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(shaderYellow.ID, "Matrices");
	// then we link each shader's uniform block to this uniform binding point
	glUniformBlockBinding(shaderRed.ID, uniformBlockIndexRed, 0);
	glUniformBlockBinding(shaderGreen.ID, uniformBlockIndexGreen, 0);
	glUniformBlockBinding(shaderBlue.ID, uniformBlockIndexBlue, 0);
	glUniformBlockBinding(shaderYellow.ID, uniformBlockIndexYellow, 0);
	// Now actually create the buffer
	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	// define the range of the buffer that links to a uniform binding point
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

	// store the projection matrix (we only do this once now) (note: we're not using zoom anymore by changing the FoV)
	glm::mat4 projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

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

		// set the view and projection matrix in the uniform block - we only have to do this once per loop iteration.
		glm::mat4 view = camera.GetViewMatrix();
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// draw 4 cubes 
		// RED
		glBindVertexArray(cubeVAO);
		shaderRed.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f)); // move top-left
		shaderRed.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// GREEN
		shaderGreen.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f)); // move top-right
		shaderGreen.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// YELLOW
		shaderYellow.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f)); // move bottom-left
		shaderYellow.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// BLUE
		shaderBlue.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f)); // move bottom-right
		shaderBlue.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
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
	glDeleteBuffers(1, &cubeVBO);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}