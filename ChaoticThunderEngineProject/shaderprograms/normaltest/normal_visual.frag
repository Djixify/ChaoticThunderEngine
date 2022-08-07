#version 330 core
out vec4 FragColor;

in GS_OUT {
	vec4 color;
} gs_in;

void main()
{
    FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}