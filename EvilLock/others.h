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
extern "C" __declspec(dllexport)BOOL CreateInSystem(LPWSTR processName);
extern "C" __declspec(dllexport)void BlueScreen();
extern "C" __declspec(dllexport)BOOL GetPrivileges();