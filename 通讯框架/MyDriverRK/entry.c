#include <ntifs.h>

// �豸���������
#define DEVICE_NAME L"\\device\\MyDriverRK"
// ��������
#define LINK_NAME L"\\dosDevices\\tkzc"

// ���������ʼ��ַ
#define IRP_IOCTRL_BASE 0x8000
// ����������ɺ꣨�豸���ͣ������룬ͨ�ŷ�ʽ��Ȩ�ޣ�
#define IRP_IOCTRL_CODE(i) CTL_CODE(FILE_DEVICE_UNKNOWN, IRP_IOCTRL_BASE + i, METHOD_BUFFERED, FILE_ANY_ACCESS)
// ����ͨ�ŵĿ����붨��
#define CTL_PRINT IRP_IOCTRL_CODE(1)

// IRP��ǲ����������������ʽ
NTSTATUS DispatchIoCtrl(PDRIVER_OBJECT pDriverObject, PIRP pIrp)
{
	// ������
	ULONG uIoCtrlCode = 0;
	// ������
	PVOID pBuffer = NULL;
	// ����
	ULONG uBufferLength = 0;
	// IRPջ
	PIO_STACK_LOCATION pStack = NULL;
	// ��ȡ������
	pBuffer = pIrp->AssociatedIrp.SystemBuffer;
	// ��ȡIRPջ
	pStack = IoGetCurrentIrpStackLocation(pIrp);
	// ��ȡ����������
	uBufferLength = pStack->Parameters.DeviceIoControl.InputBufferLength;
	// ��ȡ������
	uIoCtrlCode = pStack->Parameters.DeviceIoControl.IoControlCode;
	// ���տ�������зַ�
	switch (uIoCtrlCode)
	{
	case CTL_PRINT:
		DbgPrint("%s", pBuffer);
		RtlZeroMemory(pBuffer, 1024);
		ULONG uRetBufferLength = strlen("IO Success!");
		RtlCopyMemory(pBuffer, "IO Success!", uRetBufferLength);
		// ����IRP�����Ѿ��ɹ���
		pIrp->IoStatus.Status = STATUS_SUCCESS;
		// ���ض����ֽڵ�����
		pIrp->IoStatus.Information = uRetBufferLength;
		// ����IRP��������
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
		// �������óɹ�
		return STATUS_SUCCESS;
	default:
		break;
	}
}

// IRP��ǲ������������
NTSTATUS DispatchCreate(PDRIVER_OBJECT pDriverObject, PIRP pIrp)
{
	// ����IRP�����Ѿ��ɹ���
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	// ���ض����ֽڵ�����
	pIrp->IoStatus.Information = 0;
	// ����IRP��������
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	// �������óɹ�
	return STATUS_SUCCESS;
}

// IRP��ǲ��������Ĭ�ϴ���
NTSTATUS DispatchDefault(PDRIVER_OBJECT pDriverObject, PIRP pIrp)
{
	// ����IRP�����Ѿ��ɹ���
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	// ���ض����ֽڵ�����
	pIrp->IoStatus.Information = 0;
	// ����IRP��������
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	// �������óɹ�
	return STATUS_SUCCESS;
}

// IRP��ǲ����������ȡ
NTSTATUS DispatchRead(PDRIVER_OBJECT pDriverObject, PIRP pIrp)
{
	// ������ָ��
	PVOID pReadBuffer = NULL;
	// ����
	ULONG uReadLength = 0;
	// IRPջָ��
	PIO_STACK_LOCATION pStack = NULL;
	// ��ȡIRP��������ַ
	pReadBuffer = pIrp->AssociatedIrp.SystemBuffer;
	// ��ȡIRPջ
	pStack = IoGetCurrentIrpStackLocation(pIrp);
	// ��ȡ����
	uReadLength = pStack->Parameters.Read.Length;
	// ��ȡ���ص��ַ�������
	ULONG uStrLen = strlen("Read Success!");
	// �Ƚϣ�ȡ��Сֵ
	ULONG uMin = 0;
	uMin = uReadLength > uStrLen ? uStrLen : uReadLength;
	// ����������
	RtlCopyMemory(pReadBuffer, "Read Success!", uMin);
	// ����IRP�����Ѿ��ɹ���
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	// ���ض����ֽڵ�����
	pIrp->IoStatus.Information = uMin;
	// ����IRP��������
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	// �������óɹ�
	return STATUS_SUCCESS;
}

// IRP��ǲ��������д��
NTSTATUS DispatchWrite(PDRIVER_OBJECT pDriverObject, PIRP pIrp)
{
	// ������
	PVOID pWriteBuffer = NULL;
	// ����
	ULONG uWriteLength = 0;
	// IRPջָ��
	PIO_STACK_LOCATION pStack = NULL;
	// ��ȡIRP��������ַ
	pWriteBuffer = pIrp->AssociatedIrp.SystemBuffer;
	// ��ȡIRPջ
	pStack = IoGetCurrentIrpStackLocation(pIrp);
	// ��ȡ����
	uWriteLength = pStack->Parameters.Read.Length;
	// ��ȡ���ص��ַ�������
	ULONG uStrLen = strlen("Write Success!");
	// ��ӡ
	DbgPrint("%s", pWriteBuffer);
	// ����IRP�����Ѿ��ɹ���
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	// ���ض����ֽڵ�����
	pIrp->IoStatus.Information = uWriteLength;
	// ����IRP��������
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	// �������óɹ�
	return STATUS_SUCCESS;
}

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	DbgPrint("Unload Success!");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegPath)
{
	// �豸����
	UNICODE_STRING uDeviceName = { 0 };
	// ������������
	UNICODE_STRING uLinkName = { 0 };
	// ״̬
	NTSTATUS ntStatus = STATUS_SUCCESS;
	// �豸����
	PDRIVER_OBJECT pDeviceObject = NULL;
	// ��ʼ���ַ��������豸����
	RtlInitUnicodeString(&uDeviceName, DEVICE_NAME);
	// ��ʼ���ַ���������������
	RtlInitUnicodeString(&uLinkName, LINK_NAME);
	// �����豸����
	ntStatus = IoCreateDevice(pDriverObject, 0, &uDeviceName, FILE_DEVICE_UNKNOWN, 0, TRUE, &pDriverObject);
	if (!NT_SUCCESS(ntStatus))
	{
		IoDeleteDevice(pDriverObject);
		DbgPrint("IoCreateDevice Error!");
		return ntStatus;
	}

	pDeviceObject->Flags = DO_BUFFERED_IO; // ͨѶ��ʽ ���ڻ����IO��ʽ

	// ������������
	ntStatus = IoCreateSymbolicLink(&uLinkName, &uDeviceName);
	if (!NT_SUCCESS(ntStatus))
	{
		IoDeleteSymbolicLink(&uLinkName);
		DbgPrint("IoCreateSymbolicLink Error!");
		return ntStatus;
	}

	ULONG i = 0;
	for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION + 1; i++)
	{
		// ��������Ϣ����Ĭ�ϴ�����
		pDeviceObject->MajorFunction[i] = DispatchDefault;
	}
	pDeviceObject->MajorFunction[IRP_MJ_CREATE] = DispatchCreate; // ����Create��Ϣ������
	pDeviceObject->MajorFunction[IRP_MJ_READ] = DispatchRead;
	pDeviceObject->MajorFunction[IRP_MJ_WRITE] = DispatchWrite;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoCtrl;

	pDriverObject->DriverUnload = DriverUnload; // ��������ж�غ���
	DbgPrint("Hello Driver!");
	return STATUS_SUCCESS;
}