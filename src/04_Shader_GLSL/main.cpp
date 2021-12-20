#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const char *vertexShaderSouce = "#version 330 core\n"
                                "layout (location = 0) in vec3 aPos;\n"
                                "out vec4 vColor;\n"
                                "void main()\n"
                                "{\n"
                                "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                "vColor =  vec4(1.0,0.0f,0.5f,1.0f);\n"
                                "   gl_PointSize = 20.0f;\n"
                                "}\0";

const char *fragmentShaderSouce1 = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec4 vColor;"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vColor;\n"
                                   "}\n\0";

const char *fragmentShaderSouce2 = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
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

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported:" << nrAttributes << std::endl;

    //定义顶点数组
    float vertexs[] = {
        //第一个三角形
        -0.5f, 0.5f, 0.0f,
        -0.75f, -0.5f, 0.0f,
        -0.25f, -0.5f, 0.0f,

        //第二个三角形
        0.5f, 0.5f, 0.0f,
        0.75f, -0.5f, 0.0f,
        0.25f, -0.5f, 0.0f};

    unsigned int indexes[] =
        {
            0, 1, 3,  // 第一个三角形
            1, 2, 3}; // 第二个三角形

    // VAO:CPU发送顶点数据到GPU中后，在GPU内存中会存储这些顶点数据，方便渲染时GPU立即访问到这些顶点。
    //     从CPU放松数据到显卡相对较慢，所以一次尽可能发送多的数据，交由GPU来保存

    // VBO：用来封装所有和定点处理器相关的数据的特殊对象，它并不保存实际数据，而是放顶点数据，顶点颜色，索引缓冲等的状态配置；
    //      类似于Unity中的mesh

    unsigned int VBOS[2], VAOS[2];
    glGenBuffers(2, VBOS);
    glGenVertexArrays(2, VAOS);

    //绑定VAO对象
    glBindVertexArray(VAOS[0]);

    //绑定VBO对象
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]); //只需绑定一次就可以

    //填充VBO数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);

    //解析顶点数据 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    //绑定VAO对象
    glBindVertexArray(VAOS[1]);

    //绑定VBO对象
    glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]); //只需绑定一次就可以

    //填充VBO数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);

    //解析顶点数据 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(9 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    //创建顶点着色器
    unsigned int vertexShader, fragmentShader1, fragmentShader2, shaderProgram1, shaderProgram2;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);

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

    glShaderSource(fragmentShader1, 1, &fragmentShaderSouce1, NULL);
    glCompileShader(fragmentShader1);

    glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT1::COMPILATION_FAILED\n"
                  << std::endl;
    }

    glShaderSource(fragmentShader2, 1, &fragmentShaderSouce2, NULL);
    glCompileShader(fragmentShader2);

    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT2::COMPILATION_FAILED\n"
                  << std::endl;
    }

    //创建程序对象
    shaderProgram1 = glCreateProgram();
    shaderProgram2 = glCreateProgram();

    //将着色器附加到程序对象上
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glAttachShader(shaderProgram2, fragmentShader2);

    //链接着色器
    glLinkProgram(shaderProgram1);
    glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM_LINK_FAILED\n"
                  << std::endl;
    }

    glLinkProgram(shaderProgram2);
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER_PROGRAM_LINK_FAILED\n"
                  << std::endl;
    }

    //删除着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1);
    glDeleteShader(fragmentShader2);
    // //线框模式
    // glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram1);
        glBindVertexArray(VAOS[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram2);
        glBindVertexArray(VAOS[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawArrays(GL_LINE, 0, 6);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAOS);
    glDeleteBuffers(2, VBOS);
    glDeleteProgram(shaderProgram1);
    glDeleteProgram(shaderProgram2);
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