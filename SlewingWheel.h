#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "res/GL_heads/Camera.hpp"
#include "res/GL_heads/Shader.h"

/*
���ֻ�����Բ�ѣ�800KVA��400KW
*/

struct WheelAttrib
{
	std::string wheel_name;		//���ֻ�����
	int wheel_idx;				//���ֻ����
	int wheel_state;			//���ֻ�����״̬��-1��ɫϵͳ���ߣ�ͣ��״̬����0��ɫδ�ϵ����ϣ�1��ɫ����
	bool wheel_mode;			//���ֻ�����ģʽ��0�������У�1ȡ������
};

class SlewingWheel
{
	friend class Record;
public:
	SlewingWheel();
	void reset();												//����
	void draw(Camera& camera, Shader& wheelShader);
	bool startCheck(std::vector<std::string> equipments);		//��鴫����豸���������Ƿ񶼿��У�����falseΪ��ֹ����������trueΪ��������
	void run(std::vector<std::string> equipments, bool mode);	//��������
	void shutDown(std::vector<std::string> equipments);			//����ֹͣ
	void set_focus(std::vector<std::string>& equipments);		//ѡ�����̵���ʾ
	void lose_focus();											//ʧ�����������ʾ

private:
	WheelAttrib wheel_attr;				//���ֻ�
	float wheel_coords[56];				//���ֻ����꣺x,y,z,r,g,b,focus
	std::vector<WheelAttrib> wheels;	//���ֻ�״̬�洢����
	unsigned int wheelVBO, wheelVAO;	//VAO & VBO
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

};
