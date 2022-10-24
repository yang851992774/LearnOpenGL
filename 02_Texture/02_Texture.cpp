/*
 学习目的：
 加载贴图，
 为顶点坐标设置uv
 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"

#include <iostream>
#include <cmath>

#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// stores how much we're seeing of either texture
float mixValue = 1.0f;

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
    Shader ourShader("shader/02_texture/textureUV.vs","shader/02_texture/textureUV.fs");
    
//    float vertices[] = {
//        // 位置              // 颜色               //uv
//         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   0.0f,0.0f,  // 右下
//        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f,0.0f,  // 左下
//         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.5f,1.0f   // 顶部
//    };
//
//    unsigned int indices[] = {  // note that we start from 0!
//        0, 1, 2  // first Triangle
//    };
    
    float vertices[] = {
    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };
    
//    float vertices[] = {
//          // positions          // colors           // texture coords (note that we changed them to 'zoom in' on our texture image)
//           0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.55f, 0.55f, // top right
//           0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.55f, 0.45f, // bottom right
//          -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.45f, 0.45f, // bottom left
//          -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.45f, 0.55f  // top left
//      };
    
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    
    
    unsigned int VBO,VAO,EBO;
    
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
    //attribute1 posiiton
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8* sizeof(float),(void *)0);
    glEnableVertexAttribArray(0);
    
    //attribute2 color
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8* sizeof(float),(void *)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    
    //attribute3 uv
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8* sizeof(float),(void *)(6* sizeof(float)));
    glEnableVertexAttribArray(2);
    
    //load texture1
    unsigned int texture1;
    glGenTextures(1,&texture1);
    glBindTexture(GL_TEXTURE_2D,texture1);
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,GL_NEAREST);
    
    int width,height,nrChannels;
    unsigned char *data = stbi_load("img/container.jpg", &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "FAILED TO LOAD TEXTURE " << std::endl;
    }
    stbi_image_free(data);
    
    
    //load texture2
    unsigned int texture2;
    glGenTextures(1,&texture2);
    glBindTexture(GL_TEXTURE_2D,texture2);
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,GL_LINEAR);
    
    //加载图片时翻转y轴
//    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("img/awesomeface.png", &width, &height, &nrChannels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "FAILED TO LOAD TEXTURE " << std::endl;
    }
    stbi_image_free(data);
    
    //在设置uniform 必须激活/use Shader
    ourShader.use();
    //(GLint location, GLint v0)
    // either set it manually like so:
//    glUniform1i(glGetUniformLocation(ourShader.ID, "ourTexture"), 0);
    // or set it via the texture class
    ourShader.setInt("ourTexture", 0);
    ourShader.setInt("ourTexture2", 1);
    
//    glBindBuffer(GL_ARRAY_BUFFER,0);
//    glBindVertexArray(0);
    
//    std::cout << "link" << std::endl;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        ourShader.use();
        ourShader.setFloat("mixValue", mixValue);
        
        glBindVertexArray(VAO);
//        glDrawArrays(GL_TRIANGLES,0,3);
        
//        GLenum mode, GLsizei count, GLenum type, const void *indices
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);


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
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
     {
         mixValue += 0.005f; // change this value accordingly (might be too slow or too fast based on system hardware)
         if(mixValue >= 1.0f)
             mixValue = 1.0f;
     }
     if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
     {
         mixValue -= 0.005f; // change this value accordingly (might be too slow or too fast based on system hardware)
         if (mixValue <= 0.0f)
             mixValue = 0.0f;
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
