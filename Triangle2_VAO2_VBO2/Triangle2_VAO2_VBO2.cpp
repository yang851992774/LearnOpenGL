/*
 目的：
 创建相同的两个三角形，但对它们的数据使用不同的VAO和VBO：
 */
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

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
    
    //如果使用的是Mac OS X系统，你还需要加下面这行代码到你的初始化代码中这些配置才能起作用（将上面的代码解除注释）
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
    //通知GLFW将我们窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);
    //设置窗口大小改变回掉
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    //GLAD是用来管理OpenGL的函数指针的
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    //我们给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //build and compile our shader program
    // ---------------------------------------
    //vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);//第二参数指定了传递的源码字符串数量
    glCompileShader(vertexShader);
    
    int success;
    char info[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader,512,NULL,info);
        std::cout <<  "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info << std::endl;
    }
    
    //fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);//第二参数指定了传递的源码字符串数量
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader,512,NULL,info);
        std::cout <<  "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info << std::endl;
    }
    
    //link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram,512,NULL,info);
        std::cout <<  "ERROR::SHADER::PROGRAM::LINK_FAIL\n" << info << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    float vertices[] =
    {
        // first triangle
       -0.9f, -0.5f, 0.0f,  // left
       -0.0f, -0.5f, 0.0f,  // right
       -0.45f, 0.5f, 0.0f  // top
      
    };
    float vertices1[] =
    {
       // second triangle
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top
    };
    
    unsigned int VBOs[2],VAOs[2];
    
    glGenVertexArrays(2,VAOs);
    glGenBuffers(2,VBOs);
    
    //初始化代码（只运行一次 (除非你的物体频繁改变)）
    glBindVertexArray(VAOs[0]);//接下来VBO都绑定注册到VAO中去了
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3* sizeof(float),(void *)0);
    glEnableVertexAttribArray(0);
    
    
    glBindVertexArray(VAOs[1]);//接下来VBO都绑定注册到VAO中去了
    glBindBuffer(GL_ARRAY_BUFFER,VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices1),vertices1,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void *)0);
    glEnableVertexAttribArray(0);
    
    //解绑 VBO VAO 都允许解绑
    //注意，这是允许的，对 glVertexAttribPointer 的调用将VBO注册为顶点属性的绑定顶点缓冲区对象，这样之后我们就可以安全地解除绑定
//    glBindBuffer(GL_ARRAY_BUFFER,0); //
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
//    glBindVertexArray(0);
    

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input 监听输出
        // -----
        processInput(window);

        // render
        //自定义的颜色清空屏幕
        //调用了glClearColor来设置清空屏幕所用的颜色。
        //当调用glClear函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
        
        //glClearColor函数是一个“状态设置”函数，而glClear函数则是一个“状态使用”的函数，它使用了当前的状态来获取应该清除为的颜色
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //GL_COLOR_BUFFER_BIT 颜色缓冲
        //GL_DEPTH_BUFFER_BIT //深度缓冲
        //GL_STENCIL_BUFFER_BIT //模版缓存
        glClear(GL_COLOR_BUFFER_BIT); //gl_color_buffer_bit
        
        glUseProgram(shaderProgram);
        glBindVertexArray(VAOs[0]);
        //第一个参数是我们打算绘制的OpenGL图元的类型
        //GL_POINTS、GL_TRIANGLES、GL_LINE_STRIP
        //2、第二个参数指定了顶点数组的起始索引
        //3、指定我们打算绘制多少个顶点
        glDrawArrays(GL_TRIANGLES,0,3);
        
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES,0,3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        //交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上
        //Double Buffer双缓存概念，前缓存保存最终输出的图像，它在屏幕上显示，所有渲染指令都会在后缓存绘制，当所有渲染指令完成，我们交换前缓存和后缓存
        glfwSwapBuffers(window);
        //函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(2,VAOs);
    glDeleteBuffers(2,VBOs);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    //需要正确释放/删除之前的分配的所有资源
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
// 每当窗口大小改变(通过操作系统或用户调整大小) ，这个回调函数就会执行
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    //确保视窗与新窗口尺寸相匹配; 注意宽度和高度将明显大于视网膜显示器上指定的尺寸。
    //glViewport函数前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）。
    glViewport(0, 0, width, height);
}
