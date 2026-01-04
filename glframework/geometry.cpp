#include "geometry.h"
#include <vector>

Geometry::Geometry()
{
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

    // 顶点位置数据
    float positions[] = {
        // 前面 (z = -fHalfSize)
        -fHalfSize, -fHalfSize, -fHalfSize,  // 0: 左下
         fHalfSize, -fHalfSize, -fHalfSize,  // 1: 右下
         fHalfSize,  fHalfSize, -fHalfSize,  // 2: 右上
        -fHalfSize,  fHalfSize, -fHalfSize,  // 3: 左上

        // 后面 (z = fHalfSize)
         fHalfSize, -fHalfSize,  fHalfSize,  // 4: 左下
        -fHalfSize, -fHalfSize,  fHalfSize,  // 5: 右下
        -fHalfSize,  fHalfSize,  fHalfSize,  // 6: 右上
         fHalfSize,  fHalfSize,  fHalfSize,  // 7: 左上

         // 左面 (x = -fHalfSize)
         -fHalfSize, -fHalfSize,  fHalfSize,  // 8: 左下
         -fHalfSize, -fHalfSize, -fHalfSize,  // 9: 右下
         -fHalfSize,  fHalfSize, -fHalfSize,  // 10: 右上
         -fHalfSize,  fHalfSize,  fHalfSize,  // 11: 左上

         // 右面 (x = fHalfSize)
          fHalfSize, -fHalfSize, -fHalfSize,  // 12: 左下
          fHalfSize, -fHalfSize,  fHalfSize,  // 13: 右下
          fHalfSize,  fHalfSize,  fHalfSize,  // 14: 右上
          fHalfSize,  fHalfSize, -fHalfSize,  // 15: 左上

          // 上面 (y = fHalfSize)
          -fHalfSize,  fHalfSize, -fHalfSize,  // 16: 左下
           fHalfSize,  fHalfSize, -fHalfSize,  // 17: 右下
           fHalfSize,  fHalfSize,  fHalfSize,  // 18: 右上
          -fHalfSize,  fHalfSize,  fHalfSize,  // 19: 左上

          // 下面 (y = -fHalfSize)
          -fHalfSize, -fHalfSize,  fHalfSize,  // 20: 左下
           fHalfSize, -fHalfSize,  fHalfSize,  // 21: 右下
           fHalfSize, -fHalfSize, -fHalfSize,  // 22: 右上
          -fHalfSize, -fHalfSize, -fHalfSize   // 23: 左上
    };

    //  UV 坐标
    float uv[] = {
        // 前面
        1.0f, 0.0f,  // 0: 左下 (原0,0 -> 现在1,0)
        0.0f, 0.0f,  // 1: 右下 (原1,0 -> 现在0,0)
        0.0f, 1.0f,  // 2: 右上 (原1,1 -> 现在0,1)
        1.0f, 1.0f,  // 3: 左上 (原0,1 -> 现在1,1)

        // 后面
        1.0f, 0.0f,  // 4: 左下
        0.0f, 0.0f,  // 5: 右下
        0.0f, 1.0f,  // 6: 右上
        1.0f, 1.0f,  // 7: 左上

        // 左面
        1.0f, 0.0f,  // 8: 左下
        0.0f, 0.0f,  // 9: 右下
        0.0f, 1.0f,  // 10: 右上
        1.0f, 1.0f,  // 11: 左上

        // 右面
        1.0f, 0.0f,  // 12: 左下
        0.0f, 0.0f,  // 13: 右下
        0.0f, 1.0f,  // 14: 右上
        1.0f, 1.0f,  // 15: 左上

        // 上面
        1.0f, 0.0f,  // 16: 左下
        0.0f, 0.0f,  // 17: 右下
        0.0f, 1.0f,  // 18: 右上
        1.0f, 1.0f,  // 19: 左上

        // 下面
        1.0f, 0.0f,  // 20: 左下
        0.0f, 0.0f,  // 21: 右下
        0.0f, 1.0f,  // 22: 右上
        1.0f, 1.0f   // 23: 左上
    };

    //法线数据
    float normals[] = {
        // 前面 (4个顶点，法线都是 (0, 0, -1))
        0.0f, 0.0f, -1.0f,  // 0
        0.0f, 0.0f, -1.0f,  // 1
        0.0f, 0.0f, -1.0f,  // 2
        0.0f, 0.0f, -1.0f,  // 3

        // 后面 (4个顶点，法线都是 (0, 0, 1))
        0.0f, 0.0f, 1.0f,   // 4
        0.0f, 0.0f, 1.0f,   // 5
        0.0f, 0.0f, 1.0f,   // 6
        0.0f, 0.0f, 1.0f,   // 7

        // 左面 (4个顶点，法线都是 (-1, 0, 0))
        -1.0f, 0.0f, 0.0f,  // 8
        -1.0f, 0.0f, 0.0f,  // 9
        -1.0f, 0.0f, 0.0f,  // 10
        -1.0f, 0.0f, 0.0f,  // 11

        // 右面 (4个顶点，法线都是 (1, 0, 0))
        1.0f, 0.0f, 0.0f,   // 12
        1.0f, 0.0f, 0.0f,   // 13
        1.0f, 0.0f, 0.0f,   // 14
        1.0f, 0.0f, 0.0f,   // 15

        // 上面 (4个顶点，法线都是 (0, 1, 0))
        0.0f, 1.0f, 0.0f,   // 16
        0.0f, 1.0f, 0.0f,   // 17
        0.0f, 1.0f, 0.0f,   // 18
        0.0f, 1.0f, 0.0f,   // 19

        // 下面 (4个顶点，法线都是 (0, -1, 0))
        0.0f, -1.0f, 0.0f,  // 20
        0.0f, -1.0f, 0.0f,  // 21
        0.0f, -1.0f, 0.0f,  // 22
        0.0f, -1.0f, 0.0f   // 23
    };

    //// 顶点索引数据
    //unsigned int indices[] = {
    //    // 前面
    //    0, 1, 2,
    //    0, 2, 3,

    //    // 后面
    //    4, 5, 6,
    //    4, 6, 7,

    //    // 左面
    //    8, 9, 10,
    //    8, 10, 11,

    //    // 右面
    //    12, 13, 14,
    //    12, 14, 15,

    //    // 上面
    //    16, 17, 18,
    //    16, 18, 19,

    //    // 下面
    //    20, 21, 22,
    //    20, 22, 23
    //};
    unsigned int indices[] = {
        // 前面
        0, 3, 2,
        0, 2, 1,

        // 后面
        4, 7, 6,
        4, 6, 5,

        // 左面
        8, 11, 10,
        8, 10, 9,

        // 右面
        12, 15, 14,
        12, 14, 13,

        // 上面
        16, 19, 18,
        16, 18, 17,

        // 下面
        20, 23, 22,
        20, 22, 21
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
    std::vector<GLuint> vctIndices;

    //生成位置，uv
    for (int i = 0; i < iLatLinesNum; i++)
    {
        for (int j = 0; j <= iLongLinesNum; j++)
        {
            float fPhi = i * glm::pi<float>() / iLatLinesNum;
            float fTheta = j * 2 * glm::pi<float>() / iLongLinesNum;

            float fYPos = fRadius * cos(fPhi);
            float fXPos = fRadius * sin(fPhi) * cos(fTheta);
            float fZPos = fRadius * sin(fPhi) * sin(fTheta);

            vctPosition.push_back(fYPos);
            vctPosition.push_back(fXPos);
            vctPosition.push_back(fZPos);

            float fU = 1.0f - (float)j / (float)iLongLinesNum;
            float fV = (float)i / (float)iLatLinesNum;

            vctUv.push_back(fU);
            vctUv.push_back(fV);
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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pSphere->m_uiEbo);

    glBindVertexArray(0);

    pSphere->m_uiIndicesCount = vctIndices.size();

	return pSphere;
}
