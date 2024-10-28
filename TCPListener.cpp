#include "TCPListener.h"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <netdb.h>

TCPListener::TCPListener(const std::string& ipAddress, int port)
    : m_ipAddress(ipAddress), m_port(port), m_socket(-1) {}

TCPListener::~TCPListener() {
    if (m_socket != -1) {
        close(m_socket);
    }
}

RudyPacket* RudyPacketCodec::decode(unsigned char* data) {
	RudyPacket* packet = new RudyPacket();
	
	std::memcpy(&packet->version, data, 2);
	std::memcpy(&packet->date_sent, data + 2, 8);
	std::memcpy(&packet->axis_1, data + 10, 2);
	std::memcpy(&packet->axis_2, data + 12, 2);
	std::memcpy(&packet->axis_3, data + 14, 2);
	std::memcpy(&packet->axis_4, data + 16, 2);
	std::memcpy(&packet->axis_5, data + 18, 2);
	std::memcpy(&packet->axis_6, data + 20, 2);
	std::memcpy(&packet->button_status, data + 22, 4);
	std::memcpy(&packet->message, data + 26, MESSAGE_SIZE);

	return packet;
}

unsigned char* RudyPacketCodec::encode(const RudyPacket* packet) {
  // Allocate 4096 bytes for the raw packet
  unsigned char* data = new unsigned char[4096];

  // Copy each field from the RudyPacket to the byte array

  // Copy version (2 bytes)
  std::memcpy(data, &packet->version, 2);

  // Copy date_sent (8 bytes)
  std::memcpy(data + 2, &packet->date_sent, 8);

  // Copy axis_1 to axis_6 (2 bytes each)
  std::memcpy(data + 10, &packet->axis_1, 2);
  std::memcpy(data + 12, &packet->axis_2, 2);
  std::memcpy(data + 14, &packet->axis_3, 2);
  std::memcpy(data + 16, &packet->axis_4, 2);
  std::memcpy(data + 18, &packet->axis_5, 2);
  std::memcpy(data + 20, &packet->axis_6, 2);

  // Copy button_status (4 bytes)
  std::memcpy(data + 22, &packet->button_status, 4);

  // Copy message (remaining bytes)
  std::memcpy(data + 26, &packet->message, MESSAGE_SIZE);

  return data;
}



bool TCPListener::Init() {
    // Create a socket
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == -1) {
        std::cerr << "Can't create socket!" << std::endl;
        return false;
    }

    // Bind the socket to an IP / port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(m_port);
    inet_pton(AF_INET, m_ipAddress.c_str(), &hint.sin_addr);

    if (bind(m_socket, (sockaddr*)&hint, sizeof(hint)) == -1) {
        std::cerr << "Can't bind to IP/port!" << std::endl;
        return false;
    }

    // Mark the socket for listening
    if (listen(m_socket, SOMAXCONN) == -1) {
        std::cerr << "Can't listen!" << std::endl;
        return false;
    }

    return true;
}

int TCPListener::WaitForConnection() {
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    int clientSocket = accept(m_socket, (sockaddr*)&client, &clientSize);

    if (clientSocket == -1) {
        std::cerr << "Problem with client connecting!" << std::endl;
        return -1;
    }

    char host[NI_MAXHOST];      // Client's remote name
    char service[NI_MAXSERV];   // Service (port) the client is connected on

    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    // Get the name info of the client
    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        std::cout << host << " connected on port " << service << std::endl;
    } else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
    }

    return clientSocket;
}

void TCPListener::Send(int clientSocket, const unsigned char* data) {
    send(clientSocket, data, 4096 + 1, 0);
}

unsigned char* TCPListener::Receive(int clientSocket) {
    unsigned char* buffer = new unsigned char[4096];
    memset(buffer, 0, 4096);

    int bytesReceived = recv(clientSocket, buffer, 4096, 0);
    if (bytesReceived == -1) {
        std::cerr << "Error in recv()" << std::endl;
        return buffer;
    }

    return buffer;
}

