#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aUV;

out vec3 ourColor;
out vec2 uv;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    ourColor = aColor;
    
// stbi_set_flip_vertically_on_load(true); 在cpu中转换
//    uv = aUV;
    
//在gpu UV 转换
//    uv = vec2(aUV.x,1.0-aUV.y);
    
//练习1
//    uv = vec2(1.0-aUV.x,1.0-aUV.y);
    
    //练习2 试试看能不能在箱子的角落放置4个笑脸
        uv = vec2(1.0-aUV.x,1.0-aUV.y) * 2.0;
}
