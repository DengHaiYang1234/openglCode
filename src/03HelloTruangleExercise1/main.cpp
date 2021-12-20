#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const char *vertexShaderSouce = "#version 330 core\n"
                                "layout (location = 0) in vec3 aPos;\n"
                                "void main()\n"
                                "{\n"
                                "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                "   gl_PointSize = 20.0f;\n"
                                "}\0";

const char *fragmentShaderSouce = "#version 330 core\n"
                                  "out vec4 FragColor;\n"
                                  "void main()\n"
                                  "{\n"
                                  "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                  "}\n\0";

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

    //定义顶点数组
    float vertexs[] = {
        //第一个三角形
        -0.5f,0.5f,0.0f,
        -0.75f,-0.5f,0.0f,
        -0.25f,-0.5f,0.0f,

        //第二个三角形
        0.5f,0.5f,0.0f,
        0.75f,-0.5f,0.0f,
        0.25f,-0.5f,0.0f,};

    unsigned int indexes[] =
        {
            0, 1, 3,  // 第一个三角形
            1, 2, 3}; // 第二个三角形


    //VAO:CPU发送顶点数据到GPU中后，在GPU内存中会存储这些顶点数据，方便渲染时GPU立即访问到这些顶点。
    //    从CPU放松数据到显卡相对较慢，所以一次尽可能发送多的数据，交由GPU来保存

    //VBO：用来封装所有和定点处理器相关的数据的特殊对象，它并不保存实际数据，而是放顶点数据，顶点颜色，索引缓冲等的状态配置；
    //     类似于Unity中的mesh
    
    //IBO：Mesh的三角形顺序
    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    //绑定VAO对象
    glBindVertexArray(VAO);

    //绑定VBO对象
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//只需绑定一次就可以

    //填充VBO数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);

    //绑定EBO对象
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //填充EBO数据
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

    //解析顶点数据 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //创建顶点着色器
    unsigned int vertexShader, fragmentShader, shaderProgram;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    //附加着色器
    glShaderSource(vertexShader, 1, &vertexShaderSouce, NULL);
    glCompileShader(vertexShader);

    //检测是否编译成功
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << std::endl;
    }

    glShaderSource(fragmentShader, 1, &fragmentShaderSouce, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << std::endl;
    }

    //创建程序对象
    shaderProgram = glCreateProgram();

    //将着色器附加到程序对象上
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    //链接着色器
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM_LINK_FAILED\n"
                  << std::endl;
    }

    //删除着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // //线框模式
    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // glDrawArrays(GL_LINE, 0, 6);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
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