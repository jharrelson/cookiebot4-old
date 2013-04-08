#ifndef SOCKET_H_
#define SOCKET_H_

#include <stdlib.h>

typedef struct socket_t
{
	// file descirpotr
	int sockfd;
	// set to 1 if connected, else 0
	int connected;
	// the address we're currently connected to
	char *address;
} socket_t;

// create a socket
socket_t *socket_create();

// connect the specified socket to a select host:port
int socket_connect(socket_t *socket, char *host, char *port);

// disconnect the socket
void socket_close(socket_t *socket);

// disconnect the socket and clean up object
void socket_destory(socket_t *socket);

// helpers
char *socket_get_address(socket_t *socket);
int socket_is_connected(socket_t *socket);

// send data over the wire
size_t socket_send(socket_t *socket, char *data, size_t data_length);

// recv data from the wire
size_t socket_recv(socket_t *socket, char *buffer, size_t buffer_length);

#endif
