#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;
class Camera
{

private:
	XMMATRIX m_viewMatrix;
	float px, py, pz;
	float rx, ry, rz;
public:
	Camera();
	Camera(const Camera& camera);
	~Camera();
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	XMMATRIX RenderCamera();
};

#endif