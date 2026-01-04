#include "shader.h"
#include "shader.h"
#include "../wrapper/checkError.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

void Shader::beginShader()
{
	GL_CALL(glUseProgram(m_programID));
}

void Shader::endShader()
{
	GL_CALL(glUseProgram(0));
}

void Shader::setFloat(const std::string& strName, float fValue)
{
	//通过名称拿到Uniform变量的位置
	GLuint uiPos = GL_CALL(glGetUniformLocation(m_programID, strName.c_str()));

	//通过loaction更新uniform变量的值
	GL_CALL(glUniform1f(uiPos, fValue));
}

void Shader::setBool(const std::string& strName, bool bValue)
{
	//通过名称拿到Uniform变量的位置
	GLuint uiPos = GL_CALL(glGetUniformLocation(m_programID, strName.c_str()));

	//通过loaction更新uniform变量的值
	GL_CALL(glUniform1i(uiPos, (int)bValue));
}

void Shader::setInt(const std::string& strName, int iValue)
{
	//通过名称拿到Uniform变量的位置
	GLuint uiPos = GL_CALL(glGetUniformLocation(m_programID, strName.c_str()));

	//通过loaction更新uniform变量的值
	GL_CALL(glUniform1i(uiPos, iValue));
}

void Shader::setVec3(const std::string& strName, glm::vec3 vec3Value)
{//通过名称拿到Uniform变量的位置
	GLuint uiPos = GL_CALL(glGetUniformLocation(m_programID, strName.c_str()));

	//通过loaction更新uniform变量的值
	GL_CALL(glUniform3fv(uiPos, 1, glm::value_ptr(vec3Value)));
}

void Shader::setMatrix4x4(const std::string& strName, glm::mat4 value)
{
	//通过名称拿到Uniform变量的位置
	GLuint uiPos = GL_CALL(glGetUniformLocation(m_programID, strName.c_str()));

	//通过loaction更新uniform变量的值
	GL_CALL(glUniformMatrix4fv(uiPos, 1, GL_FALSE, glm::value_ptr(value)));
}

void Shader::checkCompileErrors(GLuint shader, std::string type)
{
	int success = 0;
	char infoLog[1024] = { 0 };
	if (type == "COMPILE")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILE_ERROR\n" << infoLog << std::endl;
		}
	}
	else if (type == "LINK")
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}
	else
	{
		std::cout << "ERROR::SHADER::UNKNOWN_TYPE " << type << std::endl;
	}
}


Shader::Shader()
{
	
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	//声明装入着色器源码的字符串
	std::string vertexShaderSource;
	std::string fragmentShaderSource;
	//声明读取文件流对象
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	//确保ifstream对象可以抛出异常
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		//打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		//读取文件内容到字符串流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//关闭文件
		vShaderFile.close();
		fShaderFile.close();
		//转换字符串流到字符串
		vertexShaderSource = vShaderStream.str();
		fragmentShaderSource = fShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::" << e.what() << std::endl;
	}

	//创建Shader对象
	GLuint vertexShader;
	GLuint fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vShaderCode = vertexShaderSource.c_str();
	const char* fShaderCode = fragmentShaderSource.c_str();
	//绑定着色器源码
	GL_CALL(glShaderSource(vertexShader, 1, &vShaderCode, NULL));
	GL_CALL(glShaderSource(fragmentShader, 1, &fShaderCode, NULL));


	//编译着色器
	GL_CALL(glCompileShader(vertexShader));
	checkCompileErrors(vertexShader, "COMPILE");

	GL_CALL(glCompileShader(fragmentShader));
	checkCompileErrors(fragmentShader, "COMPILE");
	
	//创建程序对象
	m_programID = glCreateProgram();

	//附加着色器到程序对象
	GL_CALL(glAttachShader(m_programID, vertexShader));
	GL_CALL(glAttachShader(m_programID, fragmentShader));

	//链接程序
	GL_CALL(glLinkProgram(m_programID));
	//检查链接错误
	checkCompileErrors(m_programID, "LINK");
	
	//删除着色器对象
	GL_CALL(glDeleteShader(vertexShader));
	GL_CALL(glDeleteShader(fragmentShader));
}

Shader::~Shader()
{
}