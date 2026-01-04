#include "material.h"
#include "texture.h"
#include "shader.h"

Material::Material()
    : m_vec3Ambient(0.1f, 0.1f, 0.1f)
    , m_vec3Diffuse(0.8f, 0.8f, 0.8f)
    , m_vec3Specular(0.5f, 0.5f, 0.5f)
    , m_fShininess(32.0f) {
}

void Material::applyToShader(Shader& shader, const std::string& uniformName) const {
    std::string base = uniformName + ".";

    // 设置材质参数
    shader.setVec3(base + "ambient", m_vec3Ambient);
    shader.setVec3(base + "diffuse", m_vec3Diffuse);
    shader.setVec3(base + "specular", m_vec3Specular);
    shader.setFloat(base + "shininess", m_fShininess);

    // 设置纹理标志
    shader.setBool(base + "useDiffuseTexture", useDiffuseTexture());
    shader.setBool(base + "useSpecularTexture", useSpecularTexture());

    // 绑定纹理（如果存在）
    if (useDiffuseTexture()) {
        diffuseTexture->bind();
        shader.setInt(base + "diffuseTexture", 0); // 纹理单元0
    }

    if (useSpecularTexture()) {
        specularTexture->bind();
        shader.setInt(base + "specularTexture", 1); // 纹理单元1
    }
}

// 预定义材质（常见材质参数）
Material* Material::createEmerald() {
    Material *mat = new Material();
    mat->m_vec3Ambient = glm::vec3(0.0215f, 0.1745f, 0.0215f);
    mat->m_vec3Diffuse = glm::vec3(0.07568f, 0.61424f, 0.07568f);
    mat->m_vec3Specular = glm::vec3(0.633f, 0.727811f, 0.633f);
    mat->m_fShininess = 0.6f * 128.0f;
    return mat;
}

Material* Material::createGold() {
    Material* mat = new Material();
    mat->m_vec3Ambient = glm::vec3(0.24725f, 0.1995f, 0.0745f);
    mat->m_vec3Diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f);
    mat->m_vec3Specular = glm::vec3(0.628281f, 0.555802f, 0.366065f);
    mat->m_fShininess = 0.4f * 128.0f;
    return mat;
}

Material* Material::createPlastic() {
    Material* mat = new Material();
    mat->m_vec3Ambient = glm::vec3(0.0f, 0.0f, 0.0f);
    mat->m_vec3Diffuse = glm::vec3(0.55f, 0.55f, 0.55f);
    mat->m_vec3Specular = glm::vec3(0.70f, 0.70f, 0.70f);
    mat->m_fShininess = 0.25f * 128.0f;
    return mat;
}

Material* Material::createDefault() {
    Material* mat = new Material();
    return mat;
}