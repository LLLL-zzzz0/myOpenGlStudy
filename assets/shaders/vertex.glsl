#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;

uniform float time;
uniform mat4 transform;
uniform mat4 viewMarTrix;
uniform mat4 projectionMatrix;


out vec3 color;
out vec2 uv;
void main()
{
    vec4 vPos = vec4(aPos, 1.0);
    vec4 transPotion = projectionMatrix * viewMarTrix * transform * vPos;
    gl_Position = transPotion;
    uv = aUV;
}