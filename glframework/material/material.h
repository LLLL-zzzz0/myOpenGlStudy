#pragma once

#include "../core.h"

enum class MaterialType
{
    PhongMaterial,
    WhiteMaterial
};

class Material 
{
public:
    Material();
    virtual ~Material();
    MaterialType getMaterialType();

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

protected:
    MaterialType m_enumType{ MaterialType::PhongMaterial };
};