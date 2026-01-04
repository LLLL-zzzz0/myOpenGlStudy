#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

uniform mat4 transform;
uniform mat4 viewMarTrix;
uniform mat4 projectionMatrix;

out vec3 Normal;
out vec2 uv;
out vec3 FragPos;  
void main()
{
    vec4 vPos = vec4(aPos, 1.0);
    vec4 transPotion = projectionMatrix * viewMarTrix * transform * vPos;
    FragPos = vec3(transform * vec4(aPos, 1.0));
    gl_Position = transPotion;
    uv = aUV;
    Normal = aNormal;
}