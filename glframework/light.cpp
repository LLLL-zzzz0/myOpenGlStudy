#include "light.h"
#include "shader.h"

int Light::m_siLightCount = 0;

Light::Light(LightType lightType):
    m_lightType(lightType),
    m_vec3Position(0.0f, 0.0f, 0.0f),
    m_vec3Driection(0.0f, -1.0f, 0.0f),
    m_vec3Ambient(0.1f, 0.1f, 0.1f),
    m_vec3Diffuse(1.0f, 1.0f, 1.0f),
    m_vec3specular(1.0f, 1.0f, 1.0f),
    m_iId(m_siLightCount++)
{
}

void Light::applyToShader(Shader& shaderProgram, const std::string& strUniformName) const
{
    std::string strBaseName = strUniformName + ".";

    shaderProgram.setInt(strBaseName + "type", static_cast<int>(m_lightType));

    shaderProgram.setVec3(strBaseName + "position", m_vec3Position);
    shaderProgram.setVec3(strBaseName + "direction", m_vec3Driection);

    //设置颜色
    shaderProgram.setVec3(strBaseName + "ambient", m_vec3Ambient);
    shaderProgram.setVec3(strBaseName + "diffuse", m_vec3Diffuse);
    shaderProgram.setVec3(strBaseName + "specular", m_vec3specular);

    // 设置衰减参数
    shaderProgram.setFloat(strBaseName + "constant", attenuation.constant);
    shaderProgram.setFloat(strBaseName + "linear", attenuation.linear);
    shaderProgram.setFloat(strBaseName + "quadratic", attenuation.quadratic);

    if (m_lightType == SPOTLIGHT)
    {
        shaderProgram.setFloat(strBaseName + "cutOff", spotParams.cutOff);
        shaderProgram.setFloat(strBaseName + "outerCutOff", spotParams.outerCutOff);
    }
}
