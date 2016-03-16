#ifdef gogi
/*#include <Windows.h>*/
#include <time.h>
#include <random>
#include "resource.h"
#include <fstream>
using namespace std;

#define szWindowClass	TEXT("First")
#define szTitle			TEXT("First App")
#define MAX_FILE_SIZE	1024
#define PEN_WIDTH_MAX	20
#define PEN_WIDTH_MIN	1
#define RAND_SIZE_WIDTH_MAX		400
#define RAND_SIZE_WIDTH_MIN		50
#define RAND_SIZE_HEIGHT_MAX	400
#define RAND_SIZE_HEIGHT_MIN	50

unsigned currentSize = 0;

int currentWindowWidth = 0;
int currentWindowHeight = 0;
int RandBetween(int bigger, int smaller);

HDC	hdc;
PAINTSTRUCT	ps;
HWND hWnd;

LRESULT CALLBACK WndProc(HWND hWnd
	, UINT message
	, WPARAM wParam
	, LPARAM lParam);

HCURSOR LoadAnimatedCursor(HINSTANCE hInstance, UINT nID, LPCTSTR pszResouceType)
{
	HCURSOR hCursor = NULL;
	if (hInstance)
	{
		HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(nID), pszResouceType);
		DWORD dwResourceSize = SizeofResource(hInstance, hResource);
		if (dwResourceSize > 0)
		{
			HGLOBAL hRsrcGlobal = LoadResource(hInstance, hResource);
			if (hRsrcGlobal)
			{
				LPBYTE pResource = (LPBYTE)LockResource(hRsrcGlobal);
				if (pResource)
				{
					hCursor = (HCURSOR)CreateIconFromResource(pResource, dwResourceSize, FALSE, 0x00030000);
					UnlockResource(pResource);
				}
				FreeResource(hRsrcGlobal);
			}
		}
	}
	return hCursor;
}

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	//wcex.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wcex.hCursor = LoadAnimatedCursor(hInstance,IDR_ANIMATED_CURSOR2,L"ANIMATED_CURSOR");//LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName = szWindowClass;

	if (!RegisterClassEx(&wcex))
		return 0;

	hWnd = CreateWindowEx(WS_EX_APPWINDOW
		, szWindowClass
		, szTitle
		, WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, 800
		, 600
		, NULL
		, NULL
		, hInstance
		, NULL);

	if (!hWnd)
		return 0;

	WCHAR appTitle[256];
	LoadString(hInstance, IDS_TITLE, appTitle, 256);
	SetWindowText(hWnd, appTitle);

	ShowWindow(hWnd, nCmdShow);

	MSG			msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

enum PenStyle{
	PEN_STYLE_SOLID_FIRST = PS_SOLID,
	PEN_STYLE_DASH = PS_DASH,
	PEN_STYLE_DOT = PS_DOT,
	PEN_STYLE_DASHDOT = PS_DASHDOT,
	PEN_STYLE_DASHDOTDOT_LAST = PS_DASHDOTDOT
};
enum BrushStyle
{
	BRUSH_STYLE_FILL_FIRST = HS_HORIZONTAL - 1,
	BRUSH_STYLE_HORIZONTAL = HS_HORIZONTAL,
	BRUSH_STYLE_VERTICAL = HS_VERTICAL,
	BRUSH_STYLE_FDIAGONAL = HS_FDIAGONAL,
	BRUSH_STYLE_BDIAGONAL = HS_BDIAGONAL,
	BRUSH_STYLE_CROSS = HS_CROSS,
	BRUSH_STYLE_DIAGCROSS_LAST = HS_DIAGCROSS
};
typedef struct{
	int r;
	int g;
	int b;
} RgbData;
typedef struct{
	int left;
	int top;
	int width;
	int height;
}PositionAndStyle;

// enum BRUSH_STYLE{
// 	BRUSH_STYLE_WHITE
// 	BRUSH_STYLE_LTGRAY
// 	BRUSH_STYLE_GRAY
// 	BRUSH_STYLE_DKGRAY
// 	BRUSH_STYLE_BLACK
// 	BRUSH_STYLE_NULL
// 	BRUSH_STYLE_HOLLOW
// 	BRUSH_STYLE_WHITE_PEN
// 	BRUSH_STYLE_BLACK_PEN
// 	BRUSH_STYLE_NULL_PEN
// 	BRUSH_STYLE_OEM_FIXED_FONT
// 	BRUSH_STYLE_ANSI_FIXED_FONT
// 	BRUSH_STYLE_ANSI_VAR_FONT
// 	BRUSH_STYLE_SYSTEM_FONT
// 	BRUSH_STYLE_DEVICE_DEFAULT_FONT
// 	BRUSH_STYLE_DEFAULT_PALETTE
// 	BRUSH_STYLE_SYSTEM_FIXED_FONT
// };							

