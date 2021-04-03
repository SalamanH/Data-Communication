// client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")
#include <string>
#include <thread>
#include <iostream>
#include <iomanip>

SOCKET ClientSocket;
struct sockaddr_in SvrAddr;

char RxBuffer[128] = {};

std::string TxBuffer;
std::string transportMethod;
int addr_len;

using namespace std;


void sendMsg()
{
	while (true)
	{
		std::getline(std::cin, TxBuffer);
		
		if (transportMethod == "TCP")
		{
			//tcp segment
			send(ClientSocket, TxBuffer.c_str(), TxBuffer.length(), 0);
		}
		else if (transportMethod == "UDP")
		{
			//UDP
			sendto(ClientSocket, TxBuffer.c_str(), TxBuffer.length(), 0,
				(struct sockaddr*)&SvrAddr, sizeof(SvrAddr));
		}

		if (TxBuffer == "[q]")
			break;
	}


}

void receiveMsg()
{
	while (true)
	{
		if (transportMethod == "TCP")
		{
			//tcp segment
			memset(RxBuffer, 0, sizeof(RxBuffer));
		recv(ClientSocket, RxBuffer, sizeof(RxBuffer), 0);
		}
		else if (transportMethod == "UDP")
		{
			//UDP
			recvfrom(ClientSocket, RxBuffer, sizeof(RxBuffer), 0,
				(struct sockaddr*)&SvrAddr, &addr_len);
		}

		
		std::cout << std::right << std::setw(13) << "Rx: " << RxBuffer << std::endl;

		if (TxBuffer == "[q]")
			break;
	}



}


int main()
{
	//std::string ip_addr;
	char ip_addr[40] = {};
	int port_Num;

	cout << "Enter Ip Address: ";
	cin >> ip_addr;

	cout << endl << "Enter Port Numeer: ";
	cin >> port_Num;

	cout << endl << "enter Transport Method IN CAPS (TCP / UDP): ";
	cin >> transportMethod;

	cout << endl;

	
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << "Could not start DLLs" << std::endl;
		return 0;
	}

	//SOCKET ClientSocket;
	if (transportMethod == "TCP")
	{
		//tcp segment
		ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	else if (transportMethod == "UDP")
	{
		//UDP
		ClientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}
	
	if (ClientSocket == INVALID_SOCKET) {
		std::cout << "Could not create socket" << std::endl;
		WSACleanup();
		return 0;
	}

	
	//both tcp udp
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_port = htons(port_Num);
	SvrAddr.sin_addr.s_addr = inet_addr(ip_addr);
	//SvrAddr.sin_addr.s_addr = inet_addr("159.203.26.94");

	//tcp segment
	if (transportMethod == "TCP")
	{
		if ((connect(ClientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR) {
				std::cout << "Failed to connect to server" << std::endl;
				closesocket(ClientSocket);
				WSACleanup();
				return 0;
			}
	}
	addr_len = sizeof(SvrAddr);
	
	std::cout << "Conversation has started! Start typing" << std::endl;
	
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
