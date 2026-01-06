#pragma once
#include "../../glframework//core.h"
#include "camera.h"
#include <unordered_map>
#include <memory>

class CameraControl
{
public:

	CameraControl();
	~CameraControl();

	virtual void onMouseEvent(const int& iButton, const int& iAction, const double& dXPos, const double& dYPos);
	virtual void onCursorEvent(const double& dXPos, const double& dYPos);
	virtual void onKeyboardEvent(const int& iKey, const int& iAction, const int& iMods);
	virtual void onScrollEvent(float fOffset);

	virtual void update();
	virtual void resetCamera();

	void setCamera(std::shared_ptr<Camera> pCamera) { m_pCamera = pCamera; }
	void setSensitivity(float fSensitivity) { m_fSensitivity = fSensitivity; }
	void setScaleSpeed(float fScaleSpeed) { m_fScaleSpeed = fScaleSpeed; }
	Camera* getMyCamera() { return m_pCamera.get(); }
protected:
	//1. 鼠标按键状态
	bool m_bLeftMouseDown = false;
	bool m_bRightMouseDown = false;
	bool m_bMiddleMouseDown = false;

	//2 当前鼠标的位置
	double m_dCurrentXpos = 0.0;
	double m_dCurrentYpos = 0.0;

	//3. 镜头敏感度
	float m_fSensitivity = 0.7f;

	//4. 按键状态
	std::unordered_map<int, bool> m_hashMapKey;

	//5 当前控制的摄像机
	std::shared_ptr<Camera> m_pCamera;

	//6.相机缩放速度
	float m_fScaleSpeed = 0.2f;
};