#include "Camera.h"
Camera::Camera()
{
	px = py = pz = 0.0f;
	rx = ry = rz = 0.0f;
}

Camera::Camera(const Camera & camera)
{
}
Camera::~Camera()
{
}

XMFLOAT3 Camera::GetPosition()
{
	return XMFLOAT3(px,py,pz);
}

XMFLOAT3 Camera::GetRotation()
{
	return XMFLOAT3(rx,ry,rz);
}
void Camera::SetPosition(float x, float y, float z)
{
	px = x;
	py = y;
	pz = z;
}
void Camera::SetRotation(float x, float y, float z)
{
	rx = x;
	ry = y;
	rz = z;
}
XMMATRIX Camera::RenderCamera()
{
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -10.0f, 0.0f);
	XMMATRIX rotateMatrix;
	m_viewMatrix = XMMatrixLookAtLH(Eye, At, Up);
	return m_viewMatrix;
}
