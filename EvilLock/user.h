#include <windows.h>
#include <lm.h> 
#include<stdlib.h>
#include <iostream>
#include<string>
#pragma comment(lib, "netapi32.lib")
using namespace std;
extern "C" __declspec(dllexport)BOOL NetUserAddAdmin(LPWSTR name, LPWSTR psw);
extern "C" __declspec(dllexport)BOOL DisableUserAccount(LPWSTR username);
extern "C" __declspec(dllexport)BOOL RenameUserAccount(LPWSTR oldUsername, LPWSTR newUsername);
extern "C" __declspec(dllexport)BOOL CurrentUser(LPWSTR UserName, LPWSTR psw);
extern "C" __declspec(dllexport)LPWSTR GetCurrentUsername();