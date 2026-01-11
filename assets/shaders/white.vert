#version 460 core
layout(location = 0) in vec3 aPos;

uniform mat4 modelMatrix;
uniform mat4 viewMarTrix;
uniform mat4 projectionMatrix;

void main()
{
    vec4 transformPositon = vec4(aPos, 1.0);//转化为齐次坐标
    transformPositon = modelMatrix * transformPositon;
    gl_Position = projectionMatrix * viewMarTrix * transformPositon;
}