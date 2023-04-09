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
	//ֻ�ڰ�������Ҽ�ʱ�����任�ӽ�
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
	this->compitence = 11;
	std::string temp_1[12] = { u8"�ο�", u8"ж������ʻԱ", u8"��������Ա", u8"�𳵵���Ա", u8"�𳵴����ܵ���Ա", u8"���̹���Ա", u8"������Ա", u8"�ѳ�����Ա", u8"Ͳ�ֹ���Ա", u8"��������Ա", u8"����Ա", u8"�ϵ�" };
	std::string temp_2[12] = { u8"���й۹�Ȩ��", u8"ӵ��ж�����Ŀ���Ȩ��", u8"ӵ�д����ĵ���Ȩ��", u8"ӵ�л𳵵ĵ���Ȩ��", u8"ӵ�л𳵺ʹ����ĵ���Ȩ��", u8"ӵ������ϵͳ����Ȩ��", u8"ӵ�з���ϵͳ����Ȩ��", u8"ӵ�жѳ�ϵͳ����Ȩ��", u8"ӵ��Ͳ��ϵͳ����Ȩ��", u8"ӵ��ϵͳ��������Ȩ��", u8"ӵ�п��ƻ���Ȩ��", u8"ȫ֪ȫ��" };
	for (int i = 0; i < 12; i++)
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
	glfwSetScrollCallback(this->window, mouse_scroll_posotion);
	//����������ڲ���ʾ���
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
	//std::unique_ptr<Conveyor> conv(new Conveyor);
	//std::unique_ptr<SlewingWheel> wheel(new SlewingWheel);
	//std::unique_ptr<Flow> flow(new Flow);
	//std::unique_ptr<Berth> berth(new Berth);
	Conveyor conv;
	SlewingWheel wheel;
	Flow flow;
	Berth berth;
	TrainLoader train;
	Yard yard;
	Silo silo;
	SimuCore simucore;
	Environment env;

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

	//״̬��ʼ��
	if (this->random_initiating)
	{
		berth.ship_random_initiator(simucore);
		train.train_random_initiator(simucore);
		yard.yard_random_initiator(simucore);
		silo.silo_random_initiator(simucore);
	}

	//���Ч����ʾ
	glEnable(GL_DEPTH_TEST);
	//imGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	ImGui::StyleColorsDark();
	//�����������壬����������CPUռ����
	ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyh.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
	conv.initGuiStyle();
	flow.initGuiStyle();
	berth.initGuiStyle();
	train.initGuiStyle();
	yard.initGuiStyle();
	silo.initGuiStyle();
	simucore.initGuiStyle();
	//����
	env.createEnv(woodShader, glassShader, skyboxShader);

	while (!glfwWindowShouldClose(window))
	{
		env.processInput(camera, window);
		int end_train_1 = false;
		int end_train_2 = false;
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//����״̬
		simucore.updateTime();
		env.updateEnv();
		if (!simucore.simu_pause || simucore.stepping)
		{
			conv.updateConvs(simucore.run_rate);
			berth.updateBuckets(simucore.run_rate);
			if (berth.berth_finished != -1)
			{
				//����ж�ս���
				flow.end_shipunloading(berth.berth_finished, conv, wheel, berth, yard);
			}
			int ret_ship = berth.updateShips(simucore.run_rate);
			if (ret_ship == 1)
			{
				//װ��װ������
				flow.end_shiploading(conv, berth);
			}
			end_train_1 = train.updateTrains(simucore.run_rate);
			if (yard.updateYards(simucore.run_rate))
			{
				flow.stop_yard_flow(yard.terminate_wheel, conv, wheel, berth, train, silo);
			}
			silo.updateStraight(simucore.run_rate);
			if (silo.updateSilos(simucore.run_rate))
			{
				flow.stop_silo_flow(conv, wheel, yard);
			}
			//��������
			if (simucore.stepping)
			{
				simucore.stepping = false;
			}
		}
		//����
		if (simucore.reset_rand || simucore.reset_zero)
		{
			conv.reset();
			wheel.reset();
			flow.reset();
			berth.reset(simucore, simucore.reset_rand);
			train.reset(simucore, simucore.reset_rand);
			yard.reset(simucore, simucore.reset_rand);
			silo.reset(simucore, simucore.reset_rand);
			simucore.reset_rand = false;
			simucore.reset_zero = false;
		}
		//����ͼ��
		conv.draw(camera, convShader, 0.3f * std::sin((float)simucore.time * 2) + 0.7f);
		wheel.draw(camera, wheelShader);
		berth.drawUnloader(camera, unloaderShader, bucketShader);
		berth.drawLoader(camera, unloaderShader);
		berth.drawShip(camera, shipShader);
		train.drawBuilding(camera, buildingShader);
		train.drawTrain(camera, trainShader);
		yard.drawYard(camera, yardShader);
		silo.drawSilo(camera, siloShader);
		//start new frame for imGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//������崰��
		ImGui::Begin(u8"�������");
		simucore.base_info();
		this->identity_choose();
		if (this->compitence == 6 || this->compitence == 11)
		{
			simucore.simulator_gui();
		}
		env.env_dispatch(simucore.runtime_hours, simucore.runtime_minutes, (this->compitence == 10 || this->compitence == 11), camera, woodShader, glassShader, skyboxShader);
		if (this->compitence == 5 || this->compitence == 9 || this->compitence == 11)
		{
			conv.conv_dispatch(this->compitence == 11);
			flow.showGui(conv, wheel, berth, train, yard, silo);
		}
		if ((this->compitence > 0 && this->compitence < 5) || this->compitence == 9 || this->compitence == 11)
		{
			if (ImGui::CollapsingHeader(u8"������𳵵���̨"))
			{
				ImGui::Indent();
				if (this->compitence != 1 && this->compitence != 3)
				{
					int ship_berth = berth.ship_dispatch();
					if (ship_berth != -1)
					{
						//��������ж�
						flow.ship_leave(ship_berth, conv, wheel, yard);
					}
				}
				if (this->compitence == 1 || this->compitence == 9 || this->compitence == 11)
				{
					berth.unloader_dispatch();
				}
				if (this->compitence != 1 && this->compitence != 2)
				{
					end_train_2 = train.train_dispatch();
				}
				ImGui::Text(" ");
				ImGui::Unindent();
			}
		}
		if ((this->compitence > 6 && this->compitence < 10) || this->compitence == 11)
		{
			if (ImGui::CollapsingHeader(u8"�ѳ���Ͳ�ֵ���̨"))
			{
				ImGui::Indent();
				flow.add_type(conv, wheel, berth, train, yard);
				if (this->compitence != 8)
				{
					yard.yard_dispatch(this->compitence == 11);
				}
				if (this->compitence != 7)
				{
					silo.silo_dispatch(this->compitence == 11);
				}
				ImGui::Unindent();
			}
		}
		ImGui::End();
		yard.yard_choose();
		silo.show_select();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//ĩβ����
		if (end_train_1 || end_train_2)
		{
			flow.train_check(end_train_1, end_train_2, conv, wheel, train, yard);
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
	if (ImGui::CollapsingHeader(u8"Ȩ��ѡ��"))
	{
		ImGui::Indent();
		static int choose = 11;
		for (int i = 0; i < 12; i++)
		{
			ImGui::RadioButton((this->names[i]).c_str(), &choose, i);
			if (i != 5 && i != 11)
			{
				ImGui::SameLine();
			}
		}
		this->compitence = choose;
		ImGui::Text((this->descriptions[choose]).c_str());
		ImGui::Unindent();
	}
}