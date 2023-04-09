#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	geometryShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		//���ļ�
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);
		//���ļ��Ļ��������ݶ�����
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();
		//�ر��ļ��������
		vertexShaderFile.close();
		fragmentShaderFile.close();
		//����ת��Ϊ�ַ���
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
	catch (std::ifstream::failure& e)		//���쳣��try��catch��ͱ���
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << e.what() << std::endl;
		glfwTerminate();
		return;
	}
	vertexShaderData = vertexCode.c_str();
	fragmentShaderData = fragmentCode.c_str();
	geometryShaderData = geometryCode.c_str();
	//2.������ɫ��
	int success;
	char infoLog[512];
	//������ɫ��
	vertex = glCreateShader(GL_VERTEX_SHADER);	//����������ɫ��
	glShaderSource(vertex, 1, &vertexShaderData, NULL);	//��Դ��
	glCompileShader(vertex);						//����Դ��
	//������
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//Ƭ����ɫ������
	fragment = glCreateShader(GL_FRAGMENT_SHADER);	//����Ƭ����ɫ��
	glShaderSource(fragment, 1, &fragmentShaderData, NULL);	//��Դ��
	glCompileShader(fragment);							//����Դ��
	//������
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//������ɫ��
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
	//��ɫ������
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	if (geometryPath != nullptr)
		glAttachShader(ID, geometry);	glLinkProgram(ID);
	//������
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//ɾ����ɫ������Ϊ�����Ѿ����ӵ���ɫ��������Ⱦ���ߣ��У�������Ҫ
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);
}

void Shader::use()
{
	glUseProgram(ID);
}
