#include <ntifs.h>

NTSTATUS ProtecedProcessFun(ULONG ulPid)
{
	DWORD_PTR pEprocess = NULL;
	ULONG ulProcessID = 0;
	ULONG ulProtectedFlag = 0;
	pEprocess = (DWORD_PTR)PsGetCurrentProcess();
	PLIST_ENTRY HeaderNode = (PLIST_ENTRY)(pEprocess + 0xb8);
	PLIST_ENTRY NextNode = HeaderNode->Flink;
	while (NextNode != HeaderNode)
	{
		pEprocess = (DWORD_PTR)(NextNode - 0xb8);
		ulProcessID = *((ULONG*)(pEprocess + 0xb4));
		if (ulProcessID == ulPid)
		{
			ulProtectedFlag = *((ULONG*)(pEprocess + 0x26c));
			*((ULONG*)(pEprocess + 0x26c)) = ulProtectedFlag | 0x800;
			DbgPrint("Success!");
			return STATUS_SUCCESS;
		}
		NextNode = NextNode->Flink;
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
	pDriverObject->DriverUnload = DriverUnload;
	ProtecedProcessFun(4044);
	return STATUS_SUCCESS;
}