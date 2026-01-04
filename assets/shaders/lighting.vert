#version 330 core

// 输入
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;  // 新增：法线输入

// 输出
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;  // 片段在世界空间中的位置

// 变换矩阵
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
    // 计算裁剪空间位置
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    // 传递纹理坐标
    TexCoord = aTexCoord;
    
    // 计算世界空间中的位置（用于光照计算）
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // 计算法线矩阵（处理非均匀缩放）
    // 注意：这里我们传递的是逆转置的左上3x3矩阵
    // 在实际中，应该在CPU计算好法线矩阵后传递过来
    // 这里为了简单，我们假设模型没有非均匀缩放
    Normal = mat3(transpose(inverse(model))) * aNormal;
}