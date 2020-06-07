#include <DirectXMath.h>
#include <iostream>
using namespace DirectX;
using namespace	std;

void DisplayVector(XMVECTOR showVec)
{
	XMFLOAT3 c;
	XMStoreFloat3(&c, showVec);
	cout << "( " << c.x << ", " << c.y << ", " << c.z << ") " << endl;
}
void PrintMatrixRow(XMVECTOR showVec)
{
	XMFLOAT4 f;
	XMStoreFloat4(&f, showVec);
	cout << "( " << f.x << ", " << f.y << ", " << f.z << ", " << f.w << ") " << endl;
}
void DisplayMatrix(XMMATRIX* m)
{
	for (int i = 0; i < 4; i++)
	{
		PrintMatrixRow(m->r[i]);
	}
}
//int main()
//{
//
//	XMMATRIX mat1 = XMMatrixIdentity();
//	XMVECTOR vec1 = XMVectorSet(10.0f, 2.0f, 5.0f, 10.0f);
//	vec1 = XMVector3Normalize(vec1);
//	DisplayVector(vec1);
//	system("pause");
//	return 0;
//}