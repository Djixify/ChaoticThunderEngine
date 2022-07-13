#version 330 core
out vec4 FragColor;

uniform float time;

void main()
{
    FragColor = vec4(0.0f,0.0f,0.6f+(sin(time) * 0.4),1.0f);
} 