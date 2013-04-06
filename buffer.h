
#ifndef BUFFER_H_
#define BUFFER_H_

typedef struct buffer_t
{
	// current position/offset within buffer
	size_t position;
	// total allocated memory for the buffer
	size_t size;
	// current length of the buffer
	size_t length;
	// the buffer that holds data
	char *data;

} buffer_t;

// create buffer
buffer_t *buffer_create();
buffer_t *buffer_create_size(size_t size);

// destroy buffer and free memory
void buffer_destroy(buffer_t *buffer);

// resize buffer to a new size
int buffer_resize(buffer_t *buffer, size_t size);

// write a hex dump of buffer to file
void buffer_dump(buffer_t *buffer, FILE *file);

// add data to buffer based on current position
void buffer_write_data(buffer_t *buffer, void *data, size_t length);
void buffer_write_byte(buffer_t *buffer, char data);
void buffer_write_word(buffer_t *buffer, short data);
void buffer_write_dword(buffer_t *buffer, int data);
void buffer_write_string(buffer_t *buffer, char *data);

#endif
