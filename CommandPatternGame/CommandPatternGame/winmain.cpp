#include "DXApp.h"
#include "TestGame.h"
int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	//MessageBox(NULL, "Hi, Kevin.", "Check", NULL);
	TestGame tApp(hInstance);
	if (!tApp.Init())
	{
		MessageBox(NULL, "Initialization Error I guess?", "Check", NULL);
		return 1;
	}
	return tApp.Run();
}
