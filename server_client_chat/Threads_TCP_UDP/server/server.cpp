// server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")
#include <string>
#include <thread>
#include <iostream>
#include <iomanip>

std::string TxBuffer;
SOCKET ClientSocket;
char RxBuffer[128] = {};


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
		
		//recv(ClientSocket, RxBuffer, sizeof(RxBuffer), 0);
		//std::cout << "Msg Rx: " << RxBuffer << std::endl;

		
			memset(RxBuffer, 0, sizeof(RxBuffer));
			recv(ClientSocket, RxBuffer, sizeof(RxBuffer), 0);
			std::cout << std::right << std::setw(13) << "Rx: " << RxBuffer << std::endl;
			//std::cout << "Rx: " << RxBuffer << std::endl;
		
		if (TxBuffer == "[q]")
			break;
	}
	
}

int main() {

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "Could not start DLLs" << std::endl;
		return 0;
	}

	SOCKET ListenSocket;
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		std::cout << "Could not create socket" << std::endl;
		WSACleanup();
		return 0;
	}

	struct sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	SvrAddr.sin_port = htons(27000);
	if (bind(ListenSocket, (struct sockaddr*)&SvrAddr,
		sizeof(SvrAddr)) == SOCKET_ERROR) {
		std::cout << "Could not bind socket to address" << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		return 0;
	}

	if (listen(ListenSocket, 1) == SOCKET_ERROR) {
		std::cout << "Could not start to listen" << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		return 0;
	}

	std::cout << "Waiting for client connection" << std::endl;


	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		std::cout << "Failed to accept connection" << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		return 0;
	}

	closesocket(ListenSocket);
	std::cout << "Connection Established" << std::endl;



	//char TxBuffer[128] = "Thanks for your message!";
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

	
	

	return 0;
}
