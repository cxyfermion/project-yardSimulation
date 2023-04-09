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

#define GLSL_VERSION "#version 330\n"

/*
���ǵĳ������Ѿ������˶�������壬����ÿ���������ɳɰ���ǧƽ̹����������ɡ����������������ϸ�������ķ�ʽ�����Ӷ���ϸ�ڣ�������ʵ�У����ض���μ���������������������ɵ���ʵ������ȷ�����棬Ȼ�������������ʱ�������ʵ�����ز�ס�ˡ���ʵ�е�������沢����ƽ̹�ģ����Ǳ��ֳ���������͹��ƽ�ģ�ϸ�ڡ�
���磬ש��ı��档ש��ı���ǳ��ֲڣ���Ȼ������ȫƽ̹�ģ��������Žӷ촦ˮ�఼�ۣ��Լ��ǳ����ϸС�Ŀն������������һ���й�ĳ����п�����һ��ש��ı��棬����ͳ����ˡ�
��������Թ���ӽ�����������⣺��ʲôʹ���汻��Ϊ��ȫƽ̹�ı������������𰸻��Ǳ���ķ����������Թ����㷨���ӽǿ��ǵĻ���ֻ��һ���¾����������״������Ǵ�ֱ�����ķ���������ש�����ֻ��һ������������������ȫ�������������������һ�µķ�ʽ���������ÿ��fragment�������Լ��Ĳ�ͬ�ķ��߻��������������ǾͿ��Ը��ݱ���ϸ΢��ϸ�ڶԷ����������иı䣻�����ͻ���һ�ֱ��濴����Ҫ���ӵö�Ļþ���
ÿ��fragmentʹ�����Լ��ķ��ߣ����ǾͿ����ù�������һ�������ɺܶ�΢С�ģ���ֱ�ڷ��������ģ�ƽ������ɣ���������ϸ�ڽ���õ���������������ÿ��fragmentʹ�ø��Եķ��ߣ����һ����������fragmentʹ��ͬһ�����ߵļ�������������ͼ��normal mapping����͹��ͼ��bump mapping����

������ͼ
Ϊʹ������ͼ������������ҪΪÿ��fragment�ṩһ�����ߡ���diffuse��ͼ��specular��ͼһ�������ǿ���ʹ��һ��2D���������淨�����ݡ�2D���������Դ�����ɫ�͹������ݣ������Դ��淨���������������ǿ��Դ�2D�����в����õ��ض�����ķ���������
���ڷ��������Ǹ����ι��ߣ�������ͨ��ֻ���ڴ�����ɫ��Ϣ���������淨���������Ƿǳ�ֱ�ӡ��������һ�룬�ͻ�֪�������е���ɫ������r��g��bԪ�ش���һ��3D���������Ƶ�����Ҳ���Խ�����������x��y��zԪ�ش��浽�����У�������ɫ��r��g��bԪ�ء����������ķ�Χ��-1��1֮�䣬����������Ҫ����ӳ�䵽0��1�ķ�Χ��
    vec3 rgb_normal = normal * 0.5 + 0.5; // �� [-1,1] ת���� [0,1]
�����������任Ϊ��������RGB��ɫԪ�أ����Ǿ��ܰѸ��ݱ������״��fragment�ķ��߱�����2D�����С�
�����һ��ƫ��ɫ�����������������ҵ��ļ������з�����ͼ���������ģ���������Ϊ���з��ߵ�ָ��ƫ��z�ᣨ0, 0, 1������һ��ƫ������ɫ������������z�᷽��Ҳ������������΢ƫ�ƣ���ɫҲ�ͷ�������΢�仯������������������һ����ȡ�
���磬����Կ�����ÿ��ש��Ķ�������ɫ������ƫ�̣�������Ϊש��Ķ����ķ���ƫ����ָ����y�᷽��0, 1, 0����������������ɫ���ˡ�

�������������ǰ󶨵����ʵ�����Ԫ��Ȼ��ʹ������ĸı��˵�������ɫ������Ⱦһ��ƽ�棺
    uniform sampler2D normalMap;
    void main()
    {
        // �ӷ�����ͼ��Χ[0,1]��ȡ����
        normal = texture(normalMap, fs_in.TexCoords).rgb;
        // ����������ת��Ϊ��Χ[-1,1]
        normal = normalize(normal * 2.0 - 1.0);
        [...]
        // �����������������
    }
�������ǽ��������ķ�����ɫ��0��1����ӳ���-1��1�����ܽ�RGB��ɫ���´���ɷ��ߣ�Ȼ��ʹ�ò������ķ�������Ӧ���ڹ��յļ��㡣������������ʹ�õ���Blinn-Phong��ɫ����
��һ����ͬ������ռ��н��й��գ��������ռ��������ͼ��������ָ���������ռ����z�������еĹ�������������������z������б任���������Ǿ���ʼ��ʹ��ͬ���ķ�����ͼ�����ܳ������⡣�������ռ�������߿ռ䣨tangent space����

���߿ռ�
������ͼ�еķ����������������߿ռ��У������߿ռ��У�������Զָ����z�������߿ռ���λ�������α���֮�ϵĿռ䣺��������ڵ��������εı��زο���ܡ�
TBN������������ĸ�ֱ����tangent��bitangent��normal���������ǽ���������������������Ҫ��������һ�������߿ռ�ת��Ϊ��ͬ�ռ�ı������������Ҫ�����໥��ֱ��������������һ������ķ�����ͼ�����ڣ��ϡ��ҡ�ǰ�����������������̳����������ơ�

�ֹ��������ߺ͸�����
����ƽ��ʹ���������������������1��2��3��1��3��4�����������������Σ���
    // positions
    glm::vec3 pos1(-1.0,  1.0, 0.0);
    glm::vec3 pos2(-1.0, -1.0, 0.0);
    glm::vec3 pos3(1.0, -1.0, 0.0);
    glm::vec3 pos4(1.0, 1.0, 0.0);
    // texture coordinates
    glm::vec2 uv1(0.0, 1.0);
    glm::vec2 uv2(0.0, 0.0);
    glm::vec2 uv3(1.0, 0.0);
    glm::vec2 uv4(1.0, 1.0);
    // normal vector
    glm::vec3 nm(0.0, 0.0, 1.0);
�����ȼ����һ�������εıߺ�deltaUV���꣺
    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos1;
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;
���˼������ߺ͸����ߵıر����ݣ����ǾͿ��Կ�ʼд��������ǰ�沿���е����е�ʽ��
    GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent1 = glm::normalize(tangent1);

    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent1 = glm::normalize(bitangent1);

    [...] // ��ƽ��ĵڶ��������β������Ʋ���������ߺ͸�����
����Ԥ�ȼ������ʽ�ķ�������f��Ȼ�������ÿ��������Ԫ�ؽ�����Ӧ����˷��������Ѵ�������յĵ�ʽ�Ա���ᷢ�֣������ֱ�����á�������ǻ�Ҫ���б�׼������ȷ������/��������������ǵ�λ������
��Ϊһ����������Զ��ƽ̹����״������ֻ��Ϊÿ�������μ���һ������/�����ߣ����Ƕ���ÿ���������ϵĶ��㶼��һ���ġ�Ҫע����Ǵ����ʵ��ͨ�������κ�������֮�䶼�Ṳ���㡣��������¿�����ͨ����ÿ������ķ��ߺ�����/�����ߵȶ�������ƽ�������Ի�ø�����͵�Ч����
���ǵ�ƽ���������֮�������һЩ���㣬������Ϊ�����������໥���У���˲�����Ҫ�����ƽ�����������ۺ�ʱֻҪ���������������ס�����Ǽ����¡�
�������ߺ͸�����������ֵӦ����(1, 0, 0)��(0, 1, 0)�����Ǻͷ���(0, 0, 1)����໥��ֱ��TBN����

���߿ռ䷨����ͼ
�Ӽ����Ͻ���������ɫ�������踱���ߡ����е�������TBN���������໥��ֱ���������ǿ����ڶ�����ɫ������T��N�����Ĳ�ˣ��Լ�����������ߣ�vec3 B = cross(T, N);
������������TBN���������ʹ�����أ�ͨ����˵�����ַ�ʽʹ����
1.����ֱ��ʹ��TBN�������������԰���������ռ������ת������������ռ䡣������ǰ�������Ƭ����ɫ���У���ͨ�������õ��ķ������������TBN����ת������������ռ��У��������з��ߺ��������ձ�������ͬһ������ϵ���ˡ�
2.����Ҳ����ʹ��TBN�������������������԰���������ռ������ת������������ռ䡣�������ʹ�������������������ձ�����������ת�������߿ռ䣬�������ߺ��������ձ�����һ����һ������ϵ���ˡ�
������������ռ�ת�������߿ռ��и�����ô������ǿ��԰�������������ڶ�����ɫ����ת�������߿ռ䣬������������ɫ����������¡����ǿ��еģ���ΪlightPos��viewPos����ÿ��fragment���ж�Ҫ�ı䣬����fs_in.FragPos������Ҳ�����ڶ�����ɫ�������������߿ռ�λ�á������ϣ�����Ҫ���κ�������������ɫ���н��б任������һ�ַ����о��Ǳ���ģ���Ϊ���������ķ�����������ÿ��������ɫ������һ����
�������ڲ��ǰ�TBN�����������͸�������ɫ�������ǽ����߿ռ�Ĺ�Դλ�ã��۲�λ���Լ�����λ�÷��͸�������ɫ�����������ǾͲ�����������ɫ������о���˷��ˡ�����һ�����ѵ��Ż�����Ϊ������ɫ��ͨ����������ɫ�����е��١���Ҳ��Ϊʲô���ַ�����һ�ָ��õ�ʵ�ַ�ʽ��ԭ��
    out VS_OUT {
        vec3 FragPos;
        vec2 TexCoords;
        vec3 TangentLightPos;
        vec3 TangentViewPos;
        vec3 TangentFragPos;
    } vs_out;

    uniform vec3 lightPos;
    uniform vec3 viewPos;

    [...]

    void main()
    {
        [...]
        mat3 TBN = transpose(mat3(T, B, N));
        vs_out.TangentLightPos = TBN * lightPos;
        vs_out.TangentViewPos  = TBN * viewPos;
        vs_out.TangentFragPos  = TBN * vec3(model * vec4(position, 0.0));
    }
��������ͼӦ�õ����߿ռ��ϣ����ǻ�õ���Ͻ̳�һ��ʼ�Ǹ��������ƵĽ������������ǿ��Խ�ƽ�泯��������򣬹���һֱ��������ȷ�ģ�
    glm::mat4 model;
    model = glm::rotate(model, (GLfloat)glfwGetTime() * -10, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    glUniformMatrix4fv(modelLoc 1, GL_FALSE, glm::value_ptr(model));
    RenderQuad();

��������
Assimp�и������õ����ã������Ǽ���ģ�͵�ʱ�����aiProcess_CalcTangentSpace����aiProcess_CalcTangentSpaceӦ�õ�Assimp��ReadFile����ʱ��Assimp��Ϊÿ�����صĶ���������͵����ߺ͸���������������ʹ�õķ��������Ǳ��̳�ʹ�õ����ơ�
    const aiScene* scene = importer.ReadFile(
        path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace
    );
���ǿ���ͨ������Ĵ�����Assimp��ȡ������������߿ռ䣺
    vector.x = mesh->mTangents[i].x;
    vector.y = mesh->mTangents[i].y;
    vector.z = mesh->mTangents[i].z;
    vertex.Tangent = vector;
Ȼ���㻹�������ģ�ͼ����������ԴӴ�����ģ���м��ط�����ͼ��wavefront��ģ�͸�ʽ��.obj�������ķ�����ͼ�е㲻һ����Assimp��aiTextureType_NORMAL������������ķ�����ͼ����aiTextureType_HEIGHTȴ�ܣ��������Ǿ��������������ǣ�
    vector<Texture> specularMaps = this->loadMaterialTextures(
        material, aiTextureType_HEIGHT, "texture_normal"
    );
��Ȼ������ÿ��ģ�͵����ͺ��ļ���ʽ��˵���ǲ�ͬ�ġ�ͬ���˽�aiProcess_CalcTangentSpace���������ǺܺõĹ���Ҳ����Ҫ��������������Ҫ������������ģ���Щģ��������ʹ��һЩ����С���ɱ��羵��һ��ģ���ϵ��������ʱҲ��������һ����������ꣻ���������������������ر������ʱ��Assimp�Ͳ����ǣ�����Ͳ����ˡ�

���ڷ�����ͼ�������һ������Ҫ���ۣ��������ڲ��ػ���̫�����ܿ���������������������ʱ��֡�

���ڸ���������ϼ�������������ʱ�����������кܴ������Ĺ����㣬��������ͼӦ�õ���Щ����ʱ����������ƽ����ͨ���ܻ�ø��ø�ƽ���Ľ�����������и����⣬����TBN�������ܻ᲻�ܻ��ഹֱ������ζ��TBN�����������������ˡ�������ͼ���ܻ�����ƫ�ƣ�������Ȼ���ԸĽ���
ʹ�ý�������ķ-ʩ�������������̣�Gram-Schmidt process������ѧ���ɣ����ǿ��Զ�TBN����������������������ÿ���������ֻ����´�ֱ�ˡ��ڶ�����ɫ����������������
    vec3 T = normalize(vec3(model * vec4(tangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(normal, 0.0)));
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    vec3 B = cross(T, N);

    mat3 TBN = mat3(T, B, N)
������΢����һЩ���ܿ������ܶԷ�����ͼ����һ��������


*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
void renderQuad();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

    // build and compile shaders
    // -------------------------
    Shader shader("res/Shaders/Shader_advancedLight/4/normal_mapping.vert", "res/Shaders/Shader_advancedLight/4/normal_mapping.frag");

    // load textures
    // -------------
    unsigned int diffuseMap = loadTexture("res/textures/brickwall.jpg");
    unsigned int normalMap = loadTexture("res/textures/brickwall_normal.jpg");

    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("diffuseMap", 0);
    shader.setInt("normalMap", 1);

    // lighting info
    // -------------
    glm::vec3 lightPos(0.5f, 1.0f, 0.3f);

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

        // configure view/projection matrices
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        // render normal-mapped quad
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))); // rotate the quad to show normal mapping from multiple directions
        shader.setMat4("model", model);
        shader.setVec3("viewPos", camera.Position);
        shader.setVec3("lightPos", lightPos);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMap);
        renderQuad();

        // render light source (simply re-renders a smaller plane at the light's position for debugging/visualization)
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.1f));
        shader.setMat4("model", model);
        renderQuad();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// renders a 1x1 quad in NDC with manually calculated tangent vectors
// ------------------------------------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        // positions
        glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
        glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
        glm::vec3 pos3(1.0f, -1.0f, 0.0f);
        glm::vec3 pos4(1.0f, 1.0f, 0.0f);
        // texture coordinates
        glm::vec2 uv1(0.0f, 1.0f);
        glm::vec2 uv2(0.0f, 0.0f);
        glm::vec2 uv3(1.0f, 0.0f);
        glm::vec2 uv4(1.0f, 1.0f);
        // normal vector
        glm::vec3 nm(0.0f, 0.0f, 1.0f);

        // calculate tangent/bitangent vectors of both triangles
        glm::vec3 tangent1, bitangent1;
        glm::vec3 tangent2, bitangent2;
        // triangle 1
        // ----------
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        // triangle 2
        // ----------
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


        bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


        float quadVertices[] = {
            // positions            // normal         // texcoords  // tangent                          // bitangent
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
        };
        // configure plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
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