enum SomethingKind{
	SOMETHING_RECT,
	SOMETHING_CIRCLE
};

typedef struct{
	SomethingKind whatToDraw;
	PenStyle penStyle;
	int penWidth;
	RgbData penRGB;
	BrushStyle brushStyle;
	RgbData brushRGB;
	PositionAndStyle posAndStyle;
}종합선물셋트;

종합선물셋트 fileFileFile[MAX_FILE_SIZE];
COLORREF getColorRefFromRgbData(RgbData rgb)
{
	return RGB(rgb.r, rgb.g, rgb.b);
}

종합선물셋트 랜덤셋트생성()
{
	종합선물셋트 새로운셋트;

	새로운셋트.brushRGB.r = RandBetween(255, 0);
	새로운셋트.brushRGB.g = RandBetween(255, 0);
	새로운셋트.brushRGB.b = RandBetween(255, 0);
	새로운셋트.brushStyle = (BrushStyle)RandBetween(BRUSH_STYLE_DIAGCROSS_LAST, BRUSH_STYLE_FILL_FIRST);

	새로운셋트.penRGB.r = RandBetween(255, 0);
	새로운셋트.penRGB.g = RandBetween(255, 0);
	새로운셋트.penRGB.b = RandBetween(255, 0);
	새로운셋트.penStyle = (PenStyle)RandBetween(PEN_STYLE_DASHDOTDOT_LAST, PEN_STYLE_SOLID_FIRST);
	새로운셋트.penWidth = RandBetween(PEN_WIDTH_MAX, PEN_WIDTH_MIN);

	새로운셋트.posAndStyle.height = RandBetween(RAND_SIZE_HEIGHT_MAX, RAND_SIZE_HEIGHT_MIN);
	새로운셋트.posAndStyle.width = RandBetween(RAND_SIZE_WIDTH_MAX, RAND_SIZE_WIDTH_MIN);
	새로운셋트.posAndStyle.top = RandBetween(currentWindowHeight, 0);
	새로운셋트.posAndStyle.left = RandBetween(currentWindowWidth, 0);
	새로운셋트.whatToDraw = (SomethingKind)RandBetween(1, 0);

	return 새로운셋트;
}

/**
	원, 상자를 그릴 수 있다.
	*/
void DrawSomething(종합선물셋트 giftSet)
{

	SomethingKind whatToDraw = giftSet.whatToDraw;
	PenStyle penStyle = giftSet.penStyle;
	int penWidth = giftSet.penWidth;
	BrushStyle brushStyle = giftSet.brushStyle;
	RgbData brushRGB = giftSet.brushRGB;
	PositionAndStyle posAndStyle = giftSet.posAndStyle;
	RgbData penRGB = giftSet.penRGB;

	hdc = GetDC(hWnd);

	HBRUSH currentBrush = (brushStyle == BRUSH_STYLE_FILL_FIRST) ?
		CreateSolidBrush(getColorRefFromRgbData(brushRGB)) :
		CreateHatchBrush(brushStyle, getColorRefFromRgbData(brushRGB));

	HPEN currentPen = CreatePen(penStyle, penWidth, RGB(penRGB.r, penRGB.g, penRGB.b));

	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, currentBrush);
	HPEN oldPen = (HPEN)SelectObject(hdc, currentPen);

	switch (whatToDraw)
	{
	case SOMETHING_RECT:
		Rectangle(hdc, posAndStyle.left, posAndStyle.top, posAndStyle.left + posAndStyle.width, posAndStyle.top + posAndStyle.height);
		break;
	case SOMETHING_CIRCLE:
		Ellipse(hdc, posAndStyle.left, posAndStyle.top, posAndStyle.left + posAndStyle.width, posAndStyle.top + posAndStyle.height);
		break;
	}
	DeleteDC(hdc);
}

