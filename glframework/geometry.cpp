#include "geometry.h"
#include <vector>

Geometry::Geometry()
{
}

Geometry::Geometry(
    const std::vector<float>& vctPositions,
    const std::vector<float>& vctNormals,
    const std::vector<float>& vctUvs,
    const std::vector<unsigned int>& vctIndices)
{
    m_uiIndicesCount = vctIndices.size();

	glGenBuffers(1, &m_uiPosVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiPosVbo);
	glBufferData(GL_ARRAY_BUFFER, vctPositions.size() * sizeof(float), vctPositions.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_uiNormalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiNormalVbo);
	glBufferData(GL_ARRAY_BUFFER, vctNormals.size() * sizeof(float), vctNormals.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_uiUvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiUvVbo);
	glBufferData(GL_ARRAY_BUFFER, vctUvs.size() * sizeof(float), vctUvs.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_uiEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vctIndices.size() * sizeof(unsigned int), vctIndices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_uiVao);
	glBindVertexArray(m_uiVao);

	glBindBuffer(GL_ARRAY_BUFFER, m_uiPosVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_uiUvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_uiNormalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiEbo);

	glBindVertexArray(0);
}

Geometry::~Geometry()
{
	if (m_uiVao != 0)
	{
		glDeleteVertexArrays(1, &m_uiVao);
	}

	if (m_uiPosVbo != 0)
	{
		glDeleteBuffers(1, &m_uiPosVbo);
	}

    if (m_uiNormalVbo != 0)
    {
        glDeleteBuffers(1, &m_uiNormalVbo);
    }

	if (m_uiUvVbo != 0)
	{
		glDeleteBuffers(1, &m_uiUvVbo);
	}

	if (m_uiEbo != 0)
	{
		glDeleteBuffers(1, &m_uiEbo);
	}
}

