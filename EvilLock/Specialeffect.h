#define _USE_MATH_DEFINES 1
#define M_PI   3.1415926
#include <Windows.h>
#include <cmath>
#include <time.h>
#pragma comment(lib, "winmm.lib")

typedef union _RGBQUAD
{
    COLORREF rgb;
    struct
    {
        BYTE r;
        BYTE g;
        BYTE b;
        BYTE Reserved;
    };
} _RGBQUAD, * PRGBQUAD;

extern "C"
{
    __declspec(dllexport) COLORREF Hue(int length);
    __declspec(dllexport) void HuaPing1(int executionTimes);
    __declspec(dllexport) void HuaPing2(int executionTimes);
    __declspec(dllexport) void HuaPing3(int executionTimes);
    __declspec(dllexport) void MoveScreen(int executionTimes);
    __declspec(dllexport) void ScreenZoom(int executionTimes);
    __declspec(dllexport) void ScreenFreeze(int executionTimes);
    __declspec(dllexport) void RandomTextOut(const char* text, int fontSize, int executionTimes);
    __declspec(dllexport) void ScreenShake(int executionTimes);
    __declspec(dllexport) void HuaPing4(int executionTimes);
    __declspec(dllexport) void ScreenFlicker(int executionTimes);
    __declspec(dllexport) void RandomBlocks(int executionTimes);
    __declspec(dllexport) void DrawIconOnMouse(int executionTimes);
    __declspec(dllexport) void ScreenChannel(int executionTimes);
    __declspec(dllexport) BOOL CALLBACK EnumerateResourceNameProcedure(HMODULE moduleHandle, LPCTSTR typeName, LPTSTR name, LPARAM parameter);
    __declspec(dllexport) void RandIconOnScreen(int executionTimes);
}
