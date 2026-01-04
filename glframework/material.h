#pragma once

#include "core.h"
#include <memory>

class Shader;
class Texture;

class Material {
public:
    Material();

    // 应用到着色器的方法
    void applyToShader(Shader& shader, const std::string& uniformName = "material") const;

    // Phong材质参数
    glm::vec3 m_vec3Ambient;     // 环境光反射系数
    glm::vec3 m_vec3Diffuse;     // 漫反射系数
    glm::vec3 m_vec3Specular;    // 镜面反射系数
    float m_fShininess;       // 光泽度（镜面反射指数）

    // 纹理（可选）
    std::shared_ptr<Texture> diffuseTexture;
    std::shared_ptr<Texture> specularTexture;

    // 是否使用纹理
    bool useDiffuseTexture() const { return diffuseTexture != nullptr; }
    bool useSpecularTexture() const { return specularTexture != nullptr; }

    // 预定义材质
    static Material* createEmerald();
    static Material* createGold();
    static Material* createPlastic();
    static Material* createDefault();

private:
    std::string m_name;
};