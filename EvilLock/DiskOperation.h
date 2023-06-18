#define _CRT_SECURE_NO_WARNINGS
#define _NTSCSI_USER_MODE_
#include <Windows.h>
#include <WinIoCtl.h>
#include <Ntddscsi.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <guiddef.h>
#include <shlobj_core.h>
#include <strsafe.h>
#include <scsi.h>
#include "ENstr.h"
#define SCSI_R FALSE
#define SCSI_W TRUE
#define PHYSICAL_SECTOR_SIZE 512
extern "C"
{
    //BOOL SCSIBuild10CDB(PSCSI_PASS_THROUGH_DIRECT srb, ULONGLONG offset, ULONG length, BOOLEAN write);
    __declspec(dllexport) BOOL SCSISectorIO(HANDLE hDrive, ULONGLONG offset, LPBYTE buffer, UINT buffSize, BOOLEAN write);
    __declspec(dllexport) BOOL GetPhysicalDriveHandle(HANDLE& handle, int driveNumber);
    __declspec(dllexport) BOOL DiskSectorIO(HANDLE hDrive, int Id, int num, unsigned char* buffer, BOOL write);
    __declspec(dllexport) int GetPhysicalDriveNumber();
    __declspec(dllexport) const char* GetPartitiontype(HANDLE hDevice);
    __declspec(dllexport) int GetSystemDiskPhysicalNumber();
    __declspec(dllexport) int SetDriveLetterToEFI(const char* devicePath, char driveLetter);
}
