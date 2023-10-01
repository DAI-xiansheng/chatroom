#include<WinSock2.h>
#include<ws2tcpip.h>
#include<Windows.h>
#include<stdio.h>

#pragma comment(lib,"Ws2_32.lib")
#define BUF_SIZE 1024

char szMsg[BUF_SIZE];

unsigned SendMsg(void* arg)
{
    SOCKET sock = *((SOCKET*)arg);
    while (1)
    {
        scanf_s("%s", szMsg);
        if (!strcmp(szMsg, "QUIT\n") || !strcmp(szMsg, "quit\n"))
        {
            closesocket(sock);
            exit(0);
        }
            send(sock, szMsg, strlen(szMsg), 0);

    }
    return 0;
}

unsigned RecvMsg(void* arg)
{
    SOCKET sock = *((SOCKET*)arg);

    char msg[BUF_SIZE];

    while (1)
    {
        int len = recv(sock, msg, sizeof(msg) - 1, 0);
        if (len == -1)
        {
            return -1;
        }
        msg[len] = '\0';
        printf("%s\n", msg);
    }
    return 0;
}

int main()
{
    //��ʼ��socket��������Դ https://learn.microsoft.com/zh-cn/windows/win32/api/winsock/ns-winsock-wsadata��
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {

        return -1;
    }

    if (LOBYTE(wsaData.wVersion) != 2 ||
        HIBYTE(wsaData.wVersion) != 2) {
        WSACleanup();
        return -1;
    }

    //����socket
    SOCKET hSock;
    hSock = socket(AF_INET, SOCK_STREAM, 0);

    //�󶨶˿�
    SOCKADDR_IN servAdr;
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_port = htons(9999);
    inet_pton(AF_INET, "118.89.70.152", &servAdr.sin_addr);

    //���ӷ�����
    if (connect(hSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
    {
        printf("connect error : %d", GetLastError());
        return -1;
    }
    else
    {
        printf("��ӭ����˽�������ң���������������û�����");
    }

    //ѭ������Ϣ
    HANDLE hSendHand = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SendMsg, (void*)&hSock, 0, NULL);

    //ѭ������Ϣ
    HANDLE hRecvHand = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecvMsg, (void*)&hSock, 0, NULL);

    //�ȴ��߳̽���
    WaitForSingleObject(hSendHand, INFINITE);
    WaitForSingleObject(hRecvHand, INFINITE);


    closesocket(hSock);
    WSACleanup();
}