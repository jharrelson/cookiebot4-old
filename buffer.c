#include "buffer.h"

buffer_t *buffer_create()
{
   buffer_create_size(256);
}

buffer_t *buffer_create_size(size_t size)
{
	if (size == 0)
		size = 256;

	buffer_t *new_buffer = malloc(sizeof(buffer_t));

	if (new_buffer == NULL)
		return NULL;

	new_buffer->position = 0;
	new_buffer->length = 0;
	new_buffer->size = size;
	new_buffer->data = malloc(sizeof(char) * size);

	if (new_buffer->data == NULL)
	{
		free(new_buffer);
		return NULL;
	}

	memset(new_buffer->data, 0, size);

	return new_buffer;
}

void buffer_destroy(buffer_t *buffer)
{
   free(buffer);
}

int buffer_resize(buffer_t *buffer, size_t size)
{
	char *new_data = malloc(sizeof(char) * size);
	
	if (new_data == NULL)
		return 1;

	buffer->size = size;

	memset(new_data, 0, size);
	memcpy(new_data, buffer->data, buffer->length);

	free(buffer->data);
	buffer->data = new_data;

	return 0;
}

void buffer_dump(buffer_t *buffer, FILE *file)
{
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
            fprintf(file, "%02x ", buffer->data[i + j]);
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
               fprintf(file, ".", buffer->data[i + j]);
            else
         		fprintf(file, "%hhc", buffer->data[i + j]);
         }
      }

      fprintf(file, "\n");
   }
}

void buffer_write_data(buffer_t *buffer, void *data, size_t length)
{
	if ((buffer->position + length) > buffer->size)
	{
		size_t new_size = buffer->position + length;

		buffer_resize(buffer, new_size);
	}

   memcpy(buffer->data + buffer->position, data, length);
   buffer->position += length;
   buffer->length += length;
}

void buffer_write_byte(buffer_t *buffer, unsigned char data)
{
	buffer_write_data(buffer, &data, 1);
}

void buffer_write_word(buffer_t *buffer, short data)
{
	buffer_write_data(buffer, &data, 2);
}

void buffer_write_dword(buffer_t *buffer, int data)
{
	buffer_write_data(buffer, &data, 4);
}

void buffer_write_string(buffer_t *buffer, char *data)
{
	size_t len = strlen(data) + 1;
 
	buffer_write_data(buffer, data, len);
}
