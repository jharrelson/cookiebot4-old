#include <stdio.h>

#include "buffer.h"

int main()
{
	buffer_t *buff = buffer_create_size(64);

	buffer_write_byte(buff, 'a');
	buffer_write_byte(buff, 'b');

	buffer_write_word(buff, 0x1337);

	buffer_write_dword(buff, 0xdeadbeef);

	buffer_write_string(buff, "this is a string");
	buffer_write_string(buff, "this is another null terimated string");
	
	buffer_dump(buff, stdout);

	fprintf(stdout, "\n");

	int len = strlen("####### block of data ##############");
	buffer_write_data(buff, (char*)"####### block of data ##############", len);
	
	buffer_dump(buff, stdout);

	buffer_destroy(buff);
}
