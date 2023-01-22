#include <ntifs.h>

PCHAR PsGetProcessImageFileName(PEPROCESS pEprocess);

typedef struct _KLDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY list_entry;
	ULONG ulNum1;
	ULONG ulNum2;
	ULONG ulNum3;
	ULONG ulNum4;
	ULONG ulNum5;
	ULONG ulNum6;
	ULONG ulNum7;
	UNICODE_STRING path;
	UNICODE_STRING name;
	ULONG ulFlags;
} KLDR_DATA_TABLE_ENTRY, * PKLDR_DATA_TABLE_ENTRY;

VOID ProcessNotifyExRoutine(PEPROCESS pEprocess, HANDLE hProcess, PPS_CREATE_NOTIFY_INFO CreateInfo)
{
	if (CreateInfo == NULL)
	{
		return;
	}
	PCHAR pszImageFileName = PsGetProcessImageFileName(pEprocess);
	if (0 == _stricmp(pszImageFileName, "DllInject.exe"))
	{
		CreateInfo->CreationStatus = STATUS_UNSUCCESSFUL;
	}
}

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	NTSTATUS ntStatus = PsSetCreateProcessNotifyRoutineEx(
		(PCREATE_PROCESS_NOTIFY_ROUTINE_EX)ProcessNotifyExRoutine, // 指向要注册或删除 PCREATE_PROCESS_NOTIFY_ROUTINE_EX 例程的指针。 每当创建新进程时，操作系统都调用此例程
		TRUE // 指定 PsSetCreateProcessNotifyRoutineEx 是否会在回调例程列表中添加或删除指定的例程
	);
	if (!NT_SUCCESS(ntStatus))
	{
		DbgPrint("PsSetCreateProcessNotifyRoutineEx Failed!");
	}
	DbgPrint("Driver Unload Success!");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegPath)
{
	PKLDR_DATA_TABLE_ENTRY pKDRE = (PKLDR_DATA_TABLE_ENTRY)pDriverObject->DriverSection;
	pKDRE->ulFlags = pKDRE->ulFlags | 0x24;
	// PsSetCreateProcessNotifyRoutineEx 例程注册或删除回调例程，在创建或退出进程时通知调用方
	NTSTATUS ntStatus = PsSetCreateProcessNotifyRoutineEx(
		(PCREATE_PROCESS_NOTIFY_ROUTINE_EX)ProcessNotifyExRoutine, // 指向要注册或删除 PCREATE_PROCESS_NOTIFY_ROUTINE_EX 例程的指针。 每当创建新进程时，操作系统都调用此例程
		FALSE // 指定 PsSetCreateProcessNotifyRoutineEx 是否会在回调例程列表中添加或删除指定的例程
	);
	if (!NT_SUCCESS(ntStatus))
	{
		DbgPrint("PsSetCreateProcessNotifyRoutineEx Failed!");
	}
	pDriverObject->DriverUnload = DriverUnload;
	DbgPrint("Driver Load Success!");
	return STATUS_SUCCESS;
}