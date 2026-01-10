#pragma once
#include <iostream>

#define appInstance Application::getInstance()

/*
    单例类
	成员函数：
		getInstance()：获取单例对象
		getWidth()：获取窗口宽度
		getHeight()：获取窗口高度

	响应回调函数
	函数指针 ResizeCallback：窗口大小变化回调函数
	响应键盘消息回调函数 keyCallback 可自行添加


*/

class GLFWwindow;

using ResizeCallback = void(*)(int iWidth, int iHeight);
using KeyCallback = void(*)(int iKey, int iAction, int iMods);
using MouseCallback = void(*)(int iButton, int iAction, int iMods);
using CursorCallback = void(*)(double dxPos, double dyPos);
using ScrollCallback = void(*)(double dYOffset);



class Application
{
public:
	~Application();

	static Application* getInstance();
	uint32_t getWidth() const { return m_uiWidth; }
	uint32_t getHeight() const { return m_uiHeight; }
	void getCursorPoition(double *dXPos, double *dYPos);


	bool Init(const int& iWidth = 800, const int& iHeight = 600);
	bool Update();
	void Destroy();

	void setResizeCallback(ResizeCallback callback) { m_resizeCallbackFunc = callback; }
	void setKeyCallback(KeyCallback callback) { m_keyCallbackFunc = callback; }
	void setMouseCallback(MouseCallback callback) { m_mouseCallbackFunc = callback; }
	void setCursorCallback(CursorCallback callback) { m_cursorCallbackFunc = callback; }
	void setScrollCallback(ScrollCallback callback) { m_scrollCallbackFunc = callback; }

	GLFWwindow* getWindow()const { return m_pGLFWwindow; }

private:
	static void framebufferSizeCallback(GLFWwindow* pGLFWWindow, int iWidth, int iHeight);
	static void keyCallback(GLFWwindow* pGLFWWindow, int iKey, int iScancode, int iAction, int iMods);
	static void mouseCallback(GLFWwindow* pGLFWWindow, int iButton, int iAction, int iMods);
	static void cursorCallback(GLFWwindow* pGLFWWindow, double dxPos, double dyPos);
	static void scrollCallback(GLFWwindow* pGLFWWindow, double dXOffset, double dYOffset);

private:
	static Application* m_pAppInstance;

	uint32_t m_uiWidth { 0 };
	uint32_t m_uiHeight { 0 };
	GLFWwindow* m_pGLFWwindow { nullptr };

	ResizeCallback m_resizeCallbackFunc { nullptr };
	KeyCallback m_keyCallbackFunc { nullptr };
	MouseCallback m_mouseCallbackFunc{ nullptr };
	CursorCallback m_cursorCallbackFunc{ nullptr };
	ScrollCallback m_scrollCallbackFunc{ nullptr };

	Application();
};
