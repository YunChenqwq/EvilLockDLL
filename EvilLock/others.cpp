#include"others.h"
using namespace std;
typedef /*__success(return >= 0)*/ LONG NTSTATUS;
typedef NTSTATUS* PNTSTATUS;
#define STATUS_SUCCESS  ((NTSTATUS)0x00000000L)
typedef struct _LSA_UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} LSA_UNICODE_STRING, * PLSA_UNICODE_STRING, UNICODE_STRING, * PUNICODE_STRING;

typedef enum _HARDERROR_RESPONSE_OPTION {
	OptionAbortRetryIgnore,
	OptionOk,
	OptionOkCancel,
	OptionRetryCancel,
	OptionYesNo,
	OptionYesNoCancel,
	OptionShutdownSystem
} HARDERROR_RESPONSE_OPTION, * PHARDERROR_RESPONSE_OPTION;

typedef enum _HARDERROR_RESPONSE {
	ResponseReturnToCaller,
	ResponseNotHandled,
	ResponseAbort,
	ResponseCancel,
	ResponseIgnore,
	ResponseNo,
	ResponseOk,
	ResponseRetry,
	ResponseYes
} HARDERROR_RESPONSE, * PHARDERROR_RESPONSE;
typedef UINT(CALLBACK* NTRAISEHARDERROR)(NTSTATUS, ULONG, PUNICODE_STRING, PVOID, HARDERROR_RESPONSE_OPTION, PHARDERROR_RESPONSE);
typedef UINT(CALLBACK* RTLADJUSTPRIVILEGE)(ULONG, BOOL, BOOL, PINT);
/*触发蓝屏*/
void BlueScreen() {

	HINSTANCE hDLL = LoadLibrary(TEXT("ntdll.dll"));
	NTRAISEHARDERROR NtRaiseHardError;
	RTLADJUSTPRIVILEGE RtlAdjustPrivilege;
	int nEn = 0;
	HARDERROR_RESPONSE reResponse;
	if (hDLL != NULL)
	{
		NtRaiseHardError = (NTRAISEHARDERROR)GetProcAddress(hDLL, "NtRaiseHardError");
		RtlAdjustPrivilege = (RTLADJUSTPRIVILEGE)GetProcAddress(hDLL, "RtlAdjustPrivilege");
		if (!NtRaiseHardError)
		{
			FreeLibrary(hDLL);
		}
		if (!RtlAdjustPrivilege)
		{
			FreeLibrary(hDLL);
		}
		RtlAdjustPrivilege(0x13, TRUE, FALSE, &nEn);//0x13 = SeShutdownPrivilege
		NtRaiseHardError(0xC000021A, 0, 0, 0, OptionShutdownSystem, &reResponse);
	}
}
//获取调试权限
BOOL GetPrivileges()
{
	HANDLE hProcess;
	HANDLE hTokenHandle;
	TOKEN_PRIVILEGES tp;
	hProcess = GetCurrentProcess();
	if (!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hTokenHandle))
	{
	//	std::cerr <<("无法打开进程令牌" )<< std::endl;
		return false;
	}
	tp.PrivilegeCount = 1;
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid))
	{
	//	std::cerr << "无法获取特权值" << std::endl;
		CloseHandle(hTokenHandle);
		CloseHandle(hProcess);
		return false;
	}
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hTokenHandle, FALSE, &tp, sizeof(tp), NULL, NULL))
	{
	//	std::cerr << "无法调整特权" << std::endl;
		CloseHandle(hTokenHandle);
		CloseHandle(hProcess);
		return false;
	}
	CloseHandle(hTokenHandle);
	CloseHandle(hProcess);
	//std::cout << "获取特权成功" << std::endl;
	return true;
}
/*以system权限打开一个程序*/
BOOL CreateInSystem(LPWSTR processName)
{
	HANDLE hToken;
	LUID Luid;
	TOKEN_PRIVILEGES tp;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &Luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = Luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, false, &tp, sizeof(tp), NULL, NULL);
	CloseHandle(hToken);

	// 枚举进程获取 lsass.exe 的 ID 和 winlogon.exe 的 ID，它们是少有的可以直接打开句柄的系统进程
	DWORD idL{}, idW{};
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (Process32First(hSnapshot, &pe)) {
		do {
			if (0 == _stricmp(pe.szExeFile, "lsass.exe")) {
				idL = pe.th32ProcessID;
			}
			else if (0 == _stricmp(pe.szExeFile, "winlogon.exe")) {
				idW = pe.th32ProcessID;
			}
		} while (Process32Next(hSnapshot, &pe));
	}
	CloseHandle(hSnapshot);

	// 获取句柄，先试 lsass 再试 winlogon
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, idL);
	if (!hProcess) hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, idW);
	HANDLE hTokenx;
	// 获取令牌
	OpenProcessToken(hProcess, TOKEN_DUPLICATE, &hTokenx);
	// 复制令牌
	DuplicateTokenEx(hTokenx, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hToken);
	CloseHandle(hProcess);
	CloseHandle(hTokenx);

	// 启动信息
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFOW));
	si.cb = sizeof(STARTUPINFOW);
	si.lpDesktop = (LPWSTR)L"winsta0\\default";  // 显示窗口
	// 启动进程
	BOOL result = CreateProcessWithTokenW(hToken, LOGON_NETCREDENTIALS_ONLY,
		NULL, processName, NORMAL_PRIORITY_CLASS,
		NULL, NULL, &si, &pi);
	CloseHandle(hToken);
	if (!result) {
		return false;
	}
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return true;
}