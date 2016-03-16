#include <windows.h>탄막
#include <time.h>
#include "resource.h"
#include <atlimage.h>
#include "MyTime.h"
#include "MyInput.h"
#include <math.h>
#include <string>

#define szWindowClass	TEXT("First")
#define szTitle			TEXT("First App")
#define WINDOW_SIZE_X 650
#define WINDOW_SIZE_Y 520

const float MAX_FPS = 60;
const float MIN_DT = 1 / MAX_FPS;


CMyInput g_myInput;
CMyTime g_myTime;
HGDIOBJ original = NULL;

CImage g_bitmap_Background[3];
float g_PosX_Background[3];
int g_yPos[3];

float g_currentMouseX;
float g_currentMouseY;

CImage g_bitmap_Airplane;
const unsigned g_sizeX_Airplane = 50;
const unsigned g_sizeY_Airplane = 15;
char g_cntAnimation_Airplane = 0;
char g_AnimationFrame_Airplane = 5;
bool g_FlagForAnimation_Airplane = true;
float g_PositionX_Airplane = 100.f;
float g_PositionY_Airplane = 100.f;
float g_Speed_Airplane = 100.f;

CImage g_bitmap_Explosion;
const unsigned g_sizeX_Explosion = 100;
const unsigned g_sizeY_Explosion = 100;
char g_cntAnimation_ExplosionI = 0;
char g_cntAnimation_ExplosionJ = 0;
int g_randrandX = 0;
int g_randrandY = 0;

enum MODEMODE{
	MODEMODE_LINELINE,
	MODEMODE_BOXBOX,
	MODEMODE_CIRCLECIRCLE,
	MODEMODE_BOXCIRCLE
};
MODEMODE g_modeMode;

WCHAR g_szPath[256];
PAINTSTRUCT	_ps;
HDC	_hdc;
HWND	_hWnd;
HINSTANCE _hInstance;
HDC memoryDC;

LRESULT CALLBACK WndProc(HWND hWnd
	, UINT message
	, WPARAM wParam
	, LPARAM lParam);
