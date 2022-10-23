#version 330 core
out vec4 FragColor;
//in vec3 ourColor;
in vec3 ourPosColor;

void main()
{
    FragColor = vec4(ourPosColor,1.0);
}
