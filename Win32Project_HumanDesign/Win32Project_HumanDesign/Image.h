#pragma once
#include <windows.h>
#include <atlimage.h>
#include "resource.h"

class Image
{
public:
	Image();
	Image(HDC targetDC);
	virtual ~Image();
private:
	CImage _img;
};