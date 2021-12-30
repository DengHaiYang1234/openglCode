#ifndef BUFFER_GROMETRY
#define BUFFER_GROMETRY

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <iostream>

using namespace std;

const float PI = glm::pi<float>();

struct Vertex
{
    glm::vec3 Postion;   //顶点坐标
    glm::vec3 Normal;    //发现
    glm::vec3 TexCoords; //纹理坐标
    glm::vec3 Tangent;   //切线
    glm::vec3 Bitangent; //副切线
};

class BufferGeometry
{
public:
    vector<Vertex> vertices;      //顶点集合
    vector<unsigned int> indices; //三角形顶点顺序
    unsigned int VAO;

    //打印参数信息
    void logParameters()
    {
        for (unsigned int i = 0; i < vertices.size(); i++)
        {
            // cout << "-----------------" << endl;
            // cout << "vertex ->> x: " << vertices[i].Position.x << ",y: " << vertices[i].Position.y << ",z: " << vertices[i].Position.z << endl;
            // cout << "normal ->> x: " << vertices[i].Normal.x << ",y: " << vertices[i].Normal.y << ",z: " << vertices[i].Normal.z << endl;
            // cout << "TexCoords ->> x: " << vertices[i].TexCoords.x << ",y: " << vertices[i].TexCoords.y << endl;
            // cout << "-----------------" << endl;
        }
    }

    // 计算切线向量并添加到顶点属性中
    void computeTangents()
    {
    }

    void dispose()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

private:
    glm::mat4 matrix = glm::mat4(1.0f);

protected:
    unsigned int VBO, EBO;

    void setupBuffers()
    {
        //生成 VAO VBO EBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        //绑定并填充顶点数据
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_DYNAMIC_DRAW);
        
        //绑定并填充三角形索引数据
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    }
};

#endif