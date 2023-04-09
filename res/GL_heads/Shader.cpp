#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	geometryShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		//打开文件
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);
		//将文件的缓冲区内容读入流
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();
		//关闭文件处理程序
		vertexShaderFile.close();
		fragmentShaderFile.close();
		//将流转换为字符串
		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();
		if (geometryPath != nullptr)
		{
			geometryShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure& e)		//有异常被try中catch后就报错
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << e.what() << std::endl;
		glfwTerminate();
		return;
	}
	vertexShaderData = vertexCode.c_str();
	fragmentShaderData = fragmentCode.c_str();
	geometryShaderData = geometryCode.c_str();
	//2.编译着色器
	int success;
	char infoLog[512];
	//顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);	//创建顶点着色器
	glShaderSource(vertex, 1, &vertexShaderData, NULL);	//绑定源码
	glCompileShader(vertex);						//编译源码
	//错误检查
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//片段着色器类似
	fragment = glCreateShader(GL_FRAGMENT_SHADER);	//创建片段着色器
	glShaderSource(fragment, 1, &fragmentShaderData, NULL);	//绑定源码
	glCompileShader(fragment);							//编译源码
	//错误检查
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//几何着色器
	if (geometryPath != nullptr)
	{
		const char* geometryShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geometryShaderCode, NULL);
		glCompileShader(geometry);
		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(geometry, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	//着色器程序
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	if (geometryPath != nullptr)
		glAttachShader(ID, geometry);	glLinkProgram(ID);
	//错误检查
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//删除着色器，因为它们已经连接到着色器程序（渲染管线）中，不再需要
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);
}

void Shader::use()
{
	glUseProgram(ID);
}