void drawBitMap(CImage cImage, int sizeX, int sizeY);
int RandBetween(int max, int min);
float 두점사이의거리(float x1, float y1, float x2, float y2);

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam,
	int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	_hInstance = hInstance;

	//마우스 지운다
	ShowCursor(false);

	if (!RegisterClassEx(&wcex))
		return 0;

	_hWnd = CreateWindowEx(WS_EX_APPWINDOW
		, szWindowClass
		, szTitle
		, WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, WINDOW_SIZE_X
		, WINDOW_SIZE_Y
		, NULL
		, NULL
		, hInstance
		, NULL);

	if (!_hWnd)
		return 0;

	ShowWindow(_hWnd, nCmdShow);

	MSG			msg;


	float dt = 0;

	//메인 게임 루프
	while (TRUE)
	{

		g_myTime.ProcessTime();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (dt < MIN_DT)
			dt += g_myTime.GetElapsedTime();
		else
		{
			g_myInput.KeyboardProcess();
			_hdc = GetDC(_hWnd);
			memoryDC = CreateCompatibleDC(_hdc);
			HBITMAP myBitmap = CreateCompatibleBitmap(_hdc, 800, 600);
			SelectObject(memoryDC, myBitmap);
			//original = SelectObject(memoryDC, GetStockObject(DC_PEN));
			for (int i = 0; i < 3; ++i)
			{
				if (g_PosX_Background[i] + 640 < 0)
					g_PosX_Background[i] = 0;
			}

			//배경 그리기
			g_bitmap_Background[0].BitBlt(memoryDC, g_PosX_Background[0], 0, SRCCOPY);
			g_bitmap_Background[0].BitBlt(memoryDC, g_PosX_Background[0] + 640, 0, SRCCOPY);
			g_bitmap_Background[1].BitBlt(memoryDC, g_PosX_Background[1], 124, SRCCOPY);
			g_bitmap_Background[1].BitBlt(memoryDC, g_PosX_Background[1] + 640, 124, SRCCOPY);
			g_bitmap_Background[2].BitBlt(memoryDC, g_PosX_Background[2], 124 + 214, SRCCOPY);
			g_bitmap_Background[2].BitBlt(memoryDC, g_PosX_Background[2] + 640, 124 + 214, SRCCOPY);
			for (int i = 0; i < 3; ++i)
				g_PosX_Background[i] -= 100 * (3 - i)* dt;

			//폭발 그리기
			g_bitmap_Explosion.TransparentBlt(memoryDC, g_randrandX, g_randrandY, g_sizeX_Explosion, g_sizeY_Explosion, g_cntAnimation_ExplosionJ * 100, g_cntAnimation_ExplosionI * 100, g_sizeX_Explosion, g_sizeY_Explosion, RGB(255, 0, 255));
			++g_cntAnimation_ExplosionJ;
			if (g_cntAnimation_ExplosionJ > 5)
			{
				++g_cntAnimation_ExplosionI;
				g_cntAnimation_ExplosionJ = 0;
			}
			if (g_cntAnimation_ExplosionI > 5)
			{
				g_cntAnimation_ExplosionI = 0;
				g_randrandX = RandBetween(WINDOW_SIZE_X, 0);
				g_randrandY = RandBetween(WINDOW_SIZE_Y - 50, 0);
			}

			//비행기 그리기
			if (g_myInput.GetKey(VK_W))
				g_PositionY_Airplane -= g_Speed_Airplane*dt;
			if (g_myInput.GetKey(VK_A))
				g_PositionX_Airplane -= g_Speed_Airplane*dt;
			if (g_myInput.GetKey(VK_S))
				g_PositionY_Airplane += g_Speed_Airplane*dt;
			if (g_myInput.GetKey(VK_D))
				g_PositionX_Airplane += g_Speed_Airplane*dt;
			if (g_FlagForAnimation_Airplane)
			{
				g_bitmap_Airplane.BitBlt(memoryDC, g_PositionX_Airplane, g_PositionY_Airplane, g_sizeX_Airplane, g_sizeY_Airplane, g_sizeX_Airplane, 0, SRCAND);
				g_bitmap_Airplane.BitBlt(memoryDC, g_PositionX_Airplane, g_PositionY_Airplane, g_sizeX_Airplane, g_sizeY_Airplane, g_sizeX_Airplane, g_sizeY_Airplane, SRCPAINT);
				if (g_cntAnimation_Airplane < g_AnimationFrame_Airplane)
					++g_cntAnimation_Airplane;
				else
				{
					g_cntAnimation_Airplane = 0;
					g_FlagForAnimation_Airplane = false;
				}
			}
			else
			{
				//						dc,			그릴x,									그릴y,								사이즈x,				사이즈y,			파일내위치
				g_bitmap_Airplane.BitBlt(memoryDC, g_PositionX_Airplane, g_PositionY_Airplane, g_sizeX_Airplane, g_sizeY_Airplane, 0, 0, SRCAND);
				g_bitmap_Airplane.BitBlt(memoryDC, g_PositionX_Airplane, g_PositionY_Airplane, g_sizeX_Airplane, g_sizeY_Airplane, 0, g_sizeY_Airplane, SRCPAINT);
				if (g_cntAnimation_Airplane < g_AnimationFrame_Airplane)
					++g_cntAnimation_Airplane;
				else
				{
					g_cntAnimation_Airplane = 0;
					g_FlagForAnimation_Airplane = true;
				}
			}

			//FillRect(memoryDC, nullptr, wcex.hbrBackground);
			//프레임 영역
			//박스충돌
			if (g_myInput.GetKey(VK_Z))
				g_modeMode = MODEMODE_LINELINE;
			else if (g_myInput.GetKey(VK_X))
				g_modeMode = MODEMODE_BOXBOX;
			else if (g_myInput.GetKey(VK_C))
				g_modeMode = MODEMODE_CIRCLECIRCLE;
			else if (g_myInput.GetKey(VK_V))
				g_modeMode = MODEMODE_BOXCIRCLE;
			switch (g_modeMode)
			{
			case MODEMODE_LINELINE:
			{
				float x1 = 100, x2 = 600, y1 = 0, y2 = WINDOW_SIZE_Y;
				float b = x2 - x1, a = y1 - y2;
				float c = (x1 - x2)*y1 + (y2 - y1)*x1;
				MoveToEx(memoryDC, x1, y1, nullptr);
				LineTo(memoryDC, x2, y2);
				if (a*g_currentMouseX + b*g_currentMouseY + c > 0)
					TextOut(memoryDC, 0, 0, L"IN", wcslen(L"IN"));
				else
					TextOut(memoryDC, 0, 0, L"OUT", wcslen(L"OUT"));
				float 거리 = fabs(a*g_currentMouseX + b*g_currentMouseY + c) / sqrt(pow(a, 2) + pow(b, 2));
				auto georiString = (std::wstring(L"거리 : ") + std::to_wstring(거리));
				auto gogigogi = georiString.c_str();
				TextOut(memoryDC, 0, 20, gogigogi, wcslen(gogigogi));

				Ellipse(memoryDC, g_currentMouseX - 5, g_currentMouseY - 5, g_currentMouseX + 5, g_currentMouseY + 5);
			}
			break;
			case MODEMODE_BOXBOX:
			{
				float bx1 = g_currentMouseX, by1 = g_currentMouseY;
				float size1W = 200, size1H = 100;
				float bx2 = 200, by2 = 200;
				float size2W = 200, size2H = 200;

				//충돌시
				if (((bx1 - bx2) >= -size1W && (bx1 - bx2) <= size2W) && ((by2 - by1) >= -size2H && (by2 - by1) <= size1H))
				{
					SelectObject(memoryDC, GetStockObject(DC_BRUSH));

					SetDCBrushColor(memoryDC, RGB(255, 0, 0));
				}

				Rectangle(memoryDC, bx2, by2, bx2 + size2W, by2 + size2H);
				Rectangle(memoryDC, bx1, by1, bx1 + size1W, by1 + size1H);
			}
			break;
			case MODEMODE_CIRCLECIRCLE:
			{
				float x2 = 200, y2 = 200;
				float r1 = 80, r2 = 100;
				float x1 = g_currentMouseX, y1 = g_currentMouseY;

				if (두점사이의거리(x1, y1, x2, y2) <= (r1 + r2))
				{
					SelectObject(memoryDC, GetStockObject(DC_BRUSH));
					SetDCBrushColor(memoryDC, RGB(0, 255, 0));
				}
				float startX1 = x1 - r1, startY1 = y1 - r1;
				float startX2 = x2 - r2, startY2 = y2 - r2;
				Ellipse(memoryDC, startX2, startY2, startX2 + 2 * r2, startY2 + 2 * r2);
				Ellipse(memoryDC, startX1, startY1, startX1 + 2 * r1, startY1 + 2 * r1);
			}
			break;
			case MODEMODE_BOXCIRCLE:
			{
				float bx = 200, by = 200;
				float width = 200, height = 100;
				float cx = g_currentMouseX, cy = g_currentMouseY;
				float r = 50;
				float startCx = cx - r, startCy = cy - r;
				float daegakseon = sqrt(pow(width, 2) + pow(height, 2));
				float xx1 = bx, yy1 = by, xx2 = bx + width, yy2 = by, xx3 = bx, yy3 = by + height, xx4 = bx + width, yy4 = by + height;
				if ((bx < startCx+r*2 && bx + width > startCx) && (by < startCy + 2 * r && by + height > startCy))
				{
					if ((cx < bx || cx>bx + width) && (cy<by || cy>by + height))
						if (!((두점사이의거리(cx, cy, xx1, yy1) < r) || (두점사이의거리(cx, cy, xx2, yy2) < r) || (두점사이의거리(cx, cy, xx3, yy3) < r) || (두점사이의거리(cx, cy, xx4, yy4) < r)))
							goto GOGI;
					SelectObject(memoryDC, GetStockObject(DC_BRUSH));
					SetDCBrushColor(memoryDC, RGB(0, 0, 255));
				}
			GOGI:
				Rectangle(memoryDC, bx, by, bx + width, by + height);
				Ellipse(memoryDC, startCx, startCy, startCx + 2 * r, startCy + 2 * r);

			}
			break;
			}//end of switch






			BitBlt(_hdc, 0, 0, 800, 600, memoryDC, 0, 0, SRCCOPY);
			DeleteDC(memoryDC);
			ReleaseDC(_hWnd, _hdc);
			DeleteObject(myBitmap);
			dt = 0;
		}

	}

	return (int)msg.wParam;
}

