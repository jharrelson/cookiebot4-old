#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"

// create a buffer with default size 256 bytes
buffer_t *buffer_create()
{
   return buffer_create_size(256);
}

// create a buffer (filled with null chars) of a specified size in bytes
//		- returns pointer to buffer struct that is allocated
//		- returns null if there was a problem
buffer_t *buffer_create_size(size_t size)
{
	if (size == 0)
		size = 256;

	buffer_t *new_buffer = malloc(sizeof(buffer_t));

	// return null -- we couldn't allocate memory block for struct
	if (new_buffer == NULL)
		return NULL;

	new_buffer->position = 0;
	new_buffer->length = 0;
	new_buffer->size = size;
	new_buffer->data = malloc(sizeof(char) * size);

	// return null -- we couldn't allocate memory block for buffer
	if (new_buffer->data == NULL)
	{
		// make sure we release the allocated struct
		free(new_buffer);
		return NULL;
	}

	// zero out our buffer
	memset(new_buffer->data, 0, size);

	return new_buffer;
}

// destory our buffer by release allocated memory
void buffer_destroy(buffer_t *buffer)
{
	free(buffer->data);
   free(buffer);
}

// clear the buffer, reset length and position
void buffer_clear(buffer_t *buffer)
{
	memset(buffer->data, 0, buffer->length);
	buffer->length = 0;
	buffer->position = 0;
}

// resize buffer to a specified size
//		- return 1 if there was an issue
//		- return 0 on successful resize
int buffer_resize(buffer_t *buffer, size_t size)
{
	// make sure we have a valid buffer!
	if (buffer == NULL)
		return 1;

	char *new_data = malloc(sizeof(char) * size);
	
	// couldn't allocate memory block
	if (new_data == NULL)
		return 1;

	buffer->size = size;

	// zero out our data and copy over the old data
	memset(new_data, 0, size);
	memcpy(new_data, buffer->data, buffer->length);

	// free old buffer and point to new block
	free(buffer->data);
	buffer->data = new_data;

	return 0;
}

// set the buffer's position
void buffer_set_position(buffer_t *buffer, size_t position)
{
	// the buffer will be resized if a write is attempted
	//	when position > buffer->position
	buffer->position = position;
}

// creates a hex dump of data within our buffer
// also prints useful information about buffer (size, addresses, etc)
void buffer_dump(buffer_t *buffer, FILE *file)
{	
	// make sure we're pointing to a valid buffer!
	if (buffer == NULL)
	{
		fprintf(file, "failed to dump buffer -- buffer was null\n");
		return;
	}

   fprintf(file, "buffer dump @ %p [data @ %p] (length : %d/%d)\n",
            buffer, buffer->data, buffer->length, buffer->size);

   int i = 0;
   for (i = 0; i < buffer->length; i += 16)
   {
      fprintf(file, "%04x: ", i);

      int j = 0;
      for (j = 0; j < 16; j++)
      {
         if (j == 8)
            fprintf(file, " ");

         if ((i + j) < buffer->length)
            fprintf(file, "%02x ", (unsigned char)buffer->data[i + j]);
         else
            fprintf(file, "   ");
      }
      fprintf(file, " | ");

      for (j = 0; j < 16; j++)
      {
         if (j == 8)
            fprintf(file, " ");

         if ((i + j) < buffer->length)
         {
            if (buffer->data[i + j] <= 0x20 || buffer->data[i+j] >= 0x7f)
               fprintf(file, ".");
            else
         		fprintf(file, "%c", buffer->data[i + j]);
         }
      }

      fprintf(file, "\n");
   }
}

// add some data of a specified length to the buffer
// 	data is inserted at the current position of the buffer, 
// 	the position is then updated by length
void buffer_write_data(buffer_t *buffer, void *data, size_t length)
{
	// make sure we're pointing to a valid buffer!
	if (buffer == NULL)
		return;

	// overflow occurred -- resize our buffer to fit new data
	if ((buffer->position + length) > buffer->size)
	{
		size_t new_size = buffer->position + length;

		buffer_resize(buffer, new_size);
	}

   memcpy(buffer->data + buffer->position, data, length);
   buffer->position += length;
   buffer->length += length;
}

// write a single byte to the buffer
void buffer_write_byte(buffer_t *buffer, char data)
{
	buffer_write_data(buffer, &data, 1);
}

// write a word (2 bytes) to the buffer
void buffer_write_word(buffer_t *buffer, short data)
{
	buffer_write_data(buffer, &data, 2);
}

// write a dword (4 bytes) to the buffer
void buffer_write_dword(buffer_t *buffer, int data)
{
	buffer_write_data(buffer, &data, 4);
}

// write a string of characters to the buffer ending with a null
void buffer_write_string(buffer_t *buffer, char *data)
{
	// just add 1 to the length to account for the null 
	// 	remember: buffer is zeroed out when created
	size_t len = strlen(data) + 1;
 
	buffer_write_data(buffer, data, len);
}

// attempt to read the next number of bytes from the buffer
//		- the bytes read are passed by ref (data)
//		- returns 0 on success
//		- returns 1 on failure
int buffer_read_data(buffer_t *buffer, void *data, size_t length)
{
	// make sure the buffer is valid!
	if (buffer == NULL)
		return 1;

	// don't read past the buffer!
	if ((length + buffer->position) > buffer->length)
		return 1;

	memcpy(data, buffer->data + buffer->position, length);
	buffer->position += length;

	return 0;
}

// read a byte from the buffer
//		- read byte is returned by ref (data)
//		- returns is based on 0 for success, 1 for failure
int buffer_read_byte(buffer_t *buffer, char *data)
{
	return buffer_read_data(buffer, data, 1);
}

// read a word (2 bytes) from the buffer
//		- read word is returned by ref (data)
//		- returns is based on 0 for success, 1 for failure
int buffer_read_word(buffer_t *buffer, short *data)
{
	return buffer_read_data(buffer, data, 2);
}

// read a dword (4 bytes) from the buffer
//		- read dword is returned by ref (data)
//		- returns is based on 0 for success, 1 for failure
int buffer_read_dword(buffer_t *buffer, int *data)
{
	return buffer_read_data(buffer, data, 4);
}

// read a null-teriminated string from the buffer
//		- returns 0 on success
//		- returns 1 on failure (invalid buffer or read past buffer size)
//		- returns 2 when finding we didn't read a null before max_length
int buffer_read_string(buffer_t *buffer, char *data, size_t max_length)
{
	size_t length = 0;
	size_t position;

	// make sure the buffer is valid!
	if (buffer->data == NULL)
		return 1;

	position = buffer->position;

	// make sure we don't try to read past the buffer size
	if ((position + max_length) > buffer->length)
		return 1;

	// copy character by character until we hit max_length or a null	
	while ((buffer->data[position] != '\0') && (length < max_length))
	{
		data[length] = buffer->data[position];
		position++;
		length++;
	}

	buffer->position += length + 1;
	
	// make sure we're null-terminated
	data[length] = '\0';

	// we read as much as we could, but didn't hit a null
	if (buffer->data[position] != '\0')
		return 2;

	return 0;
}
