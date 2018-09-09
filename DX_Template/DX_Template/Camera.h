#ifndef  _CAMERA_H_
#define _CAMERA_H_
#include <DirectXMath.h>
using namespace DirectX;
class Camera
{
public:
	Camera(int Width, int Height);
	~Camera();

private: 
	XMMATRIX camView;
	XMMATRIX WVP;

	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp;

	XMVECTOR DefaultForward;
	XMVECTOR DefaultRight;
	XMVECTOR camForward;
	XMVECTOR camRight;
	XMMATRIX camRotationMatrix;
	
	float posX, posY, posZ;
	float rotX, rotY, rotZ;
	int		width, height;
public:
	void CameraInit();
	void UpdateCamera();
	void ReleaseAll();
	

	// Variable for camera movement.
	float	 moveLeftRight = 0.0f;
	float	 moveBackForward = 0.0f;
	float	 camYaw = 0.0f;
	float	 camPitch = 0.0f;

	XMMATRIX getCamViewMatrix() const;
	XMVECTOR getCamPosition() const;
	XMVECTOR getCamTarget() const;

};
#endif // ! _CAMERA_H_

