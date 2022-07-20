#version 330 core
out vec4 FragColor;

uniform vec2 mouse;

void main()
{
    vec2 dist = gl_FragCoord.xy - mouse;
    if (length(dist) < 4) {
        FragColor = vec4(1.0f,0.0f,0.0f,0.5f);
    } else {
        FragColor = vec4(0.0f,0.7f,0.0f,0.5f);
    }
}