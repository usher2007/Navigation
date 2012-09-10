#include <iostream>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <Windows.h>

using namespace std;

const int Port = 4000;
const char *const IpAddr = "192.168.0.213";

int main(int argc, char *argv[])
{
	WSADATA Ws;
	SOCKET ClientSocket;
	struct sockaddr_in ServerAddr;
	int Ret = 0;
	int AddrLen = 0;
	HANDLE hThread = NULL;
	char SendBuffer[MAX_PATH];

	if(WSAStartup(MAKEWORD(2,2), &Ws) != 0)
	{
		cout<<"Init Windows Socket Failed!"<<GetLastError()<<endl;
		return -1;
	}

	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(ClientSocket == INVALID_SOCKET)
	{
		cout<<"Create Socket Failed!"<<GetLastError()<<endl;
		return -1;
	}

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr(IpAddr);
	ServerAddr.sin_port = htons(Port);
	memset(ServerAddr.sin_zero, 0x00, 8);

	Ret = connect(ClientSocket, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr));
	if(Ret == SOCKET_ERROR)
	{
		cout<<"Connect Error"<<GetLastError()<<endl;
		return -1;
	}
	
	cout<<"Connect Success!"<<endl;

	while(true)
	{
		cin.getline(SendBuffer, sizeof(SendBuffer));
		Ret = send(ClientSocket, SendBuffer, (int)strlen(SendBuffer), 0);
		if(Ret == SOCKET_ERROR)
		{
			cout<<"Send Info Error"<<GetLastError()<<endl;
			break;
		}
	}

	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}