#pragma once

#include "core.h"

class Geometry
{
public:
	Geometry();
	~Geometry();

	static Geometry* createBox(float fSize);
	static Geometry* createSphere(float fSize);

	GLuint getVao()const { return m_uiVao; }
	uint32_t getIndicesCount()const { return m_uiIndicesCount; }
private:
	GLuint m_uiVao{ 0 };
	GLuint m_uiPosVbo{ 0 };
	GLuint m_uiUvVbo{ 0 };
	GLuint m_uiEbo{ 0 };

	uint32_t m_uiIndicesCount{ 0 };
};