// 메시지 처리 함수
LRESULT CALLBACK WndProc(HWND hWnd
	, UINT message
	, WPARAM wParam
	, LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
	{


		GetCurrentDirectoryW(sizeof(g_szPath), g_szPath);
		WCHAR temp[256];
		wcscpy_s(temp, g_szPath); wcscat_s(temp, L"/Image/sky1.bmp");
		g_bitmap_Background[0].Load(temp);
		wcscpy_s(temp, g_szPath); wcscat_s(temp, L"/Image/sky2.bmp");
		g_bitmap_Background[1].Load(temp);
		wcscpy_s(temp, g_szPath); wcscat_s(temp, L"/Image/sky3.bmp");
		g_bitmap_Background[2].Load(temp);
		wcscpy_s(temp, g_szPath); wcscat_s(temp, L"/Image/airplane.bmp");
		g_bitmap_Airplane.Load(temp);
		wcscpy_s(temp, g_szPath); wcscat_s(temp, L"/Image/explosion.bmp");
		g_bitmap_Explosion.Load(temp);


		g_PosX_Background[0] = 0;
		g_PosX_Background[1] = 0;
		g_PosX_Background[2] = 0;


		g_myTime.Init();

	}
	break;
	case WM_MOUSEMOVE:
	{
		g_currentMouseX = LOWORD(lParam);
		g_currentMouseY = HIWORD(lParam);
	}
	break;
	case WM_PAINT:
	{

	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		g_bitmap_Airplane.Destroy();
		for (auto i : g_bitmap_Background)
			i.Destroy();
		g_bitmap_Explosion.Destroy();
		SelectObject(_hdc, original);
		DeleteDC(memoryDC);
	}
	return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
int RandBetween(int max, int min)
{
	return min + (rand() % (max - min + 1));
}

float 두점사이의거리(float x1, float y1, float x2, float y2)
{
	return sqrt((pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}