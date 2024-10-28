#ifndef TCPLISTENER_H
#define TCPLISTENER_H

#include <cstdint>
#include <string>


template<typename T>
class PacketCodec {
public:
	virtual T* decode(unsigned char* data) = 0;
	virtual unsigned char* encode(const T* data) = 0;
};

enum class PacketType : unsigned char {
	ERROR=0,
	HELLO,
	LOG,
	RUN_CODE,
	CONTROL_DIFF
};

const int MESSAGE_SIZE = 4096 - (2 + 8 + 1 + 1 + (2 * 6) + 4); 

struct RudyPacket {
	uint16_t version;               // 2 bytes
	int64_t date_sent;             // 8 bytes
	bool has_next_part;            // 1 byte
	unsigned char packet_type;     // 1 byte
  int16_t axis_1;                // 2 bytes
  int16_t axis_2;                // 2 bytes
  int16_t axis_3;                // 2 bytes
  int16_t axis_4;                // 2 bytes
  int16_t axis_5;                // 2 bytes
  int16_t axis_6;                // 2 bytes
  int32_t button_status;         // 4 bytes
  char message[MESSAGE_SIZE]; // Remaining space
	
	void printMessage();
};

class RudyPacketCodec : PacketCodec<RudyPacket> {
	public:
	RudyPacket* decode(unsigned char* data) override;
	unsigned char* encode(const RudyPacket* data) override;
};


class TCPListener
{
public:
    // Constructor to initialize the listener with IP and port
    TCPListener(const std::string& ipAddress, int port);

    // Destructor to clean up the socket
    ~TCPListener();

    // Initialize the listener (create socket, bind, and listen)
    bool Init();

    // Wait for a client to connect
    int WaitForConnection();

    // Send data to a connected client
    void Send(int clientSocket, const unsigned char* data);

    // Receive data from a connected client
    unsigned char* Receive(int clientSocket);

private:
    std::string m_ipAddress;  // IP address to listen on
    int m_port;               // Port to listen on
    int m_socket;             // Listener socket
};

#endif // TCPLISTENER_H

