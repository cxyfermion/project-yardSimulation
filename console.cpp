#include "console.h"

#define SCR_WIDTH 1440
#define SCR_HEIGHT 810
#define GLSL_VERSION "#version 330\n"

Camera camera(glm::vec3(0.0f, 0.0f, 9.0f));
static bool firstMouse = true;
static float lastX = 800.0f / 2.0;
static float lastY = 600.0f / 2.0;

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

Console::Console()
{
	this->random_initiating = false;
	this->compitence = 12;
	std::string temp_1[13] = { u8"游客", u8"卸船机驾驶员", u8"船舶调度员", u8"火车调度员", u8"火车船舶总调度员", u8"流程管理员", u8"仿真人员", u8"堆场管理员", u8"筒仓管理员", u8"变电所管理员", u8"超级管理员", u8"气象员", u8"上帝" };
	std::string temp_2[13] = { u8"仅有观光权限", u8"拥有卸船机的控制权限", u8"拥有船舶的调度权限", u8"拥有火车的调度权限", u8"拥有火车和船舶的调度权限", u8"拥有流程系统控制权限", u8"拥有仿真系统控制权限", u8"拥有堆场系统控制权限", u8"拥有筒仓系统控制权限", u8"拥有变电站系统控制权限", u8"拥有系统顶级控制权限", u8"拥有控制环境权限", u8"全知全能" };
	for (int i = 0; i < 13; i++)
	{
		this->names[i] = temp_1[i];
		this->descriptions[i] = temp_2[i];
	}
}

int Console::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	this->window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Yard Simulation", NULL, NULL);
	if (this->window == NULL)
	{
		std::cout << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(this->window);
	glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
	glfwSetCursorPosCallback(this->window, mouse_cursor_posotion);
	//glfwSetScrollCallback(this->window, mouse_scroll_posotion);
	//以下语句用于不显示鼠标
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD." << std::endl;
		return 0;
	}
	return 1;
}

