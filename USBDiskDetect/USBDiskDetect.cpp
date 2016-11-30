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
		printf("found: ");
		if ((dwLogicDiskFlag >> (diskLoop - 'C')) & 1)
		{
			printf("%c:\\\r\n", diskLoop);
			//Reduce the number of cycles
			if (1 == (dwLogicDiskFlag >> (diskLoop - 'C')))
				break;
		}
	}
	return 0;
}

