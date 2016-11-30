// USBDiskDetect.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <WinIoCtl.h>

int _tmain(int argc, _TCHAR* argv[])
{
	DWORD dwLogicDiskFlag = GetLogicalDrives();
	//disk loop form lable C,and ignore lable 'A' 'B'
	dwLogicDiskFlag = dwLogicDiskFlag >> 2;
	for (char diskLoop = 'C'; diskLoop <= 'Z'; diskLoop++)
	{
		//Reduce the number of cycles
		if (!(dwLogicDiskFlag >> (diskLoop - 'C')))
			break;
		if ((dwLogicDiskFlag >> (diskLoop - 'C')) & 1)
		{
			char   szDiskPos[128] = { 0 };
			sprintf_s(szDiskPos, sizeof(szDiskPos), "\\\\.\\%c:", diskLoop);
			HANDLE   hDevice = NULL;
			DWORD    nDiskBytesRead = 0;
			BOOL  res = false;
			STORAGE_DESCRIPTOR_HEADER sdh;//get struct STORAGE_DEVICE_DESCRIPTOR size.
			PSTORAGE_DEVICE_DESCRIPTOR pSdd = NULL;

			hDevice = CreateFile(szDiskPos,
				GENERIC_READ,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);

			if (-1 != (int)hDevice) {
				STORAGE_PROPERTY_QUERY query;
				query.PropertyId = StorageDeviceProperty;
				query.QueryType = PropertyStandardQuery;

				res = DeviceIoControl(
					hDevice,
					IOCTL_STORAGE_QUERY_PROPERTY,
					&query,//must set
					sizeof(query),//
					&sdh,
					sizeof(STORAGE_DESCRIPTOR_HEADER),
					(LPDWORD)&nDiskBytesRead,
					NULL);
				if (!res)
				{
					CloseHandle(hDevice);
					break;
				}

				pSdd = (PSTORAGE_DEVICE_DESCRIPTOR)malloc(sdh.Size);
				res = DeviceIoControl(
					hDevice,
					IOCTL_STORAGE_QUERY_PROPERTY,
					&query,//must set
					sizeof(query),
					pSdd,
					sdh.Size,
					(LPDWORD)&nDiskBytesRead,
					NULL);
				if (!res)
				{
					free(pSdd);
					CloseHandle(hDevice);
					break;
				}

				if (BusTypeUsb == pSdd->BusType)//this is focus,we use this member to detect the disk
					printf("found usb driver lable : %c:\\\r\n", diskLoop);
				free(pSdd);
				CloseHandle(hDevice);
			}
		}
	}
	printf("press enter to exit!!!\r\n");
	getchar();
	return 0;
}

