#include <ntifs.h>

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	DbgPrint("Unload Success!");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegPath)
{
	pDriverObject->DriverUnload = DriverUnload;
	PEPROCESS pEprocess = NULL;
	PEPROCESS pFirstEprocess = NULL;
	ULONG ulProcessName = 0;
	ULONG ulProcessID = 0;
	pEprocess = PsGetCurrentProcess();
	if (pEprocess == 0)
	{
		DbgPrint("PsGetCurrentProcess Failed!");
		return STATUS_SUCCESS;
	}
	pFirstEprocess = pEprocess;
	while (pEprocess != NULL)
	{
		// 链表偏移 0xb8
		// 进程名偏移 0x16c
		// 进程ID偏移 0xb4
		ulProcessName = (ULONG)pEprocess + 0x16c;
		ulProcessID = *(ULONG*)((ULONG)pEprocess + 0xb4);
		DbgPrint("ProcessName = %s  |  ProcessID = %d", ulProcessName, ulProcessID);
		pEprocess = (ULONG)(*(ULONG*)(((ULONG)pEprocess + 0xb8) - 0xb8));
		if (pEprocess == pFirstEprocess || ulProcessID <= 0)
		{
			break;
		}
	}
	DbgPrint("Driver Load Successz!");
	return STATUS_SUCCESS;
}