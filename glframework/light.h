#pragma once

#include "core.h"

class Shader;

class Light
{
public:
	enum LightType
	{
		DIRECTIONALLIGHT,
		POINTLIGHT,
		SPOTLIGHT
	};

	Light(LightType lightType = DIRECTIONALLIGHT);

	void applyToShader(Shader& shaderProgram, const std::string& strUniformName) const;

	LightType m_lightType; //光照类型
	glm::vec3 m_vec3Position; //位置
	glm::vec3 m_vec3Driection; //方向

	glm::vec3 m_vec3Ambient; //环境光
	glm::vec3 m_vec3Diffuse; //漫反射光
	glm::vec3 m_vec3specular; //镜面反射光

	//衰减参数
	struct Attenuation {
		float constant;    // 常数衰减
		float linear;      // 线性衰减
		float quadratic;   // 二次衰减

		Attenuation() : constant(1.0f), linear(0.09f), quadratic(0.032f) {}
	} attenuation;

	// 聚光灯参数
	struct SpotParams {
		float cutOff;        // 内切角（余弦值）
		float outerCutOff;   // 外切角（余弦值）

		SpotParams() :
			cutOff(glm::cos(glm::radians(12.5f))),
			outerCutOff(glm::cos(glm::radians(17.5f))) {
		}
	} spotParams;

	// 工具方法
	bool isDirectionalLight() const { return m_lightType == DIRECTIONALLIGHT; }
	bool isPointLight() const { return m_lightType == POINTLIGHT; }
	bool isSpotLight() const { return m_lightType == SPOTLIGHT; }

private:
	static int m_siLightCount; // 用于生成唯一ID
	int m_iId;               // 光源唯一标识
};