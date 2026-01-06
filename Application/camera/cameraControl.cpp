#include "cameraControl.h"
#include <iostream>

CameraControl::CameraControl()
{
}

CameraControl::~CameraControl()
{
}

void CameraControl::onMouseEvent(const int& iButton, const int& iAction, const double& dXPos, const double& dYPos)
{
	//判断当前按键是否按下
	bool bPressed = GLFW_PRESS == iAction ? true : false;

	//如果按下，记录当前鼠标按下的位置
	if (bPressed)
	{
		m_dCurrentXpos = dXPos;
		m_dCurrentYpos = dYPos;
	}

	//更改按键状态
	switch (iButton)
	{
	    case GLFW_MOUSE_BUTTON_LEFT:
	    {
	    	m_bLeftMouseDown = bPressed;
			break;
	    }
	    case GLFW_MOUSE_BUTTON_RIGHT:
	    {
	    	m_bRightMouseDown = bPressed;
			break;
	    }
	    case GLFW_MOUSE_BUTTON_MIDDLE:
	    {
	    	m_bMiddleMouseDown = bPressed;
			break;
	    }
	}
}

void CameraControl::onCursorEvent(const double& dXPos, const double& dYPos)
{
}

void CameraControl::onKeyboardEvent(const int& iKey, const int& iAction, const int& iMods)
{
	//过滤repeat事件
	if (GLFW_REPEAT == iAction)
	{
		return;
	}

	if (GLFW_KEY_R == iKey && GLFW_PRESS == iAction && GLFW_MOD_CONTROL == iMods)
	{
		resetCamera();
	}

	//判断当前按键是否按下
	bool bPressed = GLFW_PRESS == iAction ? true : false;
	
	//记录在hashmap中
	m_hashMapKey[iKey] = bPressed;
}

void CameraControl::onScrollEvent(float fOffset)
{
}

void CameraControl::update()
{
}

void CameraControl::resetCamera()
{
	m_pCamera->m_vec3Position = glm::vec3(0.0f, 0.0f, 10.0f);
	m_pCamera->m_vec3Up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_pCamera->m_vec3Right = glm::vec3(1.0f, 0.0f, 0.0f);
}
