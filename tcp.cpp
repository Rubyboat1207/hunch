#include "TCPListener.h"
#include <iostream>
#include <unistd.h>


int main() {
	TCPListener listener("0.0.0.0", 5400);

	if(listener.Init()) {
		std::cout << "Listener is sigma" << std::endl;

		int clientSocket = listener.WaitForConnection();

		if(clientSocket != -1) {
			RudyPacket* packet = RudyPacketCodec().decode(listener.Receive(clientSocket));

			std::cout << "Nuts: " << packet->message << std::endl;
		}

		close(clientSocket);
	}

	return 0;

}
