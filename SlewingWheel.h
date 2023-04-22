#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "res/GL_heads/Camera.hpp"
#include "res/GL_heads/Shader.h"

/*
斗轮机（非圆堆）800KVA，400KW
*/

struct WheelAttrib
{
	std::string wheel_name;		//斗轮机代号
	int wheel_idx;				//斗轮机序号
	int wheel_state;			//斗轮机运行状态，-1灰色系统离线（停机状态），0红色未上电或故障，1绿色运行
	bool wheel_mode;			//斗轮机运行模式，0堆料运行，1取料运行
};

class SlewingWheel
{
	friend class Record;
public:
	SlewingWheel();
	void reset();												//重置
	void draw(Camera& camera, Shader& wheelShader);
	bool startCheck(std::vector<std::string> equipments);		//检查传入的设备代号容器是否都空闲，返回false为禁止启动，返回true为允许启动
	void run(std::vector<std::string> equipments, bool mode);	//负载运行
	void shutDown(std::vector<std::string> equipments);			//流程停止
	void set_focus(std::vector<std::string>& equipments);		//选中流程的显示
	void lose_focus();											//失焦后的流程显示

private:
	WheelAttrib wheel_attr;				//斗轮机
	float wheel_coords[56];				//斗轮机坐标：x,y,z,r,g,b,focus
	std::vector<WheelAttrib> wheels;	//斗轮机状态存储容器
	unsigned int wheelVBO, wheelVAO;	//VAO & VBO
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

};
