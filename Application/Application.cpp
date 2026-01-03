#include "Application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Application* Application::m_pAppInstance = nullptr;

Application* Application::getInstance()
{
	if (Application::m_pAppInstance == nullptr)
	{
		Application::m_pAppInstance = new Application();
	}
	return Application::m_pAppInstance;
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::getCursorPoition(double* dXPos, double* dYPos)
{
	glfwGetCursorPos(m_pGLFWwindow, dXPos, dYPos);
}

bool Application::Init(const int& iWidth, const int& iHeight)
{
	this->m_uiWidth = iWidth;
	this->m_uiHeight = iHeight;
	// 初始化 GLFW 的基本环境
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建窗口对象
	m_pGLFWwindow = glfwCreateWindow(this->m_uiWidth, this->m_uiHeight, "OpenGL Window", NULL, NULL);
	if (m_pGLFWwindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	//设置当前窗口为openGL的绘制舞台
	glfwMakeContextCurrent(m_pGLFWwindow);

	//使用glad加载OpenGL函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	//this 指针设置到 window user pointer 中，供回调函数使用
	glfwSetWindowUserPointer(m_pGLFWwindow, this);

	glfwSetFramebufferSizeCallback(m_pGLFWwindow, framebufferSizeCallback);

	glfwSetKeyCallback(m_pGLFWwindow, keyCallback);

	glfwSetMouseButtonCallback(m_pGLFWwindow, mouseCallback);

	glfwSetCursorPosCallback(m_pGLFWwindow, cursorCallback);

	glfwSetScrollCallback(m_pGLFWwindow, scrollCallback);

	return true;
}
bool Application::Update()
{
	//检查窗口是否被要求关闭
	if(glfwWindowShouldClose(m_pGLFWwindow))
	{
		return false;
	}

	glfwPollEvents(); //检查并调用事件函数

	//双缓存交换
	glfwSwapBuffers(m_pGLFWwindow);

	return true;
}
void Application::Destroy()
{
	//销毁窗口
	glfwTerminate();
}

void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->m_resizeCallbackFunc != nullptr)
	{
		self->m_resizeCallbackFunc(width, height);
	}
}

void Application::keyCallback(GLFWwindow* pGLFWWindow, int iKey, int iScancode, int iAction, int iMods)
{
	Application* self = (Application*)glfwGetWindowUserPointer(pGLFWWindow);
	if (self->m_keyCallbackFunc != nullptr)
	{
		self->m_keyCallbackFunc(iKey, iAction, iMods);
	}
}

void Application::mouseCallback(GLFWwindow* pGLFWWindow, int iButton, int iAction, int iMods)
{
	Application* self = (Application*)glfwGetWindowUserPointer(pGLFWWindow);
	if (self->m_mouseCallbackFunc != nullptr)
	{
		self->m_mouseCallbackFunc(iButton, iAction, iMods);
	}
}

void Application::cursorCallback(GLFWwindow* pGLFWWindow, double dxPos, double dyPos)
{
	Application* self = (Application*)glfwGetWindowUserPointer(pGLFWWindow);
	if (self->m_cursorCallbackFunc != nullptr)
	{
		self->m_cursorCallbackFunc(dxPos, dyPos);
	}
}

//滚动消息的xOFFset无用
void Application::scrollCallback(GLFWwindow* pGLFWWindow, double dXOffset, double dYOffset)
{
	Application* self = (Application*)glfwGetWindowUserPointer(pGLFWWindow);
	if (self->m_scrollCallbackFunc != nullptr)
	{
		self->m_scrollCallbackFunc(dYOffset);
	}
}

