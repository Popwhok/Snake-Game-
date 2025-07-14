
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <deque>
#pragma warning(disable: 4996)

using namespace std;
SOCKET Connections[2];

int Counter2 = 0;
char Counter;

void ClientHandler(int index) {
	char msg;

	while (true) {
		recv(Connections[index], &msg, sizeof(msg), NULL);
		for (int i = 0; i < Counter2; i++) {
			if (i == index) {
				continue;

			}
			send(Connections[i], &msg, sizeof(msg), NULL);
			
		}
	}
}

int main(int argc, char* argv[]) {

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	for (int i = 0; i < 2; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

		if (newConnection == 0) {
			std::cout << "Error #2\n";
		}
		else {
			std::cout << "Client Connected!\n";
			char msgF = 'f';
			char msgS = 's';

			Connections[i] = newConnection;
			if (i == 0)
			{
				Counter = 'f';
				Counter2++;
				send(newConnection, &msgF, sizeof(msgF), NULL);
			}
			else
			{
				Counter = 's';
				Counter2++;
				send(newConnection, &msgS, sizeof(msgS), NULL);
			}
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
		}
	}


	system("pause");
	return 0;
}


