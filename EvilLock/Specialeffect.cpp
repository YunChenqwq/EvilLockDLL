#include"Specialeffect.h"
int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) { // 根据长度生成颜色
	if (red != length) {
		red < length;
		red++;
		if (ifblue == true) {
			return RGB(red, 0, length);
		}
		else {
			return RGB(red, 0, 0);
		}
	}
	else {
		if (green != length) {
			green < length;
			green++;
			return RGB(length, green, 0);
		}
		else {
			if (blue != length) {
				blue < length;
				blue++;
				return RGB(0, length, blue);
			}
			else {
				red = 0;
				green = 0;
				blue = 0;
				ifblue = true;
			}
		}
	}
}

void HuaPing1(int executionTimes) {
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);

	for (int execution = 0; execution < executionTimes; execution++) {
		hdcScreen = GetDC(0);
		BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			rgbScreen[i].rgb *= x ^ y;
		}
		BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		ReleaseDC(NULL, hdcScreen); DeleteDC(hdcScreen);
	}
}
void HuaPing2(int executionTimes) {
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	hdcScreen = GetDC(0);
	BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);

	// 执行传入的次数
	for (int execution = 0; execution < executionTimes; execution++) {
		for (INT i = 0; i < w * h; i++) {
			rgbScreen[i].rgb *= 5;
		}
	}

	BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
	ReleaseDC(NULL, hdcScreen);
	DeleteDC(hdcScreen);
}
void HuaPing3(int executionTimes) {
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	hdcScreen = GetDC(0);
	BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);

	// 执行传入的次数
	for (int execution = 0; execution < executionTimes; execution++) {
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			rgbScreen[i].rgb += x * y;
		}
	}

	BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
	ReleaseDC(NULL, hdcScreen);
	DeleteDC(hdcScreen);
}
void MoveScreen(int executionTimes) {
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	HDC hdc = GetDC(0);
	HBRUSH brush = CreateSolidBrush(Hue(239));
	SelectObject(hdc, brush);

	// 执行传入的次数
	for (int execution = 0; execution < executionTimes; execution++) {
		BitBlt(hdc, -1, 0, w, h, hdc, 0, 0, SRCCOPY);
		BitBlt(hdc, w - 1, 0, w, h, hdc, 0, 0, PATCOPY);
	}

	DeleteObject(brush);
	ReleaseDC(0, hdc);
}
void ScreenZoom(int executionTimes) {
	int sw, sh;
	HDC hdc = GetDC(0);
	sw = GetSystemMetrics(0);
	sh = GetSystemMetrics(1);

	// 执行传入的次数
	for (int execution = 0; execution < executionTimes; execution++) {
		StretchBlt(hdc, -20, -20, sw + 40, sh + 40, hdc, 0, 0, sw, sh, SRCCOPY);
		Sleep(4);
	}

	ReleaseDC(0, hdc);
}
void ScreenFreeze(int executionTimes) {
	HDC hdc = GetDC(0);
	int sw = GetSystemMetrics(0);
	int sh = GetSystemMetrics(1);
	SetStretchBltMode(hdc, HALFTONE);

	// 执行传入的次数
	for (int execution = 0; execution < executionTimes; execution++) {
		StretchBlt(hdc, 0, 0, sw + 1, sh - 1, hdc, 0, 0, sw, sh, SRCCOPY);
		StretchBlt(hdc, 0, 0, sw - 1, sh + 1, hdc, 0, 0, sw, sh, SRCCOPY);
	}

	ReleaseDC(0, hdc);
}
void RandomTextOut(const char* text, int fontSize, int executionTimes) {
	int x = GetSystemMetrics(0);
	int y = GetSystemMetrics(1);

	// 执行传入的次数
	for (int execution = 0; execution < executionTimes; execution++) {
		HDC hdc = GetDC(0);
		SetBkMode(hdc, 0);
		SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));

		HFONT font = CreateFontA(fontSize, 0, 0, 0, FW_THIN, 0, 1, 0, ANSI_CHARSET, 0, 0, 0, 0, "Arial");
		SelectObject(hdc, font);
		TextOutA(hdc, rand() % x, rand() % y, text, strlen(text));

		DeleteObject(font);
		ReleaseDC(0, hdc);
		Sleep(10);
	}
}
void ScreenShake(int executionTimes) {
	HDC hdc = GetDC(NULL);
	int sw = GetSystemMetrics(SM_CXSCREEN), sh = GetSystemMetrics(SM_CYSCREEN);

	// 执行传入的次数
	for (int execution = 0; execution < executionTimes; execution++) {
		BitBlt(hdc, rand() % 10 - 5, rand() % 10 - 5, rand() % sw, rand() % sh, hdc, rand() % 10 - 5, rand() % 10 - 5, SRCCOPY);
	}

	ReleaseDC(0, hdc);
}
void HuaPing4(int executionTimes) {
	HDC hdcScreen = GetDC(0), hdcMem = CreateCompatibleDC(hdcScreen);
	INT w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	BITMAPINFO bmi = { 0 };
	PRGBQUAD rgbScreen = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = h;
	HBITMAP hbmTemp = CreateDIBSection(hdcScreen, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
	SelectObject(hdcMem, hbmTemp);
	hdcScreen = GetDC(0);
	BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);

	// 执行传入的次数
	for (int execution = 0; execution < executionTimes; execution++) {
		for (INT i = 0; i < w * h; i++) {
			INT x = i % w, y = i / w;
			rgbScreen[i].rgb += x ^ y;
		}
	}

	BitBlt(hdcScreen, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
	ReleaseDC(NULL, hdcScreen);
	DeleteDC(hdcScreen);
}
void ScreenFlicker(int executionTimes) {
	HDC hdc = GetDC(NULL);
	int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);

	// 执行传入的次数
	for (int execution = 0; execution < executionTimes; execution++) {
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, brush);
		PatBlt(hdc, 0, 0, w, h, PATINVERT);
		DeleteObject(brush);
	}

	ReleaseDC(0, hdc);
}
void RandomBlocks(int executionTimes) {
	HWND hwnd;
	const int NUM = 500;
	if (LockWindowUpdate(hwnd = GetDesktopWindow())) {
		HDC hdcScr = GetDCEx(hwnd, NULL, DCX_CACHE | DCX_LOCKWINDOWUPDATE);
		HDC hdcMem = CreateCompatibleDC(hdcScr);
		int cxClient = GetSystemMetrics(SM_CXSCREEN) / 10;
		int cyClient = GetSystemMetrics(SM_CYSCREEN) / 10;
		HBITMAP hBitmap = CreateCompatibleBitmap(hdcScr, cxClient, cyClient);
		SelectObject(hdcMem, hBitmap);
		srand((int)GetCurrentTime());
		int iKeep[NUM][4];
		int	x1, x2, y1, y2;
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < executionTimes; ++j) {
				if (0 == i) {
					iKeep[j][0] = x1 = cxClient * (rand() % 10);
					iKeep[j][1] = y1 = cyClient * (rand() % 10);
					iKeep[j][2] = x2 = cxClient * (rand() % 10);
					iKeep[j][3] = y2 = cyClient * (rand() % 10);
				}
				else {
					x1 = iKeep[NUM - 1 - j][0];
					y1 = iKeep[NUM - 1 - j][1];
					x2 = iKeep[NUM - 1 - j][2];
					y2 = iKeep[NUM - 1 - j][3];
				}
				BitBlt(hdcMem, 0, 0, cxClient, cyClient, hdcScr, x1, y1, SRCCOPY);
				BitBlt(hdcScr, x1, y1, cxClient, cyClient, hdcScr, x2, y2, SRCCOPY);
				BitBlt(hdcScr, x2, y2, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);
				Sleep(10);
			}
		}
		DeleteDC(hdcMem);
		ReleaseDC(hwnd, hdcScr);
		DeleteObject(hBitmap);
		LockWindowUpdate(NULL);
	}
}
void DrawIconOnMouse(int executionTimes) {
	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetWindowDC(hwnd);
	POINT point;
	for (int execution = 0; execution < executionTimes; execution++) {
		GetCursorPos(&point);
		DrawIcon(hdc, point.x - 45, point.y, LoadIcon(NULL, IDI_ERROR));
		DrawIcon(hdc, point.x, point.y + 20, LoadIcon(NULL, IDI_ASTERISK));
		DrawIcon(hdc, point.x - 15, point.y - 20, LoadIcon(NULL, IDI_WARNING));
		Sleep(10);
	}
	ReleaseDC(hwnd, hdc);
}
void ScreenChannel(int executionTimes) {
	HWND hwnd = GetDesktopWindow();
	HDC hdc = GetWindowDC(hwnd);
	for (int execution = 0; execution < executionTimes; execution++) {
		StretchBlt(hdc, 0, 30, GetSystemMetrics(SM_CXSCREEN) - 100, GetSystemMetrics(SM_CYSCREEN) - 80, hdc, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SRCCOPY);
		Sleep(10);
	}
	ReleaseDC(hwnd, hdc);
}
BOOL CALLBACK EnumerateResourceNameProcedure(HMODULE moduleHandle, LPCTSTR typeName, LPTSTR name, LPARAM parameter) {
	int screenWidth = GetSystemMetrics(SM_CXSCREEN), screenHeight = GetSystemMetrics(SM_CYSCREEN);
	if (typeName == RT_GROUP_ICON) {
		POINT position;
		GetCursorPos(&position);
		HICON iconHandle = LoadIcon(moduleHandle, name);
		int iconWidth = GetSystemMetrics(SM_CXICON) / 2, iconHeight = GetSystemMetrics(SM_CYICON) / 2;
		HDC deviceContext = GetDC(NULL);
		DrawIcon(deviceContext, rand() % screenWidth, rand() % screenHeight, LoadIcon(moduleHandle, name));
		ReleaseDC(NULL, deviceContext);
	}
	return TRUE;
}
void RandIconOnScreen(int executionTimes)
{
	for (int execution = 0; execution < executionTimes; execution++) {
		srand(time(NULL));
		HMODULE shell32Handle = LoadLibrary("shell32.dll");
		int screenWidth = GetSystemMetrics(SM_CXSCREEN), screenHeight = GetSystemMetrics(SM_CYSCREEN);

		EnumResourceNamesA(shell32Handle, RT_GROUP_ICON, EnumerateResourceNameProcedure, 20);
	}
}