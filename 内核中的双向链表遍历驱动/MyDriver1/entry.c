#include <ntifs.h>

VOID DriverUnload(PDRIVER_OBJECT pDeriverObject)
{
	DbgPrint("Driver Unload Success!");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegPath)
{
	pDriverObject->DriverUnload = DriverUnload;
	DbgPrint("Driver Load Success!");
	PLIST_ENTRY HeaderNode = NULL;
	PLIST_ENTRY NextNode = NULL;
	PUNICODE_STRING pusKernelFileName;
	HeaderNode = (PLIST_ENTRY)(pDriverObject->DriverSection);
	NextNode = HeaderNode->Flink;
	while (HeaderNode != NextNode)
	{
		pusKernelFileName = (PUNICODE_STRING)((ULONG)NextNode + 0x2c);
		DbgPrint("%wZ", pusKernelFileName);
		NextNode = NextNode->Flink;
	}
	return STATUS_SUCCESS;
}