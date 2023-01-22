#include <ntifs.h>

// 设备对象的名称
#define DEVICE_NAME L"\\device\\MyDriverRK"
// 符号链接
#define LINK_NAME L"\\dosDevices\\tkzc"

// 控制码的起始地址
#define IRP_IOCTRL_BASE 0x8000
// 控制码的生成宏（设备类型，控制码，通信方式，权限）
#define IRP_IOCTRL_CODE(i) CTL_CODE(FILE_DEVICE_UNKNOWN, IRP_IOCTRL_BASE + i, METHOD_BUFFERED, FILE_ANY_ACCESS)
// 用于通信的控制码定义
#define CTL_PRINT IRP_IOCTRL_CODE(1)

// IRP派遣函数——控制码形式
NTSTATUS DispatchIoCtrl(PDRIVER_OBJECT pDriverObject, PIRP pIrp)
{
	// 控制码
	ULONG uIoCtrlCode = 0;
	// 缓冲区
	PVOID pBuffer = NULL;
	// 长度
	ULONG uBufferLength = 0;
	// IRP栈
	PIO_STACK_LOCATION pStack = NULL;
	// 获取缓冲区
	pBuffer = pIrp->AssociatedIrp.SystemBuffer;
	// 获取IRP栈
	pStack = IoGetCurrentIrpStackLocation(pIrp);
	// 获取缓冲区长度
	uBufferLength = pStack->Parameters.DeviceIoControl.InputBufferLength;
	// 获取控制码
	uIoCtrlCode = pStack->Parameters.DeviceIoControl.IoControlCode;
	// 按照控制码进行分发
	switch (uIoCtrlCode)
	{
	case CTL_PRINT:
		DbgPrint("%s", pBuffer);
		RtlZeroMemory(pBuffer, 1024);
		ULONG uRetBufferLength = strlen("IO Success!");
		RtlCopyMemory(pBuffer, "IO Success!", uRetBufferLength);
		// 设置IRP处理已经成功了
		pIrp->IoStatus.Status = STATUS_SUCCESS;
		// 返回多少字节的数据
		pIrp->IoStatus.Information = uRetBufferLength;
		// 结束IRP处理流程
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);
		// 函数调用成功
		return STATUS_SUCCESS;
	default:
		break;
	}
}

// IRP派遣函数——创建
NTSTATUS DispatchCreate(PDRIVER_OBJECT pDriverObject, PIRP pIrp)
{
	// 设置IRP处理已经成功了
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	// 返回多少字节的数据
	pIrp->IoStatus.Information = 0;
	// 结束IRP处理流程
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	// 函数调用成功
	return STATUS_SUCCESS;
}

// IRP派遣函数——默认处理
NTSTATUS DispatchDefault(PDRIVER_OBJECT pDriverObject, PIRP pIrp)
{
	// 设置IRP处理已经成功了
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	// 返回多少字节的数据
	pIrp->IoStatus.Information = 0;
	// 结束IRP处理流程
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	// 函数调用成功
	return STATUS_SUCCESS;
}

// IRP派遣函数——读取
NTSTATUS DispatchRead(PDRIVER_OBJECT pDriverObject, PIRP pIrp)
{
	// 缓冲区指针
	PVOID pReadBuffer = NULL;
	// 长度
	ULONG uReadLength = 0;
	// IRP栈指针
	PIO_STACK_LOCATION pStack = NULL;
	// 获取IRP缓冲区地址
	pReadBuffer = pIrp->AssociatedIrp.SystemBuffer;
	// 获取IRP栈
	pStack = IoGetCurrentIrpStackLocation(pIrp);
	// 获取长度
	uReadLength = pStack->Parameters.Read.Length;
	// 获取返回的字符串长度
	ULONG uStrLen = strlen("Read Success!");
	// 比较，取较小值
	ULONG uMin = 0;
	uMin = uReadLength > uStrLen ? uStrLen : uReadLength;
	// 拷贝缓冲区
	RtlCopyMemory(pReadBuffer, "Read Success!", uMin);
	// 设置IRP处理已经成功了
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	// 返回多少字节的数据
	pIrp->IoStatus.Information = uMin;
	// 结束IRP处理流程
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	// 函数调用成功
	return STATUS_SUCCESS;
}

// IRP派遣函数——写入
NTSTATUS DispatchWrite(PDRIVER_OBJECT pDriverObject, PIRP pIrp)
{
	// 缓冲区
	PVOID pWriteBuffer = NULL;
	// 长度
	ULONG uWriteLength = 0;
	// IRP栈指针
	PIO_STACK_LOCATION pStack = NULL;
	// 获取IRP缓冲区地址
	pWriteBuffer = pIrp->AssociatedIrp.SystemBuffer;
	// 获取IRP栈
	pStack = IoGetCurrentIrpStackLocation(pIrp);
	// 获取长度
	uWriteLength = pStack->Parameters.Read.Length;
	// 获取返回的字符串长度
	ULONG uStrLen = strlen("Write Success!");
	// 打印
	DbgPrint("%s", pWriteBuffer);
	// 设置IRP处理已经成功了
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	// 返回多少字节的数据
	pIrp->IoStatus.Information = uWriteLength;
	// 结束IRP处理流程
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	// 函数调用成功
	return STATUS_SUCCESS;
}

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	DbgPrint("Unload Success!");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegPath)
{
	// 设备名称
	UNICODE_STRING uDeviceName = { 0 };
	// 符号链接名称
	UNICODE_STRING uLinkName = { 0 };
	// 状态
	NTSTATUS ntStatus = STATUS_SUCCESS;
	// 设备对象
	PDRIVER_OBJECT pDeviceObject = NULL;
	// 初始化字符串——设备名称
	RtlInitUnicodeString(&uDeviceName, DEVICE_NAME);
	// 初始化字符串——符号链接
	RtlInitUnicodeString(&uLinkName, LINK_NAME);
	// 创建设备对象
	ntStatus = IoCreateDevice(pDriverObject, 0, &uDeviceName, FILE_DEVICE_UNKNOWN, 0, TRUE, &pDriverObject);
	if (!NT_SUCCESS(ntStatus))
	{
		IoDeleteDevice(pDriverObject);
		DbgPrint("IoCreateDevice Error!");
		return ntStatus;
	}

	pDeviceObject->Flags = DO_BUFFERED_IO; // 通讯方式 基于缓冲的IO方式

	// 创建符号链接
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
		// 给所有消息设置默认处理函数
		pDeviceObject->MajorFunction[i] = DispatchDefault;
	}
	pDeviceObject->MajorFunction[IRP_MJ_CREATE] = DispatchCreate; // 设置Create消息处理函数
	pDeviceObject->MajorFunction[IRP_MJ_READ] = DispatchRead;
	pDeviceObject->MajorFunction[IRP_MJ_WRITE] = DispatchWrite;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoCtrl;

	pDriverObject->DriverUnload = DriverUnload; // 设置驱动卸载函数
	DbgPrint("Hello Driver!");
	return STATUS_SUCCESS;
}