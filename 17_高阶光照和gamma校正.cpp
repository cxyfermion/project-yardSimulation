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

#define GLSL_VERSION "#version 330\n"

/*
https://learnopengl-cn.github.io/05%20Advanced%20Lighting/01%20Advanced%20Lighting/
�߼�����
�ڹ���С���У����Ǽ򵥵ؽ����˷��Ϲ���ģ�ͣ��������ǵĳ�������һ������ʵ�С���Ȼ����ģ�Ϳ������Ѿ��ܲ����ˣ�����ʹ������ʱ����Ȼ����һЩϸ�����⣬���ǽ�����һ�����������ǡ�

Blinn-Phong
���Ϲ��ղ�������ʵ�����кܺõĽ��ƣ���������Ҳ�ܸߡ��������ľ��淴�����һЩ����³������⣬�ر������巴��Ⱥܵ�ʱ���ᵼ�´�Ƭ���ֲڵģ��߹�����
���Կ������ھ���߹�����ı�Ե������һ�������ԵĶϲ㡣������������ԭ���ǹ۲������ͷ���������ļнǲ��ܴ���90�ȡ��������Ľ��Ϊ�����������������Ϊ0.0������ܻ���ã������������߼нǴ���90��ʱ��Ӧ�ò�����յ��κι�Ŷԣ������ⲻ��ʲô���⡣
Ȼ���������뷨����ֻ�������������������������������ʱ��������ߺ͹�Դ�нǴ���90�ȣ���Դ�ᴦ�ڱ��ձ�����·������ʱ����յ������������ȷ��Ϊ0.0�����ǣ��ڿ��Ǿ���߹�ʱ�����ǲ����ĽǶȲ����ǹ�Դ�뷨�ߵļнǣ����������뷴����������ļнǡ�
���������Ӧ�ú������ˡ���ͼ����������Ϥ�ķ��Ϲ����еķ�������������$\theta$��С��90�ȡ�����ͼ�У������뷴�䷽��֮��ļн����Դ���90�ȣ���������¾����������Ϊ0.0�����ڴ��������¶�����ʲô���⣬��Ϊ�۲췽���뷴�䷽�򶼷ǳ�Զ��Ȼ����������ķ���ȷǳ�Сʱ���������ľ���߹�뾶��������Щ�෴����Ĺ��߶����Ȳ����㹻���Ӱ�졣����������¾Ͳ��ܺ������ǶԾ��������Ĺ����ˡ�

1977�꣬James F. Blinn�ڷ�����ɫģ���ϼ�����չ��������Blinn-Phong��ɫģ�͡�Blinn-Phongģ�������ģ�ͷǳ����ƣ��������Ծ����ģ�͵Ĵ�������һЩ��ͬ���������ܹ����֮ǰ�ᵽ�����⡣Blinn-Phongģ�Ͳ��������ڷ������������ǲ�������ν�İ������(Halfway Vector)�������������߼н�һ�뷽���ϵ�һ����λ����������������뷨������Խ�ӽ�ʱ������������Խ��
�����������루���ڲ���Ҫ�ģ�������������ʱ����������ͻ��뷨���������ϡ����Ե��۲�������Խ�ӽ���ԭ��������ߵķ���ʱ������߹�ͻ�Խǿ��
��ȡ��������ķ����ܼ򵥣�ֻ��Ҫ�����ߵķ��������͹۲������ӵ�һ�𣬲���������滯(Normalize)�Ϳ����ˣ�
	vec3 lightDir   = normalize(lightPos - FragPos);
	vec3 viewDir    = normalize(viewPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
������������������ʵ�ʼ���ֻ�����ǶԱ��淨�ߺͰ����������һ��Լ�����(Clamped Dot Product)���õ�˽����Ϊ�����Ӷ���ȡ����֮��нǵ�����ֵ��֮�����Ƕ����ֵȡ����ȴη���
	float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
	vec3 specular = lightColor * spec;
����֮��Blinn-Phongģ�;�ûʲô��˵���ˣ�Blinn-Phong�����ģ��Ψһ��������ǣ�Blinn-Phong�������Ƿ�����������֮��ļнǣ�������ģ�Ͳ������ǹ۲췽���뷴��������ļнǡ�
������������֮����������Ӧ�þͲ����ٿ������Ϲ����и߹�ϲ������ˡ�
����֮�⣬����ģ����Blinn-Phongģ��Ҳ��һЩϸ΢�Ĳ�𣺰����������淨�ߵļн�ͨ����С�ڹ۲��뷴�������ļнǡ����ԣ���������úͷ�����ɫ���Ƶ�Ч�����ͱ�����ʹ��Blinn-Phongģ��ʱ�����淴������ø���һ�㡣ͨ�����ǻ�ѡ�������ɫʱ����ȷ�����2��4����
�������ʹ����һ���򵥵�Ƭ����ɫ�����������ܹ��ڷ��Ϸ�����Blinn-Phong���������л���
	void main()
	{
		[...]
		float spec = 0.0;
		if(blinn)
		{
			vec3 halfwayDir = normalize(lightDir + viewDir);
			spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
		}
		else
		{
			vec3 reflectDir = reflect(-lightDir, normal);
			spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
		}
	}

����԰���B�����л����Ϲ�����Blinn-Phong����


GammaҲ�лҶ�ϵ����ÿ����ʾ�豸�����Լ���Gammaֵ��������ͬ����һ����ʽ���豸������� = ��ѹ��Gamma���ݣ��κ��豸Gamma�����϶��������1������1��һ�����������״̬����������״̬�ǣ������ѹ�����ȶ�����0��1�����䣬��ô���ٵ�ѹ�͵��ڶ������ȡ�
����CRT��Gammaͨ��Ϊ2.2�������������� = �����ѹ��2.2���ݣ�����Դӱ��ڵڶ���ͼ�п���Gamma2.2ʵ����ʾ�������ܻ��Ԥ�ڰ����෴Gamma0.45�ͻ������Ԥ����������㽲Gamma0.45���ӵ�Gamma2.2����ʾ�豸�ϣ�����ƫ������ʾЧ������У��

GammaУ��
�����Ǽ�����������������ص�������ɫ�Ժ����Ǿͱ����������ʾ�ڼ������ϡ���ȥ����������������������߹���ʾ����CRT������Щ��������һ���������Ծ��������������ѹ�����Ĳ������������ȡ������ѹ����ԼΪ�����ѹ��2.2���ݵ����ȣ������������Gamma��
��Ϊ���ۿ�����ɫ�����ȸ������ڶ����Ļҽף�������ʹ�õ�Ҳ��һ��ָ����ϵ����ѹ��2.2���ݣ���������������ͨ���������ܹ���ӳ�䵽�����ķ��������ȣ���˿�����Ч��������ע��CRT�������ǵ�ѹ��2.2���ݶ������൱��2���ݣ����CRT���ȱ�������������˵���Ҫ����

GammaУ��
GammaУ��(Gamma Correction)��˼·�������յ���ɫ�����Ӧ�ü�����Gamma�ĵ�������ͷ��ǰ���Gamma����ͼ�������һ���̻��ߣ����Ǽ�����Gamma���ߵķ�ת���ߡ�
��������ɫ��ʾ����������ʱ���ÿ����ɫ��������������ת��Gamma���ߣ�����Ӧ���˼�����Gamma�Ժ����յ���ɫ�����Ϊ���Եġ��������õ����м�ɫ���ͻ������������Ȼ������ʹ���Ǳ䰵�����������ֽ���ƽ������ˡ�

2.2ͨ�����Ǵ������ʾ�豸�Ĵ��ƽ��gammaֵ������gamma2.2����ɫ�ռ����sRGB��ɫ�ռ䡣ÿ����������gamma���߶�������ͬ������gamma2.2�ڴ�����������ϱ��ֶ�����
�������ԭ����Ϸ��������Ϊ����ṩ�ı���Ϸgamma���õ�ѡ�����Ӧÿ��������
���ڳ��������õ���ɫ��������ն��ǻ�������Gamma����Gamma1�������������е����Ⱥ�ʵ�ʱ����Ĳ�һ�������Ҫ�����������е����Ⱦ�Ҫ��������ս���У��

����������ĳ�����Ӧ��gammaУ���ķ�ʽ��

ʹ��OpenGL�ڽ���sRGB֡���塣�Լ���������ɫ���н���gammaУ����
����GL_FRAMEBUFFER_SRGB�����Ը���OpenGLÿ�������Ļ������������ɫ���浽��ɫ����֮ǰ��У��sRGB��ɫ��sRGB�����ɫ�ռ���¶�Ӧ��gamma2.2����Ҳ�Ǽ����豸��һ����׼��
����GL_FRAMEBUFFER_SRGB�Ժ�ÿ��������ɫ�����к���֡���壬OpenGL���Զ�ִ��gammaУ��������Ĭ��֡���塣
����GL_FRAMEBUFFER_SRGB�򵥵ĵ���glEnable���У�
    glEnable(GL_FRAMEBUFFER_SRGB);
�Դˣ�����Ⱦ��ͼ��ͱ�����gammaУ�������㲻��Ҫ���κ�����Ӳ���Ͱ��㴦���ˡ�
gammaУ������������ɫ�ռ�ת��Ϊ�����Կռ䣬���������һ������gammaУ���Ǽ�����Ҫ�ġ��������������֮ǰ�ͽ���gammaУ�������еĺ������������ڲ�������ȷ����ɫֵ��
���磬�����ʹ�ö��֡���壬����ܴ���������֡����֮�䴫�ݵ��м�����Ȼ�������Կռ���ɫ��ֻ�Ǹ����͸��������������Ǹ�֡����Ӧ��gammaУ����

������������׶�����Ļ�ı�����Ӧ��gammaУ��
�ڶ���������΢���ӵ㣬��ͬʱҲ�����Ƕ�gamma��������ȫ�Ŀ���Ȩ��������ÿ�����������ɫ�����е����Ӧ��gammaУ���������ڷ��͵�֡����ǰ����ɫ�ͱ�У���ˡ�
    void main()
    {
        // do super fancy lighting
        [...]
        // apply gamma correction
        float gamma = 2.2;
        fragColor.rgb = pow(fragColor.rgb, vec3(1.0/gamma));
    }
���һ�д��룬��fragColor��ÿ����ɫԪ��Ӧ����һ��1.0/gamma�������㣬У��������ɫ������ɫ�����
��������и��������Ϊ�˱���һ�£��������������ɫ����������gammaУ��������������кܶ�������ɫ�������ǿ��ֱܷ����ڲ�ͬ���壬��ô��ͱ�����ÿ����ɫ���ﶼ����gammaУ���ˡ�һ�����򵥵ķ������������Ⱦѭ�����������׶Σ��ں����ı�����Ӧ��gammaУ����������ֻҪ��һ�ξͺ��ˡ�

sRGB����
��Ϊ������������sRGB�ռ�����ʾӦ����gamma����ɫ������ʲôʱ�����ڼ�����ϻ��ơ��༭���߻���һ��ͼƬ��ʱ������ѡ����ɫ���Ǹ������ڼ������Ͽ��������֡���ʵ����ζ�������㴴����༭��ͼƬ�����������Կռ䣬������sRGB�ռ��У���ע��sRGB�ռ䶨���gamma�ӽ���2.2���������������Ļ�϶԰���ɫ��һ�������Ǹ���������֪�������Ƚ��еģ��������ڽ���ɫԪ�ؼӱ���
�����������༭�ߣ���������������������sRGB�ռ��е��������������������ȾӦ����ʹ����Щ�������Ǳ��뿼�ǵ���㡣������Ӧ��gammaУ��֮ǰ���ⲻ�Ǹ����⣬��Ϊ������sRGB�ռ䴴����չʾ��ͬ�����ǻ�����sRGB�ռ���ʹ�ã��Ӷ�����gammaУ��������ʾҲû���⡣Ȼ�������������ǰ����ж������������Կռ���չʾ�ģ�������ɫ�ͻ�仵
����ͼ��ʵ��̫���ˣ����������������Ϊ������ʵ���Ͻ���������gammaУ������һ�룬�����ǻ��ڼ������Ͽ������������һ��ͼ�����Ǿ��Ѿ�����ɫֵ������gammaУ���������ٴ���ʾ�ڼ������Ͼ�û��������������Ⱦ���ֽ�����һ��gammaУ����ͼƬ��ʵ��̫���ˡ�
Ϊ���޸�������⣬���ǵ�ȷ�������������������Կռ��н��д����ġ����ǣ����ڴ�������������߲���֪��ʲô��gammaУ����������sRGB�ռ��н��д������򵥣���Ҳ����һ���ð취��
��һ�������������У�������ЩsRGB�����ڽ����κ���ɫֵ�ļ���ǰ������Կռ䡣���ǿ�����������
    float gamma = 2.2;
    vec3 diffuseColor = pow(texture(diffuse, texCoords).rgb, vec3(gamma));
Ϊÿ��sRGB�ռ������������·ǳ����ˡ��Һã�OpenGL�������ṩ����һ��������������ǵ��鷳�������GL_SRGB��GL_SRGB_ALPHA�ڲ������ʽ��
���������OpenGL�д�����һ����������ָ��Ϊ��������sRGB�����ʽ����֮һ��OpenGL���Զ�����ɫУ�������Կռ��У�����������ʹ�õ�������ɫֵ���������Կռ��е��ˡ����ǿ���������һ������ָ��Ϊһ��sRGB����
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
����㻹�������������������alphaԪ�أ��ؾ����뽫������ڲ���ʽָ��ΪGL_SRGB_ALPHA��
��Ϊ����������������sRGB�ռ��е����Ե��������ָ��ΪsRGB����ʱҪ����С�ġ�����diffuse��������Ϊ������ɫ��������������sRGB�ռ��еġ���Ϊ�˻�ȡ���ղ�����������specular��ͼ�ͷ�����ͼ�����������Կռ��У���������������Ҳ����ΪsRGB����Ļ������վͻ����ˡ�ָ��sRGB����ʱҪ���ġ�
��diffuse������ΪsRGB����֮���㽫��������������Ӿ�����������ÿ�����嶼��ֻ����һ��gammaУ����

˥��
��ʹ����gammaУ��֮����һ����֮ͬ���ǹ���˥��(Attenuation)����ʵ�����������У����յ�˥���͹�Դ�ľ����ƽ���ɷ��ȡ�
    float attenuation = 1.0 / (distance * distance);
Ȼ����������ʹ�����˥����ʽ��ʱ��˥��Ч�����ǹ���ǿ�ң���ֻ������һСȦ��������������ʵ���������ԭ������ʹ���ڻ������ս̳��������۵�����˥�����̣����������Ǹ���Ŀ���Ȩ���������ǻ�����ʹ��˫���ߺ�����
    float attenuation = 1.0 / distance;
˫���߱�ʹ�ö��κ��������ڲ���gammaУ����ʱ����������ʵ�����������ǿ���gammaУ���Ժ�����˥��������̫���ˣ���������Ķ��κ���ͻȻ�����˸��õ�Ч����
���ֲ��������ԭ���ǣ����˥�����̸ı�������ֵ��������Ļ����ʾ������Ҳ�������Կռ䣬�ڼ�������Ч����õ�˥�����̣������Ƿ�������ġ�����ƽ��˥�����̣��������ʹ��������̣����Ҳ�����gammaУ������ʾ�ڼ������ϵ�˥������ʵ���Ͻ����(1.0/distance^2)^2.2��
��������gammaУ������������ǿ�ҵ�˥������Ҳ������Ϊʲô˫���߲���gammaУ��ʱ����������ʵ

ʾ����������ĻgammaУ��

*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
bool blinn = false;
bool blinnKeyPressed = false;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // build and compile shaders
    // -------------------------
    Shader shader("res/Shaders/Shader_advancedLight/1/Blinn-Phong.vert", "res/Shaders/Shader_advancedLight/1/Blinn-Phong.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float planeVertices[] = {
        // positions            // normals         // texcoords
         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
         10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
    };
    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    // load textures
    // -------------
    //unsigned int floorTexture = loadTexture("marble.png");
    unsigned int floorTexture = loadTexture("res/textures/wood.jpg");

    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("texture1", 0);

    // lighting info
    // -------------
    glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw objects
        shader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        // set light uniforms
        shader.setVec3("viewPos", camera.Position);
        shader.setVec3("lightPos", lightPos);
        shader.setInt("blinn", blinn);
        // floor
        glBindVertexArray(planeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        std::cout << (blinn ? "Blinn-Phong" : "Phong") << std::endl;

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKayboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKayboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKayboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKayboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed)
    {
        blinn = !blinn;
        blinnKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
    {
        blinnKeyPressed = false;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}