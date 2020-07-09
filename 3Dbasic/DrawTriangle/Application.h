
#ifndef _APPLICATION_H_
#define _APPLICATION_H_
#include "Windows.h"
#include "DX.h"


class Application: public Windows
{

public:
	Application() = default;

	virtual void OnCreate() ;
	virtual void OnUpdate() ;
	virtual void OnDestroy();

};


#endif

