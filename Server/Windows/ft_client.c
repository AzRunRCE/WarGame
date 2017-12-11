#include <stdio.h>
#include "include/client.h"

int check_if_client_exists(Client *clients, SOCKADDR_IN *csin, int playerCount)
{
	int i = 0;
	for (i = 0; i < playerCount; i++)
	{
		if (clients[i].sin.sin_addr.s_addr == csin->sin_addr.s_addr
			&& clients[i].sin.sin_port == csin->sin_port)
		{
			return 1;
		}
	}

	return 0;
}

Client* get_client(Client *clients, SOCKADDR_IN *csin, int playerCount)
{
	int i = 0;
	for (i = 0; i < playerCount; i++)
	{
		if (clients[i].sin.sin_addr.s_addr == csin->sin_addr.s_addr
			&& clients[i].sin.sin_port == csin->sin_port)
		{
			return &clients[i];
		}
	}

	return NULL;
}

int get_client_pos(Client *clients, SOCKADDR_IN *csin, int actual)
{
	for (int i = 0; i < actual; i++)
	{
		if (clients[i].sin.sin_addr.s_addr == csin->sin_addr.s_addr
			&& clients[i].sin.sin_port == csin->sin_port)
		{
			return i;
		}
		else
			return -1;
	}

	return 0;
}
void array_remove(Client* arr, size_t size, size_t index, size_t rem_size)
{
	int* begin = arr + index;                        // beginning of segment to remove
	int* end = arr + index + rem_size;               // end of segment to remove
	size_t trail_size = size - index - rem_size;       // size of the trailing items after segment

	memcpy(begin,                                  // move data to beginning
		end,                                    // from end of segment
		trail_size * sizeof(Client));

	memset(begin + trail_size,                       // from the new end of the array
		0,                                      // set everything to zero
		rem_size * sizeof(Client));

}

void remove_client(Client *clients, int to_remove, int *actual)
{
	/* we remove the client in the array */
	memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove) * sizeof(Client));
	/* number client - 1 */
	(*actual)--;
}

void send_message_to_all_clients(int sock, Client *clients, Client *sender, int actual, uint8_t *buffer, int length)
{
	for (int i = 0; i < actual; i++)
	{
		if (sender != &clients[i])
		{
			write_client(sock, &clients[i].sin, buffer, length);
		}
	}

}

int read_client(SOCKET sock, SOCKADDR_IN *sin, uint8_t *buffer)
{
	int n = 0;
	size_t sinsize = sizeof *sin;

	if ((n = recvfrom(sock, buffer, MAX_BUFFER, 0, (SOCKADDR *)sin, &sinsize)) < 0)
		printf("recvfrom() error: %d, %s\n", errno, strerror(errno));

	return n;
}

int write_client(SOCKET sock, SOCKADDR_IN *sin, const uint8_t *buffer, const int length)
{
	int n = 0;
	if ((n = sendto(sock, buffer, length, 0, (SOCKADDR *)sin, sizeof *sin)) < 0)
		printf("sendto() error: %d, %s\n", errno, strerror(errno));
	return n;
}

int init_connection(void)
{
#ifdef _WIN32
	WSADATA WSAData;                    // Contains details of the 
										// Winsock implementation
										// Initialize Winsock. 
	if (WSAStartup(MAKEWORD(1, 1), &WSAData) != 0)
	{
		printf("WSAStartup failed! Error: %d\n", SOCKET_ERRNO);
		return false;
	}
#endif
	/* UDP so SOCK_DGRAM */
	SOCKET sock;
	SOCKADDR_IN sin = { 0 };
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Allocating socket failed! Error: %d\n", SOCKET_ERRNO);
		return false;
	}

	if (sock == INVALID_SOCKET)
	{
		printf("socket() error: %d, %s\n", errno, strerror(errno));
		exit(errno);
	}

	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(PORT);
	sin.sin_family = AF_INET;
	
	if (bind(sock, (SOCKADDR *)&sin, sizeof sin) == SOCKET_ERROR)
	{
		printf("bind() error: %d, %s\n", errno, strerror(errno));
		exit(errno);
	}

	return sock;
}

void end_connection(int sock)
{
	closesocket(sock);
}