#pragma once
#include "core.h"
#include <string>
#include "../wrapper/checkError.h"

class Texture
{
public:
	Texture(const std::string& strPath, unsigned int uiUnit);
	~Texture();
	void bind();

	int getWidth() const
	{
		return m_iWidth;
	}
	int getHeight() const
	{
		return m_iHeight;
	}

private:
	GLuint m_texture{ 0 };
	int m_iWidth{ 0 };
	int m_iHeight{ 0 };
	unsigned int m_uiUnit{ 0 }; //ÎÆÀíµ¥Ôª
};