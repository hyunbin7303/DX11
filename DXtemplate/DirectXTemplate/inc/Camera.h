#ifndef _CAMERA_H_
#define _CAMERA_H_


#include <DirectXMath.h>


using namespace DirectX;
class Camera
{
private:

	XMMATRIX camView;
	XMMATRIX camProjection;
	XMMATRIX WVP;

	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp;

	XMMATRIX RotationX;
	XMMATRIX RotationY;
	XMMATRIX RotationZ;

	
	XMVECTOR DefaultForward;
	XMVECTOR DefaultRight;
	XMVECTOR camForward;
	XMVECTOR camRight;


	XMMATRIX camRotationMatrix;
	XMMATRIX groundWorld;



	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;
	float camYaw = 0.0f;
	float camPitch = 0.0f;

	int width, height;
public:
	Camera(int Width, int Height);
	~Camera();
	void CameraInit();
	void UpdateCamera();


	XMVECTOR getCamPosition() const;
	XMVECTOR getCamTarget() const;

	XMMATRIX getViewMatrix(XMMATRIX &viewMat);
};

#endif