void OpenSavedData(WCHAR* fileName)
{

	FILE *p_file = _wfopen(fileName,L"rb");
	if (NULL != p_file){
		fread(&fileFileFile, 1, sizeof(fileFileFile), p_file);
		fclose(p_file);
	}
	unsigned i = 0;
	while (fileFileFile[i].penWidth != 0)
	{
		DrawSomething(fileFileFile[i]);
		++i;
	}
}

void SaveCurrentData(WCHAR* fileName)
{
	FILE *p_file = _wfopen(fileName, L"wb");
	if (NULL != p_file){
		fwrite(fileFileFile, 1, sizeof(fileFileFile), p_file);
		fclose(p_file);
	}
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		srand(time(nullptr));
		{
			RECT rect = tagRECT();
			if (GetWindowRect(hWnd, &rect))
			{
				currentWindowWidth = rect.right - rect.left;
				currentWindowHeight = rect.bottom - rect.top;
			}
		}

		break;
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		종합선물셋트 newSet;
		newSet.whatToDraw = SOMETHING_RECT;
		newSet.posAndStyle.width = 100;
		newSet.posAndStyle.height = 100;
		newSet.posAndStyle.top = y;
		newSet.posAndStyle.left = x;
		newSet.brushStyle = BRUSH_STYLE_FILL_FIRST;
		newSet.brushRGB.r = 255;
		newSet.brushRGB.g = 255;
		newSet.brushRGB.b = 255;
		newSet.penStyle = PEN_STYLE_SOLID_FIRST;
		newSet.penWidth = 1;
		newSet.penRGB.r = 0;
		newSet.penRGB.g = 0;
		newSet.penRGB.b = 0;

		DrawSomething(newSet);
		fileFileFile[currentSize++] = newSet;
	}
	break;
	case WM_LBUTTONDOWN:
	{
		종합선물셋트 newSet = 랜덤셋트생성();
		DrawSomething(newSet);
		fileFileFile[currentSize++] = newSet;
	}
	break;
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		종합선물셋트 newSet;
		newSet.whatToDraw = SOMETHING_RECT;
		newSet.posAndStyle.width = 100;
		newSet.posAndStyle.height = 100;
		newSet.posAndStyle.top = y;
		newSet.posAndStyle.left = x;
		newSet.brushStyle = BRUSH_STYLE_FILL_FIRST;
		newSet.brushRGB.r = 255;
		newSet.brushRGB.g = 255;
		newSet.brushRGB.b = 255;
		newSet.penStyle = PEN_STYLE_SOLID_FIRST;
		newSet.penWidth = 1;
		newSet.penRGB.r = 0;
		newSet.penRGB.g = 0;
		newSet.penRGB.b = 0;

		DrawSomething(newSet);
		fileFileFile[currentSize++] = newSet;
	}
	break;
	//리사이즈할때
	case WM_SIZE:
	{
		RECT rect;
		if (GetWindowRect(hWnd, &rect))
		{
			currentWindowWidth = rect.right - rect.left;
			currentWindowHeight = rect.bottom - rect.top;
		}
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_FILE_EXIT:
			DestroyWindow(hWnd);
		case ID_FILE_OPEN:
		{
			OPENFILENAMEW OFN;
			WCHAR str[300];
			WCHAR lpstrFile[MAX_PATH] = L"";
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hWnd;
			OFN.lpstrFilter = L"Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0";
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = 256;
			OFN.lpstrInitialDir = L"c:\\";
			if (GetOpenFileName(&OFN) != 0) {
				wsprintf(str, L"%s 파일을 선택했습니다.", OFN.lpstrFile);
				MessageBox(hWnd, str, L"파일 열기 성공", MB_OK);
				currentSize = 0;
				InvalidateRect(hWnd, nullptr, true);
				UpdateWindow(hWnd);
				OpenSavedData(OFN.lpstrFile);
			}
		}
		break;
		case ID_FILE_SAVE:
		{
			OPENFILENAMEW OFN;
			WCHAR str[300];
			WCHAR lpstrFile[MAX_PATH] = L"";
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hWnd;
			OFN.lpstrFilter = L"Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0";
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = 256;
			OFN.lpstrInitialDir = L"c:\\";
			if (GetOpenFileName(&OFN) != 0)
			{
				wsprintf(str, L"%s 파일을 선택했습니다.", OFN.lpstrFile);
				SaveCurrentData(OFN.lpstrFile);
				MessageBox(hWnd, str, L"파일 저장 성공", MB_OK);
			}
		}
		break;
		}
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
int RandBetween(int max, int min)
{
	return min + (rand() % (max - min + 1));
}
#endif