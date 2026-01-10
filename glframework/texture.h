#pragma once
#include "core.h"
#include <string>
#include "../wrapper/checkError.h"

class Texture
{
public:
	using Ptr = std::shared_ptr<Texture>;

	//统一使用createTexture和createTextureFromMemory，便于缓存
	static Ptr createTexture(const std::string& strPath, unsigned int uiUnit);
	static Ptr createTextureFromMemory(
		const std::string& strPath,
		unsigned int uiUnit,
		unsigned char* pdataIn,
		uint32_t uiWidth,
		uint32_t uiHeight);

	
	~Texture();
	void bind();

	void setUnit(unsigned int uiUnit) 
	{
		if (uiUnit > 15)
		{
			m_uiUnit = 15;
			return;
		}
		m_uiUnit = uiUnit;
	}

	int getWidth() const
	{
		return m_iWidth;
	}
	int getHeight() const
	{
		return m_iHeight;
	}

private:
	//统一使用createTexture和createTextureFromMemory，便于缓存
	Texture(const std::string& strPath, unsigned int uiUnit);
	Texture(
		unsigned int uiUnit,
		unsigned char* pdataIn,
		uint32_t uiWidth,
		uint32_t uiHeight
	);
	GLuint m_texture{ 0 };
	int m_iWidth{ 0 };
	int m_iHeight{ 0 };
	unsigned int m_uiUnit{ 0 }; //纹理单元
	static std::unordered_map<std::string, std::weak_ptr<Texture>> m_mapTextureCache;
};