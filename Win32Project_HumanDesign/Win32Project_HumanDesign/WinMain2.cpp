#ifdef GOGIGOGIGOGI
#include <windows.h>탄막
#include <time.h>
#include "resource.h"
#include <atlimage.h>
#include "MyTime.h"
#include "MyInput.h"

#define szWindowClass	TEXT("First")
#define szTitle			TEXT("First App")
#define WINDOW_SIZE_X 650
#define WINDOW_SIZE_Y 520

const float MAX_FPS = 60;
const float MIN_DT = 1 / MAX_FPS;


CMyInput g_myInput;
CMyTime g_myTime;

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

			for (int i = 0; i < 3; ++i)
			{
				if (g_PosX_Background[i] +640 < 0)
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
			g_bitmap_Explosion.TransparentBlt(memoryDC,g_randrandX,g_randrandY, g_sizeX_Explosion, g_sizeY_Explosion, g_cntAnimation_ExplosionJ*100, g_cntAnimation_ExplosionI * 100, g_sizeX_Explosion, g_sizeY_Explosion, RGB(255, 0, 255));
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
				g_randrandY = RandBetween(WINDOW_SIZE_Y-50, 0);
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
				if (g_cntAnimation_Airplane<g_AnimationFrame_Airplane)
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


			//프레임 영역


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

#endif // GOGIGOGIGOGI