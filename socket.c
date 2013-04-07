#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "socket.h"

// create a tcp socket
//		- returns NULL if a socket couldn't be created
//		- returns pointer to the newly created socket object
socket_t *socket_create()
{
	socket_t *new_socket = NULL;
	
	new_socket = malloc(sizeof(socket_t));
	
	// failed to allocate memory for our socket object
	if (new_socket == NULL)
		return NULL;

	new_socket->sockfd = 0;
	new_socket->connected = 0;
	new_socket->address = NULL;

	new_socket->sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// failed to get create socket -- ABORT!
	if (new_socket->sockfd == 0)
	{
		free(new_socket);
		return NULL;
	}

	return new_socket;	
}

// attempt to connect to the specified host with corresponding port
//		- returns the ip of successful connection by ref (addr)
//		- returns 0 on successful connection
//		- returns 1 on failure to connect
//		- returns 2 if getaddrinfo() fails
int socket_connect(socket_t *socket, char *host, char *port)
{
	struct addrinfo hints;
	struct addrinfo *result;

	// zero out our hints
	memset(&hints, 0, sizeof(struct addrinfo));

	// set up our hints for getaddrinfo()
	hints.ai_family = AF_INET;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_socktype = SOCK_STREAM; 

	if (getaddrinfo(host, port, &hints, &result) == 0)
	{
		struct addrinfo *res;

		// loop through the returned addresses
		for (res = result; res != NULL; res = res->ai_next)
		{
			struct sockaddr_in *saddr = (struct sockaddr_in *)res->ai_addr;
			// attempt to connect to each address
			if (connect(socket->sockfd, res->ai_addr, res->ai_addrlen) == 0)
			{
				size_t addr_len = strlen(inet_ntoa(saddr->sin_addr));
				socket->connected = 1;

				// if address is already allocated, free it
				// the new address may require a larger size
				free(socket->address);

				// allocate size for the address we connected to and copy the addr
				socket->address = malloc(addr_len + 1);
				memset(socket->address, 0, addr_len  + 1);
				strcpy(socket->address, (char *)inet_ntoa(saddr->sin_addr));

				freeaddrinfo(result);
				return 0;
			}
		}
	} else {
		return 2;
	}

	freeaddrinfo(result);

	return 1;
}

// returns the address the socket is connected to
char *socket_get_address(socket_t *socket)
{
	// make sure we have a valid socket!
	if (socket == NULL)
		return NULL;

	return socket->address;
}

int socket_is_connected(socket_t *socket)
{
	return socket->connected;
}

// close the selected socket
void socket_close(socket_t *socket)
{
	// make sure we have a valid socket!
	if (socket == NULL)
		return;

	if (close(socket->sockfd) == 0)
		socket->connected = 0;
}

// close the socket and clean up the object
void socket_destroy(socket_t *socket)
{
	// make sure we have a valid socket!
	if (socket == NULL)
		return;

	socket_close(socket);
	free(socket->address);
	free(socket);
}

// send data over the wire
//		- returns the number of bytes sent (may be different than data_length)
size_t socket_send(socket_t *socket, char *data, size_t data_length)
{
	// make sure we have a valid socket!
	if ((socket == NULL) || (socket->connected == 0))
		return 0;

	return send(socket->sockfd, data, data_length, 0);
}

// recieve a specified number of bytes from the socket
//		- returns the number of bytes received
size_t socket_recv(socket_t *socket, char *buffer, size_t buffer_length)
{
	// make sure we have a valid socket!
	if ((socket == NULL) || (socket->connected == 0))
		return 0;

	return recv(socket->sockfd, buffer, buffer_length, 0);
}
