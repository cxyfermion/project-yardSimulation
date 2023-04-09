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

/*
HDR
һ����˵�����洢��֡����(Framebuffer)��ʱ�����Ⱥ���ɫ��ֵ��Ĭ�ϱ�������0.0��1.0֮��ġ�����������޹������ʹ����һֱ����������ɫ��ֵ�����������Χ�ڣ��������볡�����ϡ��������ܹ����еģ�Ҳ�ܸ����������Ч����
�����������������һ���ض������������ж������Դʹ��Щ��ֵ�ܺͳ�����1.0���ֻᷢ��ʲô�أ�������ЩƬ���г���1.0�����Ȼ�����ɫֵ�ᱻԼ����1.0���Ӷ����³������һƬ�����Էֱ�
�������ڴ���Ƭ�ε���ɫֵ���ǳ��ӽ�1.0���ںܴ�һ��������ÿһ������Ƭ�ζ�����ͬ�İ�ɫ������ʧ�˺ܶ��ϸ�ڣ�ʹ�����������ǳ��١�
�����������һ�������Ǽ�С��Դ��ǿ�ȴӶ���֤������û��һ��Ƭ������1.0��Ȼ���Ⲣ����һ���õķ�������Ϊ����Ҫʹ�ò���ʵ�ʵĹ��ղ�����һ�����õķ���������ɫ��ʱ����1.0��Ȼ����ת����0.0��1.0�������ڣ��Ӷ���ֹ��ʧϸ�ڡ�
��ʾ��������Ϊֻ����ʾֵΪ0.0��1.0�����ɫ�������ڹ��շ�����ȴû��������ơ�ͨ��ʹƬ�ε���ɫ����1.0����������һ���������ɫ��Χ����Ҳ������HDR(High Dynamic Range, �߶�̬��Χ)������HDR�����Ķ������Ա�÷ǳ��������Ķ������Ա�÷ǳ��������ҳ���ϸ�ڡ�
���������۾�������ԭ��ǳ����ƣ�Ҳ��HDR��Ⱦ�Ļ����������ߺ�����ɶʱ�����ۻ��Զ������Ӷ�ʹ�����͹����Ĳ��ֱ�ø�����������������һ�����Զ����ݳ������ȵ������Զ��ع⻬�顣
HDR��Ⱦ��������ƣ����������ø���Χ����ɫֵ��Ⱦ�Ӷ���ȡ��Χ�ĺڰ��������ĳ���ϸ�ڣ��������HDRֵת������[0.0, 1.0]��Χ��LDR(Low Dynamic Range,�Ͷ�̬��Χ)��ת��HDRֵ��LDRֵ�ù��̽���ɫ��ӳ��(Tone Mapping)
��ʵʱ��Ⱦ�У�HDR�����������ǳ���LDR�ķ�Χ[0.0, 1.0]�뱣�������ϸ�ڣ�ͬʱ���������ܹ����ݹ�Դ����ʵǿ��ָ������ǿ�ȡ�����̫���б������֮��Ķ������ߵ�ǿ�ȣ���ô����Ϊʲô��������������?(����˵����һ��10.0��������) �����������ø���ʵ�Ĺ��ղ���ǡ��������һ�������Ĺ��գ�������LDR��Ⱦ���ǲ���ʵ�ֵģ���Ϊ���ǻᱻ����Լ����1.0��
��Ϊ��ʾ��ֻ����ʾ��0.0��1.0��Χ֮�ڵ���ɫ�����ǿ϶�Ҫ��һЩת���Ӷ�ʹ�õ�ǰ��HDR��ɫֵ������ʾ���ķ�Χ���򵥵�ȡƽ��ֵ����ת����Щ��ɫֵ�����ܺܺõĽ��������⣬��Ϊ�����ĵط����Եø�����������������������һ����ͬ�ķ�����/��������ת����ЩHDRֵ��LDRֵ���Ӷ������Ƕ��ڳ�����������ȫ�ƿأ������֮ǰ˵��ɫ���任��Ҳ��HDR��Ⱦ�����ղ��衣

����֡����
��ʵ��HDR��Ⱦ֮ǰ������������ҪһЩ��ֹ��ɫֵ��ÿһ��Ƭ����ɫ�����к�����Լ���ķ�������֡����ʹ����һ����׼���Ķ����ʽ(��GL_RGB)Ϊ����ɫ������ڲ���ʽ��OpenGL���ڽ���Щֵ����֡����ǰ�Զ�����Լ����0.0��1.0֮�䡣��һ�����Դ󲿷�֡�����ʽ���ǳ����ģ�����ר��������ű���չ��Χֵ�ĸ����ʽ��
��һ��֡�������ɫ������ڲ���ʽ���趨����GL_RGB16F, GL_RGBA16F, GL_RGB32F ����GL_RGBA32Fʱ����Щ֡���屻��������֡����(Floating Point Framebuffer)������֡������Դ洢����0.0��1.0��Χ�ĸ���ֵ�����Էǳ��ʺ�HDR��Ⱦ��
��Ҫ����һ������֡���壬����ֻ��Ҫ�ı���ɫ������ڲ���ʽ���������ˣ�ע��GL_FLOAT����)��
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
Ĭ�ϵ�֡����Ĭ��һ����ɫ����ֻռ��8λ(bits)����ʹ��һ��ʹ��32λÿ��ɫ�����ĸ���֡����ʱ(ʹ��GL_RGB32F ����GL_RGBA32F)��������Ҫ�ı����ڴ����洢��Щ��ɫ�����Գ�������Ҫһ���ǳ��ߵľ�ȷ�ȣ�32λ���Ǳ���ģ�ʹ��GLRGB16F���㹻�ˡ�
����һ�����и�����ɫ�����֡���壬���ǿ��Է�����Ⱦ���������֡�����С�������̵̳����ӵ��У���������Ⱦһ�����յĳ���������֡�����У�֮������һ�������ı���(Screen-filling Quad)��Ӧ�����֡�������ɫ���壬������������ӣ�
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // [...] ��Ⱦ(���յ�)����
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // ����ʹ��һ����ͬ����ɫ����HDR��ɫ������Ⱦ��2D�����ı�����
    hdrShader.Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrColorBufferTexture);
    RenderQuad();
���ﳡ������ɫֵ����һ�����԰���������ɫֵ�ĸ�����ɫ�����У�ֵ�����ǳ���1.0�ġ�����򵥵���ʾ�У�����������Ϊһ���������������ͨ�����ĸ����Դ������һ���ǳ�����������ľ�ͷ��
    std::vector<glm::vec3> lightColors;
    lightColors.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
    lightColors.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
    lightColors.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
    lightColors.push_back(glm::vec3(0.0f, 0.1f, 0.0f));
��Ⱦ������֡�������Ⱦ��һ����ͨ��֡������һ���ġ��µĶ������������hdrShader��Ƭ����ɫ����������Ⱦ����ӵ�и�����ɫ���������2D�ı��Ρ�����������һ���򵥵�ֱͨƬ����ɫ��(Pass-through Fragment Shader)��
    #version 330 core
    out vec4 color;
    in vec2 TexCoords;
    uniform sampler2D hdrBuffer;
    void main()
    {
        vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
        color = vec4(hdrColor, 1.0);
    }
��������ֱ�Ӳ����˸�����ɫ���岢������ΪƬ����ɫ���������Ȼ�������2D�ı��ε�����Ǳ�ֱ����Ⱦ��Ĭ�ϵ�֡�����У���������Ƭ����ɫ�������ֵ��Լ����0.0��1.0�䣬���������Ѿ�����һЩ���ڸ�����ɫ�����ֵ������1.0��
�����ԣ��������ͷ��ǿ���ֵ��Լ����1.0����Ϊһ��������ǰ�ɫ�ģ������г���1.0�ĵط���ʧ������ϸ�ڡ���Ϊ����ֱ��ת��HDRֵ��LDRֵ����������Ǹ�����û��Ӧ��HDRһ����Ϊ���޸��������������Ҫ����������ת�����и�����ɫֵ��0.0-1.0��Χ�С�������ҪӦ�õ�ɫ��ӳ�䡣

ɫ��ӳ��
ɫ��ӳ��(Tone Mapping)��һ����ʧ��С��ת��������ɫֵ�����������LDR[0.0, 1.0]��Χ�ڵĹ��̣�ͨ��������ض��ķ���ɫƽ��(Stylistic Color Balance)��
��򵥵�ɫ��ӳ���㷨��Reinhardɫ��ӳ�䣬���漰����ɢ����HDR��ɫֵ��LDR��ɫֵ�ϣ����е�ֵ���ж�Ӧ��Reinhardɫ��ӳ���㷨ƽ���ؽ���������ֵ��ɢ��LDR�ϡ����ǽ�Reinhardɫ��ӳ��Ӧ�õ�֮ǰ��Ƭ����ɫ���ϣ�����Ϊ�˸��õĲ�������һ��GammaУ������(����SRGB�����ʹ��)��
    void main()
    {
        const float gamma = 2.2;
        vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;

        // Reinhardɫ��ӳ��
        vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
        // GammaУ��
        mapped = pow(mapped, vec3(1.0 / gamma));

        color = vec4(mapped, 1.0);
    }
����Reinhardɫ��ӳ���Ӧ�ã����ǲ��ٻ��ڳ��������ĵط���ʧϸ�ڡ���Ȼ������㷨����������������ģ���������᲻��ô��ϸҲ����ô�����ֶȡ�

��һ����Ȥ��ɫ��ӳ��Ӧ�����ع�(Exposure)������ʹ�á�����ܻ��ǵ�֮ǰ�����ڽ����ｲ���ģ�HDRͼƬ�����ڲ�ͬ�ع�ȼ���ϸ�ڡ����������һ������Ҫչ����ҹ���棬���ǵ�Ȼ���ڰ���ʹ�õ��ع⣬��ҹ��ʹ�ø��ع⣬�������۵��ڷ�ʽһ������������ع���������ǿ���ȥ���ÿ���ͬʱ�ڰ����ҹ��ͬ�������������Ĺ��ղ���������ֻ��Ҫ�����ع���������ˡ�
һ���򵥵��ع�ɫ��ӳ���㷨����������
    uniform float exposure;
    void main()
    {
        const float gamma = 2.2;
        vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
        // �ع�ɫ��ӳ��
        vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
        // GammaУ��
        mapped = pow(mapped, vec3(1.0 / gamma));

        color = vec4(mapped, 1.0);
    }
���������ǽ�exposure����ΪĬ��Ϊ1.0��uniform���Ӷ��������Ǹ��Ӿ�ȷ�趨������Ҫע�غڰ����������������HDR��ɫֵ��������˵�����ع�ֵ��ʹ����ĺڰ�������ʾ�����ϸ�ڣ�Ȼ�����ع�ֵ���������ٺڰ������ϸ�ڣ����������ǿ����������������ϸ�ڡ�
ͨ���ı��ع�ȼ������ǿ��Կ��������ĺܶ�ϸ�ڣ�����Щϸ�ڿ�����LDR��Ⱦ�ж�����ʧ�ˡ�����˵�����ͷ���������ع���ľͷ�ṹ��Լ�ɼ������õ��ع�ľͷ�Ļ��ƾͿ������������ˡ����ڽ�����ľͷ������˵���ڸ��ع��»��ܸ��õĿ�����

�ո��л�HDR���أ�QE�ı��ع��

*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path, bool gammaCorrection);
void renderQuad();
void renderCube();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
bool hdr = true;
bool hdrKeyPressed = false;
float exposure = 1.0f;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
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

    // build and compile shaders
    // -------------------------
    Shader shader("res/Shaders/Shader_advancedLight/6/lighting.vert", "res/Shaders/Shader_advancedLight/6/lighting.frag");
    Shader hdrShader("res/Shaders/Shader_advancedLight/6/hdr.vert", "res/Shaders/Shader_advancedLight/6/hdr.frag");

    // load textures
    // -------------
    unsigned int woodTexture = loadTexture("res/textures/wood.png", true); // note that we're loading the texture as an SRGB texture

    // configure floating point framebuffer
    // ------------------------------------
    unsigned int hdrFBO;
    glGenFramebuffers(1, &hdrFBO);
    // create floating point color buffer
    unsigned int colorBuffer;
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // create depth buffer (renderbuffer)
    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
    // attach buffers
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // lighting info
    // -------------
    // positions
    std::vector<glm::vec3> lightPositions;
    lightPositions.push_back(glm::vec3(0.0f, 0.0f, 49.5f)); // back light
    lightPositions.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
    lightPositions.push_back(glm::vec3(0.0f, -1.8f, 4.0f));
    lightPositions.push_back(glm::vec3(0.8f, -1.7f, 6.0f));
    // colors
    std::vector<glm::vec3> lightColors;
    lightColors.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
    lightColors.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
    lightColors.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
    lightColors.push_back(glm::vec3(0.0f, 0.1f, 0.0f));

    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("diffuseTexture", 0);
    hdrShader.use();
    hdrShader.setInt("hdrBuffer", 0);

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

        // 1. render scene into floating point framebuffer
        // -----------------------------------------------
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)SCR_WIDTH / (GLfloat)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodTexture);
        // set lighting uniforms
        for (unsigned int i = 0; i < lightPositions.size(); i++)
        {
            shader.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
            shader.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
        }
        shader.setVec3("viewPos", camera.Position);
        // render tunnel
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));
        shader.setMat4("model", model);
        shader.setInt("inverse_normals", true);
        renderCube();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 2. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
        // --------------------------------------------------------------------------------------------------------------------------
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        hdrShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        hdrShader.setInt("hdr", hdr);
        hdrShader.setFloat("exposure", exposure);
        renderQuad();

        std::cout << "hdr: " << (hdr ? "on" : "off") << "| exposure: " << exposure << std::endl;

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
             // bottom face
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
              1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             // top face
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
              1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
              1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
              1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
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

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !hdrKeyPressed)
    {
        hdr = !hdr;
        hdrKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    {
        hdrKeyPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        if (exposure > 0.0f)
            exposure -= 0.001f;
        else
            exposure = 0.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        exposure += 0.001f;
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
unsigned int loadTexture(char const* path, bool gammaCorrection)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum internalFormat;
        GLenum dataFormat;
        if (nrComponents == 1)
        {
            internalFormat = dataFormat = GL_RED;
        }
        else if (nrComponents == 3)
        {
            internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
            dataFormat = GL_RGB;
        }
        else if (nrComponents == 4)
        {
            internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
            dataFormat = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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