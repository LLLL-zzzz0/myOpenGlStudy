#pragma once

#define MAX_LIGHT_NUM 5

#include "../core.h"
#include "../../Application/camera/camera.h"
#include "../light/directionalLight.h"
#include "../light/ambientLight.h"
#include "../light/pointLight.h"
#include "../light/spotLight.h"
#include "../shader.h"
#include <vector>
#include "../../wrapper/checkError.h"

enum class MaterialType
{
    PhongMaterial,
    WhiteMaterial,
    DepthMaterial,
    OpacityMaskMaterial
};

class Material 
{
public:
    Material();
    virtual ~Material();
    MaterialType getMaterialType();
	virtual void bind(
        Shader* pShader,
        Camera* pCamera,
        glm::mat3 normalMatrix,
		const std::vector<SpotLight*>& vctSpotLight,
		const std::vector<DirectionalLight*>& vctDirectionalLight,
		const std::vector<PointLight*>& vctPointLight,
		AmbientLight* pAmbientLight
    )
    {
    }

    void setOpenGLStatus();

    //深度检测相关
	bool          m_bDepthTest{ true };
	bool          m_bDepthWrite{ true };
    GLenum        m_enumDepthFunc{ GL_LESS };
    //polygonOffset
    bool          m_bPolygonOffset{ false };
    unsigned int  m_uiPolygonOffsetType{GL_POLYGON_OFFSET_FILL};
	float         m_fDepthFactor{ 0.0f };
	float         m_fDepthUnit{ 0.0f };
    //模板测试
    bool          m_bStencilTest{ false };
	unsigned int  m_uiSFail{ GL_KEEP };
	unsigned int  m_uiZFail{ GL_KEEP };
	unsigned int  m_uiZPass{ GL_KEEP };
    unsigned int  m_uiStencilMask{ 0xFF };
    unsigned int  m_uiStencilFunc{ GL_ALWAYS };
    unsigned int  m_uiStencilRef{ 0 };
    unsigned int  m_uiStencilFuncMask{ 0xFF };
    //颜色混合
    bool          m_bBlend{ false };
	unsigned int  m_uiSBlendFactor{GL_SRC_ALPHA};
	unsigned int  m_uiDBlendFactor{GL_ONE_MINUS_SRC_ALPHA};

    float         m_fOpacity{ 1.0f };
    //面剔除
    bool          m_bFaceCulling{ false };
	unsigned int  m_uiFrontFace{ GL_CCW };
	unsigned int  m_uiCullFace{ GL_BACK };

protected:
    MaterialType m_enumType{ MaterialType::PhongMaterial };
	void setShaderSpotLight(Shader* pShader, const std::vector<SpotLight*>& vctSpotLight);
	void setShaderDirLight(Shader* pShader, const std::vector<DirectionalLight*>& vctDirectionalLight);
	void setShaderPointLight(Shader* pShader, const std::vector<PointLight*>& vctPointLight);
    void setDepthState();
    void setPolygomOffsetState();
    void setStencilState();
    void setBlendState();
    void setFaceCulling();
};