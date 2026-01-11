#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMarTrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec3 Normal;
out vec2 uv;
out vec3 FragPos;  
out vec3 worldPosition;

void main()
{
    vec4 transformPositon = vec4(aPos, 1.0);//转化为齐次坐标

    transformPositon = modelMatrix * transformPositon;
    worldPosition = transformPositon.xyz;

    gl_Position = projectionMatrix * viewMarTrix * transformPositon;
    uv = aUV;
    Normal =  normalMatrix * aNormal;
}