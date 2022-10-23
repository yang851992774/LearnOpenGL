#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
//uniform float offset;

//使用out关键字把顶点位置输出到片段着色器，并将片段的颜色设置为与顶点位置相等（来看看连顶点位置值都在三角形中被插值的结果）。做完这些后，尝试回答下面的问题：为什么在三角形的左下角是黑的?
// 因为openGL的NDC 坐标为[-1，1],三角形的左下角坐标为 (a <=0,b <=0 ,c=0)  ,颜色的的r,g,b,a的分量取值范围为 [0,1],所以，三角形的左下角的颜色为(0,0,0)，即为黑色

//out vec3 ourColor;
out vec3 ourPosColor;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    ourPosColor = aPos;
//    着色器练习1、修改顶点着色器让三角形上下颠倒
//    gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0);
    
//    着色器练习2、使用uniform定义一个水平偏移量，在顶点着色器中使用这个偏移量把三角形移动到屏幕右侧
//    gl_Position = vec4(aPos.x+offset, aPos.y, aPos.z, 1.0);
    
//    ourColor = aColor;
}
