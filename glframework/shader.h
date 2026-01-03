#include "core.h"
#include <string>

class Shader
{
public:
	Shader();
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void beginShader();//开始使用当前的Shader程序

	void endShader();//结束使用当前的Shader程序

	void setFloat(const std::string& strName, float fValue);

	void setInt(const std::string& strName, int iValue);

	void setMatrix4x4(const std::string& strName, glm::mat4 value);

	GLuint getMyProgramId() { return m_programID; }
private:
	void checkCompileErrors(GLuint shader, std::string type);//检查编译错误
private:
	GLuint m_programID{0};//着色器程序ID

};


