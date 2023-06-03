#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <WinIoCtl.h>
#include <Ntddscsi.h>
#include <iostream>
#include<string>
#include <cstdio>
#include <guiddef.h>
#include <shlobj_core.h>
#include<strsafe.h>
#include"ENstr.h"
#define SCSIOP_WRITE 0x2A
#define SCSIOP_READ 0x28
#define SCSI_R FALSE
#define SCSI_W TRUE
#define PHYSICAL_SECTOR_SIZE 512
//BOOL SCSIBuild10CDB(PSCSI_PASS_THROUGH_DIRECT srb, ULONGLONG offset, ULONG length, BOOLEAN write);
extern "C" __declspec(dllexport)BOOL SCSISectorIO(HANDLE hDrive, ULONGLONG offset, LPBYTE buffer, UINT buffSize, BOOLEAN write);
extern "C" __declspec(dllexport)BOOL GetPhysicalDriveHandle(HANDLE& handle, int driveNumber);
extern "C" __declspec(dllexport)BOOL DiskSectorIO(HANDLE hDrive, int Id, int num, unsigned char* buffer, BOOL write);
extern "C" __declspec(dllexport)int GetPhysicalDriveNumber();
extern "C" __declspec(dllexport)std::string GetPartitiontype(HANDLE hDevice);
extern "C" __declspec(dllexport)int GetSystemDiskPhysicalNumber();
extern "C" __declspec(dllexport)int SetDriveLetterToEFI(const char* devicePath, char driveLetter);
extern "C" __declspec(dllexport)void  LogicalLock();