#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../Application/stb_image.h"

Texture::Texture(const std::string& strPath, unsigned int uiUnit)
{
	m_uiUnit = uiUnit;
	//用stdImage读取图片
	int iChannels;

	//反转y轴
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(strPath.c_str(), &m_iWidth, &m_iHeight, &iChannels, STBI_rgb_alpha);

	//生成纹理并且激活单元绑定
	GL_CALL(glGenTextures(1, &m_texture));
	//激活纹理单元
	GL_CALL(glActiveTexture(GL_TEXTURE0 + m_uiUnit));
	//绑定纹理对象
	GL_CALL(glBindTexture(GL_TEXTURE_2D, m_texture));

	/*int iTempWidth = m_iWidth;
	int iTempHeight = m_iHeight;
	for (int level = 0; true; ++level)
	{
		GL_CALL(glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, iTempWidth, iTempHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
		if (iTempWidth == 1 && iTempHeight == 1)
		{
			break;
		}

		iTempWidth = iTempWidth > 1 ? iTempWidth / 2 : 1;
		iTempHeight = iTempHeight > 1 ? iTempHeight / 2 : 1;
	}*/

	//传输纹理数据,开辟显存
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

	//生成mipmap
	GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

	//***释放数据
	stbi_image_free(data);

	//设置纹理过滤方式
	//当需要的像素  > 图片的像素 linear
	//当需要的像素  < 图片的像素 nearest
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	//GL_NEAREST单个mipmap上采用最临近采样
	// 
	// GL_NEAREST_MIPMAP_LINEAR
	// 
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR));

	//设置纹理包裹方式
	//-repeat mirror 
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)); //u
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)); //v
}

Texture::~Texture()
{
	if (m_texture != 0)
	{
		GL_CALL(glDeleteTextures(1, &m_texture));
		m_texture = 0;
	}
}

void Texture::bind()
{
	//激活纹理单元
	GL_CALL(glActiveTexture(GL_TEXTURE0 + m_uiUnit));
	//绑定纹理对象
	GL_CALL(glBindTexture(GL_TEXTURE_2D, m_texture));
}
