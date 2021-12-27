#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <tools/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include <tools/stb_image.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    glfwInit();
    //设置主要和次要版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Faild to creat GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Falied to initialize GLAD" << std::endl;
        return -1;
    }

    //設置視口
    glViewport(0, 0, 800, 600);
    glEnable(GL_PROGRAM_POINT_SIZE);

    //注册窗口监听函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader ourShader("./src/06_Transform/shader/vertex.glsl", "./src/06_Transform/shader/fragment.glsl");

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported:" << nrAttributes << std::endl;

    //定义顶点数组
    float vertexs[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 左上
    };

    unsigned int indices[] = {
        // 注意索引从0开始!
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    // VAO:CPU发送顶点数据到GPU中后，在GPU内存中会存储这些顶点数据，方便渲染时GPU立即访问到这些顶点。
    //     从CPU放松数据到显卡相对较慢，所以一次尽可能发送多的数据，交由GPU来保存

    // VBO：用来封装所有和定点处理器相关的数据的特殊对象，它并不保存实际数据，而是放顶点数据，顶点颜色，索引缓冲等的状态配置；
    //      类似于Unity中的mesh

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    //绑定VAO对象
    glBindVertexArray(VAO);

    //绑定VBO对象
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //只需绑定一次就可以

    //绑定EBO对象
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    //填充EBO数据
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //填充VBO数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);

    //解析顶点数据 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    //解析顶点颜色数据 设置顶点属性指针
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //解析顶点坐标数据 设置顶点属性指针
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    //生成纹理
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    //绑定纹理
    glBindTexture(GL_TEXTURE_2D, texture1);

    //设置环绕和过滤方式
    float borderColor[] = {0.3f, 0.1f, 0.7f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //图像y轴翻转
    stbi_set_flip_vertically_on_load(true);

    //加载图片
    int width, height, nrChannels;
    unsigned char *data = stbi_load("D:/JustStudy/openglCode/static/images/textures/container.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        //生成图片
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);

    glGenTextures(1, &texture2);
    //绑定纹理
    glBindTexture(GL_TEXTURE_2D, texture2);

    //设置环绕和过滤方式

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //图像y轴翻转
    stbi_set_flip_vertically_on_load(true);

    data = stbi_load("D:/JustStudy/openglCode/static/images/textures/awesomeface.png", &width, &height, &nrChannels, 0);

    if (data)
    {
        //生成图片
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);
    ourShader.use();
    ourShader.setInt("tex", 0);
    ourShader.setInt("tex1", 1);
    float t = 0.0f;

    //初始化一个四分量的向量
    glm::vec4 position = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    //初始化一个单位矩阵
    glm::mat4 trans = glm::mat4(1.0f);

    //向右平移一个单位
    trans = glm::translate(trans, glm::vec3(1.0f, 0.0f, 0.0f));

    position = trans * position;

    //缩放
    trans = glm::scale(trans, glm::vec3(2.0f, 3.0f, 4.0f));

    position = trans * position;

    //旋转
    trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    position = trans * position;

    std::cout << position.x << "--" << position.y << "--" << position.z << std::endl;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();

        t = glfwGetTime();
        ourShader.setFloat("angle", glfwGetTime());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}