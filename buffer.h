#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct buffer_t
{
	size_t position;
	size_t size;
	size_t length;
	
	unsigned char *data;

} buffer_t;

buffer_t *buffer_create();
buffer_t *buffer_create_size(size_t size);
void buffer_destroy(buffer_t *buffer);

int buffer_resize(buffer_t *buffer, size_t size);

void buffer_dump(buffer_t *buffer, FILE *file);

void buffer_write_data(buffer_t *buffer, void *data, size_t length);
void buffer_write_byte(buffer_t *buffer, unsigned char data);
void buffer_write_word(buffer_t *buffer, short data);
void buffer_write_dword(buffer_t *buffer, int data);
void buffer_write_string(buffer_t *buffer, char *data);

