#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 uv;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;
uniform float mixValue;

void main()
{
//    FragColor = vec4(ourColor,1.0);
//    FragColor = texture(ourTexture2,uv);
    //* vec4(ourColor,1.0);
    
    FragColor = mix(texture(ourTexture, uv), texture(ourTexture2, uv), mixValue);
}
