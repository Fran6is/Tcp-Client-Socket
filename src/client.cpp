#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>

/**********************************************************************************************
To use this version of the client socket :

1. You will need to change the below IP and port to the one used in the 'TcpConsoleChat' program
2. Must have compiled and ran the 'TcpConsoleChat' server program before running this one


**********************************************************************************************/

#pragma comment (lib, "ws2_32.lib")

#define BUF_SIZE 4000
void SendResponse(int* socket, bool* const running)
{
	std::string response{};
	//Sleep(50);
	while (*running)
	{

		//Type in response to be sent
		std::getline(std::cin, response);

		//send response your response
		if ((send(*socket, response.c_str(), response.size(), 0)) == SOCKET_ERROR)
		{
			*running = false;
		}
		std::cout << "\n";
	}
}

int main(void)
{
	// Server's IP and port number
	char IP[] = "";
	unsigned int port = 0;

	//Initialize Winsock
	WSADATA winSock;
	int ver = MAKEWORD(2, 2);
	if (WSAStartup(ver, &winSock) != 0)
	{
		printf("problem initializing winsock \n");
		return -1;
	}

	//Create a socket
	int client = socket(AF_INET, SOCK_STREAM, 0);
	if (client == INVALID_SOCKET)
	{
		printf("Error creating a socket::Error #%d \n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	//Specify an address for the socket
	struct sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, IP, &hint.sin_addr);

	//Connect to server
	if (connect(client, (struct sockaddr*) & hint, sizeof(hint)) == SOCKET_ERROR)
	{
		printf("problem connecting to server error #%d \n", WSAGetLastError());
		closesocket(client);
		WSACleanup();
		return -1;
	}

	char msgBuf[BUF_SIZE]; ZeroMemory(msgBuf, BUF_SIZE);
	bool running = true;

	//Thread to send response message
	std::thread resThread{ SendResponse, &client, &running };

	int counter = 1;
	while (running)
	{
		ZeroMemory(msgBuf, BUF_SIZE);

		int bytesRecieved = recv(client, msgBuf, BUF_SIZE, 0);
		if (bytesRecieved == 0)
		{
			std::cout << "server disconnected! quitting..\n";
			running = false;
			break;
		}
		else if (bytesRecieved == SOCKET_ERROR)
		{
			std::cout << "recv error #" << WSAGetLastError() << "\nPress enter to continue..";
			running = false;
			break;
		}

		//Display message to console
		if (counter == 1)
		{
			std::cout << msgBuf;
		}
		else
		{
			std::cout << "\n" << msgBuf << "\n";
		}
		counter++;
	}
	resThread.join();
	
	//close your socket
	closesocket(client);

	//free winsock's allocated resources
	WSACleanup();

	return 0;
}