/*
 学习目的：
 通过glVertexAttribPointer 设置2个顶点属性
 1、顶点坐标。aPos
 2、顶点颜色。aColor
 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"

#include <iostream>
#include <cmath>


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <fstream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    //主版本号(Major)和次版本号(Minor) OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//context_version_major   主要的
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//context_version_minor   次要的
    //同样明确告诉GLFW我们使用的是核心模式(Core-profile)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //opengl_profile、opengl_core_profile

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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //build and compile our shader program
    // ---------------------------------------
    Shader ourShader("shader/01_vertex_color/shader.vs","shader/01_vertex_color/shader.fs");
    
    float vertices[] = {
        // 位置              // 颜色
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };
    
    unsigned int VBO,VAO;
    
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    
    //OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
    //1、第一个参数指定我们要配置的顶点属性。还记得我们在顶点着色器中使用layout(location = 0)定义了position顶点属性的位置值(Location)吗？
    //2、第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3
    //3、第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)
    //4、参数定义我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为GL_FALSE
    //5、第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个float之后，我们把步长设置为3 * sizeof(float)。
    //6、最后一个参数的类型是void*，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6* sizeof(float),(void *)0);
    glEnableVertexAttribArray(0);
    
    //设置顶点颜色属性
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6* sizeof(float),(void *)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        ourShader.use();
        
        //练习2
//        ourShader.setFloat("offset", 0.4f);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    //
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
