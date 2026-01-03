#version 330 core
out vec4 FragColor;
in vec3 color;
uniform float time;
void main()
{
    FragColor = vec4(color * (sin(time) + 1.0) / 2.0, 1.0f);
}