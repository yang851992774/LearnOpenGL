#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        //GL_COLOR_BUFFER_BIT 颜色缓冲
        //GL_DEPTH_BUFFER_BIT //深度缓冲
        //GL_STENCIL_BUFFER_BIT //模版缓存
        glClear(GL_COLOR_BUFFER_BIT); //gl_color_buffer_bit

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        //交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上
        //Double Buffer双缓存概念，前缓存保存最终输出的图像，它在屏幕上显示，所有渲染指令都会在后缓存绘制，当所有渲染指令完成，我们交换前缓存和后缓存
        glfwSwapBuffers(window);
        //函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置
        glfwPollEvents();
    }

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
