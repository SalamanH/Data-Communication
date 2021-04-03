#include <iostream>
#include <string>
#include "socket.h"

int main() {
	std::string message = "Hello";
	data_packet recovered_packet;

	Server_TCP server("127.0.0.1", 27000);

	server.create_socket();
	server.bind_socket();
	server.listen_for_connections();
	std::cout << "Waiting for a client to connect." << std::endl;
	server.accept_connection();

	server.receive_data_packet(recovered_packet);
	std::cout << "Packet name: " << recovered_packet.name << std::endl;
	std::cout << "Packet ID: " << recovered_packet.id << std::endl;
	std::cout << "Packet size: " << recovered_packet.size << std::endl;
	for (int i = 0; i < recovered_packet.size; i++) {
		std::cout << "Packet Numbers: " << i << " " << recovered_packet.numbers[i] << std::endl;
	}
	std::cout << "Packet Tail: " << recovered_packet.tail << std::endl;
	server.send_message("Got your message!");

	return 0;
}
