#include <stdio.h>
#include <windows.h>

#define DEVICE_LINK_NAME L"\\\\.\\tkzc"

// ���������ʼ��ַ
#define IRP_IOCTRL_BASE 0x8000
// ����������ɺ꣨�豸���ͣ������룬ͨ�ŷ�ʽ��Ȩ�ޣ�
#define IRP_IOCTRL_CODE(i) CTL_CODE(FILE_DEVICE_UNKNOWN, IRP_IOCTRL_BASE + i, METHOD_BUFFERED, FILE_ANY_ACCESS)
// ����ͨ�ŵĿ����붨��
#define CTL_PRINT IRP_IOCTRL_CODE(1)

int main()
{
	HANDLE hDriver = CreateFile(DEVICE_LINK_NAME, GENERIC_ALL, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDriver == INVALID_HANDLE_VALUE)
	{
		printf("ERROR CODE: %d \n", GetLastError());
		system("pause");
		return 1;
	}

	//char buffer[1024] = { 0 };
	//ULONG retLen = 0;
	//ReadFile(hDriver, buffer, 1024, &retLen, NULL);

	//printf("%s\n", buffer);

	//WriteFile(hDriver, "Write Success!", strlen("Write Success!"), &retLen, NULL);

	while (true)
	{
		system("cls");
		char InputBuffer[1024] = { 0 };
		char OutputBuffer[1024] = { 0 };
		memset(InputBuffer, 0, 1024);
		memset(OutputBuffer, 0, 1024);
		ULONG retLen = 0;
		printf("�������ַ�����");
		scanf("%s", &InputBuffer);
		DeviceIoControl(hDriver, CTL_PRINT, &InputBuffer, sizeof(InputBuffer), &OutputBuffer, sizeof(OutputBuffer), &retLen, NULL);
		printf("%s", OutputBuffer);
		system("pause");
	}

	return 0;
}