#version 330 core
in vec3 aPos;
in vec3 aColor;
uniform float time;
uniform float speed;

out vec3 color;
void main()
{
    float fOffset = sin(time * speed) * 0.3;
    gl_Position = vec4(aPos.x + fOffset, aPos.y, aPos.z, 1.0);
    color = aColor;
}