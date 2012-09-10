#include <iostream>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <Windows.h>

using namespace std;

const int Port = 4000;
const char *const IpAddr = "192.168.0.213";

DWORD WINAPI ClientThread(LPVOID lpParameter)
{
	SOCKET ClientSocket = (SOCKET)lpParameter;
	int Ret = 0;
	char RecvBuffer[MAX_PATH];

	while (true)
	{
		memset(RecvBuffer, 0x00, sizeof(RecvBuffer));
		Ret = recv(ClientSocket, RecvBuffer, MAX_PATH, 0);
		if(Ret == 0 || Ret == SOCKET_ERROR)
		{
			cout<<"Client Quit!"<<endl;
			break;
		}
		cout<<"Client Info:"<<RecvBuffer<<endl;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	WSADATA Ws;
	SOCKET ServerSocket, ClientSocket;
	struct sockaddr_in LocalAddr, ClientAddr;
	int Ret = 0;
	int AddrLen = 0;
	HANDLE hThread = NULL;

	if(WSAStartup(MAKEWORD(2,2), &Ws) != 0)
	{
		cout<<"Init Win Socket Failed!"<<GetLastError()<<endl;
		return -1;
	}

	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(ServerSocket == INVALID_SOCKET)
	{
		cout<<"Create Socket Failed!"<<GetLastError()<<endl;
		return -1;
	}

	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_addr.s_addr = inet_addr(IpAddr);
	LocalAddr.sin_port = htons(Port);
	memset(LocalAddr.sin_zero, 0x00, 8);

	Ret = bind(ServerSocket, (struct sockaddr *)&LocalAddr, sizeof(LocalAddr));
	if(Ret != 0)
	{
		cout<<"Bind Socket Failed!"<<GetLastError()<<endl;
		return -1;
	}

	Ret = listen(ServerSocket, 10);
	if(Ret != 0)
	{
		cout<<"Listen Socket Failed!"<<GetLastError()<<endl;
		return -1;
	}

	cout<<"Server Start!"<<endl;

	while(true)
	{
		AddrLen = sizeof(ClientAddr);
		ClientSocket = accept(ServerSocket, (struct sockaddr *)&ClientAddr, &AddrLen);
		if(ClientSocket == INVALID_SOCKET)
		{
			cout<<"Accept Failed!"<<GetLastError()<<endl;
			break;
		}

		cout<<"Client Connect"<<inet_ntoa(ClientAddr.sin_addr)<<":"<<ClientAddr.sin_port<<endl;

		hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)ClientSocket, 0, NULL);
		if(hThread == NULL)
		{
			cout<<"Create Thread Failed!"<<endl;
			break;
		}
		CloseHandle(hThread);
	}
	closesocket(ServerSocket);
	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}