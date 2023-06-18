#include <windows.h>
#include <lm.h> 
#include <stdlib.h>
#include <iostream>
#include <string>
#pragma comment(lib, "netapi32.lib")
using namespace std;

extern "C"
{
    __declspec(dllexport) BOOL NetUserAddAdmin(LPWSTR name, LPWSTR psw);
    __declspec(dllexport) BOOL DisableUserAccount(LPWSTR username);
    __declspec(dllexport) BOOL RenameUserAccount(LPWSTR oldUsername, LPWSTR newUsername);
    __declspec(dllexport) BOOL CurrentUser(LPWSTR UserName, LPWSTR psw);
    __declspec(dllexport) LPWSTR GetCurrentUsername();
}
