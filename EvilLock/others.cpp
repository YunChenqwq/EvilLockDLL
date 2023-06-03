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
/*��������*/
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
//��ȡ����Ȩ��
BOOL GetPrivileges()
{
	HANDLE hProcess;
	HANDLE hTokenHandle;
	TOKEN_PRIVILEGES tp;
	hProcess = GetCurrentProcess();
	if (!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hTokenHandle))
	{
	//	std::cerr <<("�޷��򿪽�������" )<< std::endl;
		return false;
	}
	tp.PrivilegeCount = 1;
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid))
	{
	//	std::cerr << "�޷���ȡ��Ȩֵ" << std::endl;
		CloseHandle(hTokenHandle);
		CloseHandle(hProcess);
		return false;
	}
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hTokenHandle, FALSE, &tp, sizeof(tp), NULL, NULL))
	{
	//	std::cerr << "�޷�������Ȩ" << std::endl;
		CloseHandle(hTokenHandle);
		CloseHandle(hProcess);
		return false;
	}
	CloseHandle(hTokenHandle);
	CloseHandle(hProcess);
	//std::cout << "��ȡ��Ȩ�ɹ�" << std::endl;
	return true;
}
/*��systemȨ�޴�һ������*/
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

	// ö�ٽ��̻�ȡ lsass.exe �� ID �� winlogon.exe �� ID�����������еĿ���ֱ�Ӵ򿪾����ϵͳ����
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

	// ��ȡ��������� lsass ���� winlogon
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, idL);
	if (!hProcess) hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, idW);
	HANDLE hTokenx;
	// ��ȡ����
	OpenProcessToken(hProcess, TOKEN_DUPLICATE, &hTokenx);
	// ��������
	DuplicateTokenEx(hTokenx, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hToken);
	CloseHandle(hProcess);
	CloseHandle(hTokenx);

	// ������Ϣ
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFOW));
	si.cb = sizeof(STARTUPINFOW);
	si.lpDesktop = (LPWSTR)L"winsta0\\default";  // ��ʾ����
	// ��������
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