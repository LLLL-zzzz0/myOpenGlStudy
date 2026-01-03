#include <glad/glad.h>
#include "checkError.h"
#include <assert.h>
#include <iostream>
#include <bitset>

void checkError()
{
	GLenum errNum;
	if((errNum = glGetError()) != GL_NO_ERROR)
	{
		std::cerr << "OpenGL Error: 0x" << std::hex << errNum << std::endl;
		assert(false);
	}
}