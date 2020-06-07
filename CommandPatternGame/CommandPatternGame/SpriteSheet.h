#pragma once

#include <wincodec.h>
#include "Graphics.h"
class SpriteSheet
{
	SpriteSheet(wchar_t*  filename, Graphics* gfx);// Used for background.



public:
	SpriteSheet();
	~SpriteSheet();
};

