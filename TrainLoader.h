#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "res/GL_heads/Camera.hpp"
#include "res/GL_heads/Shader.h"
#include "SimuCore.h"

/*
* ImGui要点注意：相邻两个button组件的名称参数要不一样，否则若相邻按钮的名称一样，只能触发第一个按钮
* 只有两个 ImGui::Button("Press") 对象中的第一个会注册的原因是 - 默认情况下 - ImGui 使用按钮的文本作为其标识符。
* 可以通过在按钮周围使用 PushID() 和 PopID() 来创建新的 ID 堆栈，或者通过在按钮文本后添加“##”来向按钮添加其他信息来规避此问题。
* 例如： ImGui::Button("Press##1") 和 ImGui::Button("Press##2") 都将注册，而两者都只有“按下”按钮。
* “##”之后的所有文本将仅用于设置该按钮的标识符，不会显示。
* 
* ImGui控件的（一般）第一个const文本参数至少要为" "，不能为""空，因为其内容会作为ID，若为空""会出现野指针

颜色显示逻辑：舱室为空时显示白色，舱室为满时显示货物颜色

BC33对应17和18道装车楼A楼，位于左边，18道在最左边 x = 3630 y = 1325
BC15对应15和16道装车楼B楼，位于右边，15道在最右边 x = 3900 y = 1175
18, 17, 16, 15道

直接计算北端和南端的浮点值，到vertShader中，若y值不在范围内，将state值设为-1，在fragShader中若state<-0.9就丢弃片段
train_coords[]按28节车皮上限计算，超过当前列车长度的车皮对应state设为-1

Type_train
1碎石		2水渣		3工业盐		4纸浆		5黄砂		6铜矿		7石油焦			8焦煤		9化工煤		10混煤		12优混		13无烟煤	14沥青
1大混		2蒙混		3大同		4大精		5陕煤		6俄罗斯煤	7俄罗斯无烟	8兴煤
1新中港	2神华精		3伊泰		4天马		5特底灰		6神混		7神优			8优一		9神华		10富动

*/

struct Type_train
{
	int type_index;			//货物小类编号，具体到货物的名称，从1开始计数
	int type_type;			//货物大类编号，1为大类命名，2为来源命名，3为企业命名
	std::string type_name;	//堆货名称
};

struct BuildingAttrib
{
	int building_state;				//装车楼运行状态，-1灰色系统离线（停机状态），0红色未上电或故障，1绿色运行
	unsigned int building_time;	//装车楼总计运行时间
	int building_pow;				//装车楼运行功率
	int building_mode;				//装车模式切换，0先进先出FIFO，1后进先出LIFO，2左（16和18道），3右（15和17道）
};

struct One_wagon
{
	int wagon_idx;		//车皮编号（从1开始）
	float amount;		//车皮载货量
};

struct TrainAttrib
{
	int train_idx;					//火车编号
	float speed_train;				//火车移速
	int train_type;					//火车运货大类（0表示未定义）
	int train_index;				//火车运货小类（0表示未定义）
	int train_lane;					//火车车道，0为15道，1为16道，2为17道，3为18道
	int train_state;				//火车状态，-1（顶点着色器的本地变量）超范围（discard），0等待装货（红色），1正在装货（每节车皮，空载白色，满载黑色），2正在离港（蓝色）
	int total_num;					//火车车皮数：A楼最多28节，B楼最多24节
	int current_num;				//当前装车车皮序号：从1加到total_num，初始值为1
	float first_pos;					//第一节车皮的y位置
	float max_per_wagon;			//每车皮满载装煤量：62-67吨
	std::vector<One_wagon> wagons;	//火车载煤值
	float train_coords[7];			//火车坐标：x,y,z,state当前状态,percent存储量百分比，focus获得焦点（未到港时x,y都为0），type货物种类
};

class TrainLoader
{
public:
	TrainLoader();
	void reset(SimuCore& core, bool rand_init);		//重置
	void train_random_initiator(SimuCore& core);	//火车随机初始化
	void drawBuilding(Camera& camera, Shader& buildingShader);
	void drawTrain(Camera& camera, Shader& trainShader);
	void initGuiStyle();							//样式初始化
	int train_dispatch();						//ImGui火车调度控制台
	int updateTrains(float simurate);			//火车状态
	void add_type(std::string str_name, int type_type);					//添加货物新类
	bool run(std::vector<std::string>& equipments, int type, int index);	//负载运行，返回true操作成功（双道都有车优先装早进港等待的车），返回false车道上没有车，无法启动
	void shutDown(std::vector<std::string>& equipments);					//流程停止
	void set_focus(std::vector<std::string>& equipments);					//选中流程的显示
	void lose_focus();														//失焦后的流程显示

private:
	ImGuiStyle* style;						//ImGui样式存储
	Type_train type_unit;					//货物单元，包含货物种类序号及其名称
	std::vector<Type_train> names;			//货物种类编号与名称存储容器
	bool lane_occupied[4];					//车道占用标识
	int index_occupied[4];					//列车编号占用标识
	float north_thresh;						//列车显示北限
	float south_thresh;						//列车显示南限
	float train_x_bias;						//列车装车楼水平车道偏移
	float length_wagon;						//单车皮长度
	BuildingAttrib building_A;				//17 18道BC33装车A楼
	BuildingAttrib building_B;				//15 16道BC15装车B楼
	float building_coords_A[6];				//装车楼A坐标：x,y,z,state,focus,mode（取值0-3）
	float building_coords_B[6];				//装车楼B坐标：x,y,z,state,focus,mode（取值0-3）
	unsigned int buildingVBO, buildingVAO;
	One_wagon wagon;
	TrainAttrib train;
	std::vector<TrainAttrib> trains;			//列车存储容器
	unsigned int trainVBO, trainVAO;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	void pre_button();
	void post_button();
	void set_button(int index, bool building);

};