void Console::run()
{
	std::unique_ptr<Conveyor> conv(new Conveyor);
	std::unique_ptr<SlewingWheel> wheel(new SlewingWheel);
	std::unique_ptr<Flow> flow(new Flow);
	std::unique_ptr<Berth> berth(new Berth);
	std::unique_ptr<TrainLoader> train(new TrainLoader);
	std::unique_ptr<Yard> yard(new Yard);
	std::unique_ptr<Silo> silo(new Silo);
	std::unique_ptr<SimuCore> simucore(new SimuCore);
	std::unique_ptr<Environment> env(new Environment);
	std::unique_ptr<Energy> energy(new Energy);
	std::unique_ptr<Message> message(new Message);
	std::unique_ptr<Web> web(new Web);
	std::unique_ptr<Record> record(new Record);

	//Conveyor conv;
	//SlewingWheel wheel;
	//Flow flow;
	//Berth berth;
	//TrainLoader train;
	//Yard yard;
	//Silo silo;
	//SimuCore simucore;
	//Environment env;
	//Energy energy;
	//Message message;
	//Web web;
	//Record record;

	/*Shader*/
	Shader convShader("res/Shaders/Shader_for_2D/shader_conv_2d.vert", "res/Shaders/Shader_for_2D/shader_conv_2d.frag", "res/Shaders/Shader_for_2D/shader_conv_2d.geom");
	Shader wheelShader("res/Shaders/Shader_for_2D/shader_wheel_2d.vert", "res/Shaders/Shader_for_2D/shader_wheel_2d.frag", "res/Shaders/Shader_for_2D/shader_wheel_2d.geom");
	Shader woodShader("res/Shaders/Shader_for_2D/cubemaps.vert", "res/Shaders/Shader_for_2D/cubemaps_wood.frag");
	Shader glassShader("res/Shaders/Shader_for_2D/cubemaps.vert", "res/Shaders/Shader_for_2D/cubemaps_glass.frag");
	Shader skyboxShader("res/Shaders/Shader_for_2D/skybox.vert", "res/Shaders/Shader_for_2D/skybox.frag");
	Shader unloaderShader("res/Shaders/Shader_for_2D/shader_unloader_2d.vert", "res/Shaders/Shader_for_2D/shader_unloader_2d.frag", "res/Shaders/Shader_for_2D/shader_unloader_2d.geom");
	Shader bucketShader("res/Shaders/Shader_for_2D/shader_bucket_2d.vert", "res/Shaders/Shader_for_2D/shader_bucket_2d.frag", "res/Shaders/Shader_for_2D/shader_bucket_2d.geom");
	Shader shipShader("res/Shaders/Shader_for_2D/shader_ship_2d.vert", "res/Shaders/Shader_for_2D/shader_ship_2d.frag", "res/Shaders/Shader_for_2D/shader_ship_2d.geom");
	Shader buildingShader("res/Shaders/Shader_for_2D/shader_building_2d.vert", "res/Shaders/Shader_for_2D/shader_building_2d.frag", "res/Shaders/Shader_for_2D/shader_building_2d.geom");
	Shader trainShader("res/Shaders/Shader_for_2D/shader_train_2d.vert", "res/Shaders/Shader_for_2D/shader_train_2d.frag", "res/Shaders/Shader_for_2D/shader_train_2d.geom");
	Shader yardShader("res/Shaders/Shader_for_2D/shader_yard_2d.vert", "res/Shaders/Shader_for_2D/shader_yard_2d.frag", "res/Shaders/Shader_for_2D/shader_yard_2d.geom");
	Shader siloShader("res/Shaders/Shader_for_2D/shader_silo_2d.vert", "res/Shaders/Shader_for_2D/shader_silo_2d.frag", "res/Shaders/Shader_for_2D/shader_silo_2d.geom");
	Shader fragsShader("res/Shaders/Shader_for_2D/shader_frags_2d.vert", "res/Shaders/Shader_for_2D/shader_frags_2d.frag", "res/Shaders/Shader_for_2D/shader_frags_2d.geom");

	//状态初始化
	if (this->random_initiating)
	{
		berth->ship_random_initiator(*simucore);
		train->train_random_initiator(*simucore);
		yard->yard_random_initiator(*simucore);
		silo->silo_random_initiator(*simucore);
	}

	//深度效果显示
	glEnable(GL_DEPTH_TEST);
	//imGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	ImGui::StyleColorsDark();
	//启用中文字体，但会大幅提升CPU占用率
	ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyh.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
	conv->initGuiStyle();
	flow->initGuiStyle();
	berth->initGuiStyle();
	train->initGuiStyle();
	yard->initGuiStyle();
	silo->initGuiStyle();
	simucore->initGuiStyle();
	energy->initGuiStyle();
	message->initGuiStyle();
	web->initGuiStyle();
	record->initGuiStyle();
	//背景
	env->createEnv(woodShader, glassShader, skyboxShader);

	while (!glfwWindowShouldClose(window))
	{
		env->processInput(camera, window);
		int end_train_1 = false;
		int end_train_2 = false;
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//更新状态
		simucore->updateTime();
		env->updateEnv();
		flow->weatherCheck(*message, *env, *energy, *conv, *wheel, *berth, *train, *yard, *silo, *web);
		if ((!simucore->simu_pause && simucore->freshRequire) || simucore->stepping)
		{
			conv->updateConvs((float)simucore->freshGapTime, simucore->run_rate, web->convAmount);
			berth->updateBuckets((float)simucore->freshGapTime, simucore->run_rate);
			if (berth->berth_finished != -1)
			{
				//船舶卸空结束
				flow->end_shipunloading(*message, *energy, berth->berth_finished, *conv, *wheel, *berth, *yard);
			}
			int ret_ship = berth->updateShips((float)simucore->freshGapTime, simucore->run_rate);
			if (ret_ship == 1)
			{
				//装船装满结束
				flow->end_shiploading(*energy, *berth, *web);
			}
			end_train_1 = train->updateTrains((float)simucore->freshGapTime, simucore->run_rate);
			if (yard->updateYards((float)simucore->freshGapTime, simucore->run_rate))
			{
				flow->stop_yard_flow(*energy, yard->terminate_wheel, *berth, *train, *silo, *web);
			}
			silo->updateStraight((float)simucore->freshGapTime, simucore->run_rate);
			if (silo->updateSilos((float)simucore->freshGapTime, simucore->run_rate))
			{
				flow->stop_silo_flow(*energy, *yard, *web);
			}
			web->update(*simucore, (float)simucore->freshGapTime, simucore->run_rate);
			if (web->finishEndName != "NULL")
			{
				flow->end_web(*conv, *wheel, web->finishEndName);
			}
			energy->update(*message, simucore->run_rate, (float)simucore->freshGapTime);
			if (energy->trip != -1)
			{
				//变压器跳闸
				flow->trip_end(energy->trip == 0, energy->getEquipments(), *conv, *wheel, *berth, *train, *yard, *silo, *web);
			}
			//刷新复位
			if (simucore->freshRequire)
			{
				simucore->freshRequire = false;
			}
			//步进修正
			if (simucore->stepping)
			{
				simucore->stepping = false;
			}
		}
		//重置
		if (simucore->reset_rand || simucore->reset_zero)
		{
			conv->reset();
			wheel->reset();
			flow->reset();
			berth->reset(*simucore, simucore->reset_rand);
			train->reset(*simucore, simucore->reset_rand);
			yard->reset(*simucore, simucore->reset_rand);
			silo->reset(*simucore, simucore->reset_rand);
			simucore->reset_rand = false;
			simucore->reset_zero = false;
			energy->reset();
			message->reset();
			web->reset();
		}
		//绘制图像
		conv->draw(camera, convShader, 0.3f * std::sin((float)simucore->time * 2) + 0.7f);
		wheel->draw(camera, wheelShader);
		berth->drawUnloader(camera, unloaderShader, bucketShader);
		berth->drawLoader(camera, unloaderShader);
		berth->drawShip(camera, shipShader);
		train->drawBuilding(camera, buildingShader);
		train->drawTrain(camera, trainShader);
		yard->drawYard(camera, yardShader);
		silo->drawSilo(camera, siloShader);
		web->drawFrags(camera, fragsShader);
		//start new frame for imGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//控制面板窗口
		ImGui::Begin(u8"控制面板");
		simucore->base_info();
		this->identity_choose();
		if (this->compitence == 12)
		{
			if (ImGui::CollapsingHeader(u8"存储管理"))
			{
				ImGui::Indent();
				int ret = record->showGui();
				if (ret > 0)
				{
					record->save_level(ret, *message, this->random_initiating, this->compitence, *simucore, *energy, *env, *web, *flow, *conv, *wheel, *berth, *train, *yard, *silo);
				}
				else if (ret < 0)
				{
					record->load_level(-ret, *message, this->random_initiating, this->compitence, *simucore, *energy, *env, *web, *flow, *conv, *wheel, *berth, *train, *yard, *silo);
				}
				record->energyGui(*message, *simucore, *energy);
				ImGui::Unindent();
			}
		}
		if (this->compitence == 6 || this->compitence == 12)
		{
			simucore->simulator_gui();
		}
		env->env_dispatch(simucore->runtime_hours, simucore->runtime_minutes, (this->compitence == 11 || this->compitence == 12), camera, woodShader, glassShader, skyboxShader);
		if (this->compitence == 9 || this->compitence == 10 || this->compitence == 12)
		{
			energy->transformerDispatch();
		}
		if (this->compitence == 5 || this->compitence == 10 || this->compitence == 12)
		{
			web->web_dispatch(this->compitence == 12, this->compitence == 10 || this->compitence == 12);
			conv->conv_dispatch(this->compitence == 12);
			flow->showGui(*message, *energy, *conv, *wheel, *berth, *train, *yard, *silo, *web);
		}
		if ((this->compitence > 0 && this->compitence < 5) || this->compitence == 10 || this->compitence == 12)
		{
			if (ImGui::CollapsingHeader(u8"船舶与火车调度台"))
			{
				ImGui::Indent();
				if (this->compitence != 1 && this->compitence != 3)
				{
					int ship_berth = berth->ship_dispatch(*message);
					if (ship_berth != -1)
					{
						//船舶离港中断
						flow->ship_leave(*energy, ship_berth, *yard, *web);
					}
				}
				if (this->compitence == 1 || this->compitence == 10 || this->compitence == 12)
				{
					berth->unloader_dispatch();
				}
				if (this->compitence != 1 && this->compitence != 2)
				{
					end_train_2 = train->train_dispatch(*message);
				}
				ImGui::Text(" ");
				ImGui::Unindent();
			}
		}
		if ((this->compitence > 6 && this->compitence < 11 && this->compitence != 9) || this->compitence == 12)
		{
			if (ImGui::CollapsingHeader(u8"堆场与筒仓调度台"))
			{
				ImGui::Indent();
				flow->add_type(*message, *conv, *wheel, *berth, *train, *yard, *web);
				if (this->compitence != 8)
				{
					yard->yard_dispatch(*message, this->compitence == 12);
				}
				if (this->compitence != 7)
				{
					silo->silo_dispatch(this->compitence == 12);
				}
				ImGui::Unindent();
			}
		}
		ImGui::End();
		flow->show_ground();
		yard->yard_choose(*message);
		silo->show_select();
		message->show();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//末尾处理
		if (end_train_1 || end_train_2)
		{
			flow->train_check(*energy, end_train_1, end_train_2, *train, *yard, *web);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Console::identity_choose()
{
	if (ImGui::CollapsingHeader(u8"权限选择"))
	{
		ImGui::Indent();
		static int choose = 12;
		for (int i = 0; i < 13; i++)
		{
			ImGui::RadioButton((this->names[i]).c_str(), &choose, i);
			if (i != 6 && i != 12)
			{
				ImGui::SameLine();
			}
		}
		this->compitence = choose;
		ImGui::Text((this->descriptions[choose]).c_str());
		ImGui::Unindent();
	}
}
