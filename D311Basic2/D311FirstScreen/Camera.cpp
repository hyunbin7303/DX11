#include "Camera.h"



Camera::Camera(int Width, int Height)
{
	width = Width;
	height = Height;
	posX = posY = posZ = 0.0f;
	rotX = rotY = rotZ = 0.0f;
}


Camera::~Camera()
{
}

void Camera::CameraInit()
{
	camPosition = XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);
	camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//Set the View matrix
	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
	DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
}

void Camera::UpdateCamera()
{
	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(camYaw);

	camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
	camUp = XMVector3TransformCoord(camUp, RotateYTempMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);

	camPosition += moveLeftRight * camRight;
	camPosition += moveBackForward * camForward;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camTarget = camPosition + camTarget;
	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
}
void Camera::ReleaseAll()
{
}
XMMATRIX Camera::getCamViewMatrix() const
{
	return camView;
}
XMVECTOR Camera::getCamPosition() const
{
	return camPosition;
}
XMVECTOR Camera::getCamTarget() const
{
	return camTarget;
}
