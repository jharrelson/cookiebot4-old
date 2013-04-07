#include <stdio.h>
#include <string.h>

#include "buffer.h"
#include "socket.h"

int main()
{
	buffer_t *buff = buffer_create_size(64);

	buffer_write_byte(buff, 'a');
	printf("wrote byte : a\n");
	buffer_write_byte(buff, 'b');
	printf("wrote byte : b\n");

	buffer_write_word(buff, 0x1337);
	printf("wrote word : 0x1337\n");

	buffer_write_dword(buff, 0xdeadbeef);
	printf("wrote dword : 0xdeadbeef\n");

	buffer_write_string(buff, (char*) "this is a string");
	printf("wrote string : this is a string\n");
	buffer_write_string(buff, (char*)"this is another null terimated string");
	printf("wrote string : this is another null terminated string\n");
	
	buffer_dump(buff, stdout);

	fprintf(stdout, "\n");

	size_t len = strlen("####### block of data ##############");
	buffer_write_data(buff, (char*)"####### block of data ##############", len);
	
	buffer_dump(buff, stdout);

	buffer_set_position(buff, 0);

	char bytes[2];
	buffer_peek_byte(buff, &bytes[0]);
	printf("peek byte : %c\n", bytes[0]);
	buffer_read_byte(buff, &bytes[0]);
	printf("read byte : %c\n", bytes[0]);
	
	buffer_peek_byte(buff, &bytes[1]);
	printf("peek byte : %c\n", bytes[1]);
	buffer_read_byte(buff, &bytes[1]);
	printf("read byte : %c\n", bytes[1]);

	short word;
	buffer_peek_word(buff, &word);
	printf("peek word : %x\n", word);
	buffer_read_word(buff, &word);
	printf("read word : %x\n", word);

	int dword;
	buffer_peek_dword(buff, &dword);
	printf("peek dword : %x\n", dword);
	buffer_read_dword(buff, &dword);
	printf("read dword : %x\n", dword);

	char string[20] = {};
	buffer_peek_string(buff, string, 20);
	printf("peek string : %s\n", string);
	buffer_read_string(buff, string, 20);
	printf("read string : %s\n", string);

	buffer_peek_string(buff, string, 20);
	printf("peek string : %s\n", string); 
	buffer_read_string(buff, string, 20);
	printf("read string : %s\n", string);

	buffer_destroy(buff);

	printf("------------------\n");

	socket_t *socket = socket_create();
	if (socket_connect(socket, "useast.battle.net", "6112") == 0)
		printf("connected!\n");
	printf("%s\n", socket_get_address(socket));

	return 0;
}