Geometry* Geometry::createBox(float fSize)
{
	Geometry* pBox = new Geometry();
    pBox->m_uiIndicesCount = 36;
	float fHalfSize = fSize / 2.0f;

    float positions[] = {
        // Front face
        -fHalfSize, -fHalfSize, fHalfSize, fHalfSize, -fHalfSize, fHalfSize, fHalfSize, fHalfSize, fHalfSize, -fHalfSize, fHalfSize, fHalfSize,
        // Back face
        -fHalfSize, -fHalfSize, -fHalfSize, -fHalfSize, fHalfSize, -fHalfSize, fHalfSize, fHalfSize, -fHalfSize, fHalfSize, -fHalfSize, -fHalfSize,
        // Top face
        -fHalfSize, fHalfSize, fHalfSize, fHalfSize, fHalfSize, fHalfSize, fHalfSize, fHalfSize, -fHalfSize, -fHalfSize, fHalfSize, -fHalfSize,
        // Bottom face
        -fHalfSize, -fHalfSize, -fHalfSize, fHalfSize, -fHalfSize, -fHalfSize, fHalfSize, -fHalfSize, fHalfSize, -fHalfSize, -fHalfSize, fHalfSize,
        // Right face
        fHalfSize, -fHalfSize, fHalfSize, fHalfSize, -fHalfSize, -fHalfSize, fHalfSize, fHalfSize, -fHalfSize, fHalfSize, fHalfSize, fHalfSize,
        // Left face
        -fHalfSize, -fHalfSize, -fHalfSize, -fHalfSize, -fHalfSize, fHalfSize, -fHalfSize, fHalfSize, fHalfSize, -fHalfSize, fHalfSize, -fHalfSize
    };

    float uv[] = {
        0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
        0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
        0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
        0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
        0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
        0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
    };

    float normals[] = {
        //前面
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        //后面
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        //上面
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        //下面
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        //右面
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        //左面
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
    };

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,   // Front face
        4, 5, 6, 6, 7, 4,   // Back face
        8, 9, 10, 10, 11, 8,  // Top face
        12, 13, 14, 14, 15, 12, // Bottom face
        16, 17, 18, 18, 19,  16, // Right face
        20, 21, 22, 22, 23, 20  // Left face
    };

    glGenBuffers(1, &pBox->m_uiPosVbo);
    glBindBuffer(GL_ARRAY_BUFFER, pBox->m_uiPosVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glGenBuffers(1, &pBox->m_uiUvVbo);
    glBindBuffer(GL_ARRAY_BUFFER, pBox->m_uiUvVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);

    glGenBuffers(1, &pBox->m_uiNormalVbo);
    glBindBuffer(GL_ARRAY_BUFFER, pBox->m_uiNormalVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

    glGenBuffers(1, &pBox->m_uiEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pBox->m_uiEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



    glGenVertexArrays(1, &pBox->m_uiVao);
    glBindVertexArray(pBox->m_uiVao);

    glBindBuffer(GL_ARRAY_BUFFER, pBox->m_uiPosVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, pBox->m_uiUvVbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, pBox->m_uiNormalVbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pBox->m_uiEbo);

    glBindVertexArray(0);

	return pBox;
}

Geometry* Geometry::createSphere(const float& fRadius, const int& iLatLinesNum, const int& iLongLinesNum)
{
    Geometry* pSphere = new Geometry();

    //声明变量 pos uv 索引
    std::vector<GLfloat> vctPosition;
    std::vector<GLfloat> vctUv;
    std::vector<GLfloat> vctNormal;
    std::vector<GLuint> vctIndices;

    //生成位置，uv
    for (int i = 0; i <= iLatLinesNum; i++)
    {
        for (int j = 0; j <= iLongLinesNum; j++)
        {
            float fPhi = i * glm::pi<float>() / iLatLinesNum;
            float fTheta = j * 2 * glm::pi<float>() / iLongLinesNum;

            float fYPos = fRadius * cos(fPhi);
            float fXPos = fRadius * sin(fPhi) * cos(fTheta);
            float fZPos = fRadius * sin(fPhi) * sin(fTheta);

            vctPosition.push_back(fXPos);
            vctPosition.push_back(fYPos);
            vctPosition.push_back(fZPos);

            float fU = 1.0f - (float)j / (float)iLongLinesNum;
            float fV = 1.0f - (float)i / (float)iLatLinesNum;

            vctUv.push_back(fU);
            vctUv.push_back(fV);

            vctNormal.push_back(fXPos);
            vctNormal.push_back(fYPos);
            vctNormal.push_back(fZPos);
        }
    }

    //生成顶点索引
    for (int i = 0; i < iLatLinesNum; i++)
    {
        for (int j = 0; j < iLongLinesNum; j++)
        {
            int iFirstIndex = i * (iLongLinesNum + 1) + j;
            int iSecondIndex = iFirstIndex + iLongLinesNum + 1;
            int iThirdIndex = iFirstIndex + 1;
            int iFourthIndex = iSecondIndex + 1;

            vctIndices.push_back(iFirstIndex);
            vctIndices.push_back(iSecondIndex);
            vctIndices.push_back(iThirdIndex);

            vctIndices.push_back(iThirdIndex);
            vctIndices.push_back(iSecondIndex);
            vctIndices.push_back(iFourthIndex);
        }
    }

    glGenBuffers(1, &pSphere->m_uiPosVbo);
    glBindBuffer(GL_ARRAY_BUFFER, pSphere->m_uiPosVbo);
    glBufferData(GL_ARRAY_BUFFER, vctPosition.size() * sizeof(GLfloat), vctPosition.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &pSphere->m_uiNormalVbo);
    glBindBuffer(GL_ARRAY_BUFFER, pSphere->m_uiNormalVbo);
    glBufferData(GL_ARRAY_BUFFER, vctNormal.size() * sizeof(GLfloat), vctNormal.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &pSphere->m_uiUvVbo);
    glBindBuffer(GL_ARRAY_BUFFER, pSphere->m_uiUvVbo);
    glBufferData(GL_ARRAY_BUFFER, vctUv.size() * sizeof(GLfloat), vctUv.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &pSphere->m_uiEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pSphere->m_uiEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vctIndices.size() *  sizeof(GLuint), vctIndices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &pSphere->m_uiVao);
    glBindVertexArray(pSphere->m_uiVao);

    glBindBuffer(GL_ARRAY_BUFFER, pSphere->m_uiPosVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, pSphere->m_uiUvVbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, pSphere->m_uiNormalVbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pSphere->m_uiEbo);

    glBindVertexArray(0);

    pSphere->m_uiIndicesCount = vctIndices.size();

	return pSphere;
}

Geometry* Geometry::createPlane(float fWidth, float fHeight) {
    Geometry* pPlane = new Geometry();
    pPlane->m_uiIndicesCount = 6;

    float fHalfW = fWidth / 2.0f;
    float fHalfH = fHeight / 2.0f;

    float positions[] = 
    {
        -fHalfW, -fHalfH, 0.0f,
        fHalfW, -fHalfH, 0.0f,
        fHalfW, fHalfH, 0.0f,
        -fHalfW, fHalfH, 0.0f,
    };

    float uvs[] = 
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    float normals[] = 
    {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };

    unsigned int indices[] = 
    {
        0, 1, 2,
        2, 3, 0
    };

    //2 VBO创建
    glGenBuffers(1, &pPlane->m_uiPosVbo);
    glBindBuffer(GL_ARRAY_BUFFER, pPlane->m_uiPosVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glGenBuffers(1, &pPlane->m_uiUvVbo);
    glBindBuffer(GL_ARRAY_BUFFER, pPlane->m_uiUvVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

    glGenBuffers(1, &pPlane->m_uiNormalVbo);
    glBindBuffer(GL_ARRAY_BUFFER, pPlane->m_uiNormalVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

    //3 EBO创建
    glGenBuffers(1, &pPlane->m_uiEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pPlane->m_uiEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //4 VAO创建
    glGenVertexArrays(1, &pPlane->m_uiVao);
    glBindVertexArray(pPlane->m_uiVao);

    glBindBuffer(GL_ARRAY_BUFFER, pPlane->m_uiPosVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, pPlane->m_uiUvVbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, pPlane->m_uiNormalVbo);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    //5.4 加入ebo到当前的vao
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pPlane->m_uiEbo);

    glBindVertexArray(0);

    return pPlane;
}
