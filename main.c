#include <stdio.h>
#include <string.h>

#include "bot.h"
#include "bncs.h"
#include "buffer.h"
#include "socket.h"

int main()
{
	bot_t *bot;
	bot = bot_create();

	bot_connect(bot);

	return 0;
}
