#include <stdio.h>

#include "bot.h"
#include "buffer.h"
#include "bncs.h"

// read the header and determines what packet we received
//	calls the corresponding handling routine for each packet
void bncs_handle_packet(bot_t *bot)
{
	buffer_t *buffer = bot->buffer;
	
	char ff;
	char packet_id;
	short packet_length;

	// packet header
	buffer_read_byte(buffer, &ff);
	buffer_read_byte(buffer, &packet_id);
	buffer_read_word(buffer, &packet_length);

	printf("received packet 0x%x\n", packet_id);
	buffer_dump(buffer, stdout);
	printf("-----\n");

	switch (packet_id)
	{
		case 0x50:
			bncs_handle_authinfo(bot);
			break;
		case 0x25:
			break;
	}

}

// send sid_auth_info (0x50)
void bncs_send_authinfo(bot_t *bot)
{
	buffer_t *buf = bot->buffer;

	buffer_clear(buf);
	// leave room for the header
	buffer_set_position(buf, 4);

	buffer_write_dword(buf, 0);
	buffer_write_data(buf, "68XI", 4);
	buffer_write_data(buf, "PXES", 4);
	buffer_write_dword(buf, 0xd9);
	buffer_write_dword(buf, 0x00);
	buffer_write_dword(buf, 0x00);
	buffer_write_dword(buf, 0x00);
	buffer_write_dword(buf, 0x00);
	buffer_write_dword(buf, 0x00);
	buffer_write_string(buf, "USA");
	buffer_write_string(buf, "United States");

	bncs_send_packet(bot, 0x50);
}

// handles the response for sid_auth_info packet (0x50)
void bncs_handle_authinfo(bot_t *bot)
{
	buffer_t *buffer = bot->buffer;

	int logon_type;
	int server_token;
	char ix86ver_filename[32] = {};
	char value_string[32] = {};

	buffer_read_dword(buffer, &logon_type);
	buffer_read_dword(buffer, &server_token);
	buffer_skip_bytes(buffer, 12);
	buffer_read_string(buffer, ix86ver_filename, 32);

	printf("logon type: ");
	switch (logon_type)
	{
		case 0x00:
			printf("broken sha-1\n");
			break;
		case 0x01:
			printf("nls version 1\n");
			break;
		case 0x02:
			printf("nls version 2\n");
			break;
		default:
			printf("unknown (0x%c)\n", logon_type);
	}

	printf("ix86ver filename: %s\n", ix86ver_filename);
}

int bncs_send_packet(bot_t *bot, char packet_id)
{
	size_t bytes_sent;
	buffer_t *buffer = bot->buffer;

	// add the bncs protocol header
	buffer_set_position(buffer, 0);
	buffer_write_byte(buffer, 0xff);
	buffer_write_byte(buffer, packet_id);
	buffer_write_word(buffer, buffer->length + 2);

	printf("sending packet 0x%x...\n", packet_id);
	buffer_dump(buffer, stdout);
	printf("-----\n");
		
	bytes_sent = socket_send(bot->socket, buffer->data, buffer->length);

	buffer_clear(buffer);

	return bytes_sent;
}
