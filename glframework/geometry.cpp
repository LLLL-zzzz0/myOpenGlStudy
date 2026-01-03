#include "geometry.h"

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

    // 正确的 UV 坐标（使用 stbi_set_flip_vertically_on_load(true) 后）
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

    // 顶点索引数据
    unsigned int indices[] = {
        // 前面
        0, 1, 2,
        0, 2, 3,

        // 后面
        4, 5, 6,
        4, 6, 7,

        // 左面
        8, 9, 10,
        8, 10, 11,

        // 右面
        12, 13, 14,
        12, 14, 15,

        // 上面
        16, 17, 18,
        16, 18, 19,

        // 下面
        20, 21, 22,
        20, 22, 23
    };

    glGenBuffers(1, &pBox->m_uiPosVbo);
    glBindBuffer(GL_ARRAY_BUFFER, pBox->m_uiPosVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glGenBuffers(1, &pBox->m_uiUvVbo);
    glBindBuffer(GL_ARRAY_BUFFER, pBox->m_uiUvVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pBox->m_uiEbo);

	return pBox;
}

Geometry* Geometry::createSphere(float fSize)
{
	return nullptr;
}
