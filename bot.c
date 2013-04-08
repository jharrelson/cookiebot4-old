#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "bot.h"
#include "bncs.h"
#include "socket.h"
#include "buffer.h"

// create a new bot instance and initialze our variables
//		- returns a pointer to the create instance
//		- returns null if creation fails
bot_t *bot_create()
{
	bot_t *new_bot = NULL;

	new_bot = malloc(sizeof(bot_t));

	if (new_bot == NULL)
		return NULL;

	new_bot->socket = socket_create();
   new_bot->buffer = buffer_create();

	strcpy(new_bot->server, "uswest.battle.net");

	return new_bot;
}

// connect the bot to a battle.net server!
int bot_connect(bot_t *bot)
{
	int result;

	fprintf(stdout, "connecting to %s...\n", bot->server);
	result = socket_connect(bot->socket, bot->server, "6112");	

	if (result == 0)
	{
		fprintf(stdout, "successfully connected to %s!\n", bot->socket->address);

		// send 0x01 to battle.net indicating we're a binary protocol
		fprintf(stdout, "sending protocol byte 0x01...\n");
		result = socket_send(bot->socket, "\x1", 1);
		printf("sent %d bytes\n", result);
		
		// send the first packet in the login sequence
		bncs_send_authinfo(bot);
		
		// parse the packets!
		bot_recv_loop(bot);
	}
	else
		fprintf(stdout, "failed to connect to %s!\n", bot->server);

	return result;
}

// receives packets from battle.net and prepares buffer for parsing
void bot_recv_loop(bot_t *bot)
{
	buffer_t *buffer = bot->buffer;
	size_t bytes_recv = 0;
	size_t buf_pos = 0;
	char recv_buf[4096] = {};

	short packet_length = 0;
	
	while (bot->socket->connected)
	{
		// read as much as we can from the wire and place it in buffer
		// 	incomplete packets should be added to the end of the buffer
		bytes_recv = socket_recv(bot->socket, recv_buf + buf_pos, 4096);
		packet_length = *(short *)(recv_buf + 2);
		buffer_write_data(buffer, recv_buf + buf_pos, packet_length);
		buf_pos += bytes_recv;

		// minimum packet size is 4 bytes
		while (buf_pos >= 4)
		{
			// received incomplete packet, try to read the rest of it
			if (buf_pos < packet_length)
				break;

			buffer_set_position(buffer, 0);
			bncs_handle_packet(bot);

			// remove packet from the buffer, packets can clump together
			buffer_set_position(buffer, 0);
			buffer_write_data(buffer, recv_buf + buf_pos, bytes_recv - packet_length);
			buf_pos -= bytes_recv;
		}
	}
}
