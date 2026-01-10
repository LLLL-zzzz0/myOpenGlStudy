#pragma once

#include "core.h"

class Geometry
{
public:
	Geometry();
	Geometry(
		const std::vector<float>& vctPositions,
		const std::vector<float>& vctNormals,
		const std::vector<float>& vctUvs,
		const std::vector<unsigned int>& vctIndices
	);
	~Geometry();

	static Geometry* createBox(float fSize);
	static Geometry* createSphere(const float& fRadius, const int & iLatLinesNum = 60, const int& iLongLinesNum = 60);
	static Geometry* createPlane(float fWidth, float fHeight);

	GLuint getVao()const { return m_uiVao; }
	uint32_t getIndicesCount()const { return m_uiIndicesCount; }
private:
	GLuint m_uiVao{ 0 };
	GLuint m_uiPosVbo{ 0 };
	GLuint m_uiUvVbo{ 0 };
	GLuint m_uiNormalVbo{ 0 };  
	GLuint m_uiEbo{ 0 };

	uint32_t m_uiIndicesCount{ 0 };
};