#include <string>
#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")
#include <thread>
#include <string>
#include <iostream>
#include <iomanip>

SOCKET ClientSocket;

char RxBuffer[128] = {};

std::string TxBuffer;

void sendMsg()
{
	while (true)
	{
		std::getline(std::cin, TxBuffer);
		send(ClientSocket, TxBuffer.c_str(), TxBuffer.length(), 0);
		if (TxBuffer == "[q]")
			break;
	}

	
}

void receiveMsg()
{
	while (true)
	{
		if (std::string(RxBuffer).length() > 0)
			{
				memset(RxBuffer, 0, sizeof(RxBuffer));
				recv(ClientSocket, RxBuffer, sizeof(RxBuffer), 0);
				std::cout << std::right << std::setw(13)  << "Rx: " << RxBuffer << std::endl;
			}
		if (TxBuffer == "[q]")
			break;
	}
	
	

}

int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "Could not start DLLs" << std::endl;
		return 0;
	}

	
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ClientSocket == INVALID_SOCKET) {
		std::cout << "Could not create socket" << std::endl;
		WSACleanup();
		return 0;
	}

	struct sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_port = htons(27000);
	SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//SvrAddr.sin_addr.s_addr = inet_addr("159.203.26.94");
	if ((connect(ClientSocket, (struct sockaddr*)&SvrAddr,
		sizeof(SvrAddr))) == SOCKET_ERROR) {
		std::cout << "Failed to connect to server" << std::endl;
		closesocket(ClientSocket);
		WSACleanup();
		return 0;
	}

	
	recv(ClientSocket, RxBuffer, sizeof(RxBuffer), 0);
	if (!strcmp(RxBuffer, "Full")) {
		std::cout << "Server full" << std::endl;
		return 0;
	}
	//thread else
	else {

		std::cout << "Conversation has started!([q]):" << std::endl;
		
		std::thread(receiveMsg).detach();
		std::thread(sendMsg).join();
		while (true) {
			
			
			
			if (TxBuffer == "[q]")
			{
				closesocket(ClientSocket);
				WSACleanup();
				break;
			}
				
		}


	}
	

	
	

	

	return 0;
}
