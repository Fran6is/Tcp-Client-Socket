#include <iostream>
#include <string>
#include <WS2tcpip.h>


#pragma comment (lib, "ws2_32.lib")

#define BUF_SIZE 4000

int main(void)
{
	// Server's IP and port number
	char IP[] = "172.217.164.206";
	unsigned int port = 80;

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

	//Send an http GET request to google
	char request[] = "GET / HTTP/1.1\r\n\r\n";
	char response[NI_MAXHOST]; ZeroMemory(response, NI_MAXHOST);

	send(client, request, strlen(request) + 1, 0);
	recv(client, response, NI_MAXHOST, 0);

	std::cout << response << std::endl;
	
	//close your socket
	closesocket(client);

	//free winsock's allocated resources
	WSACleanup();

	std::system("pause");
	return 0;
}