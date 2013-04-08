#ifndef BOT_H_
#define BOT_H_

#include "socket.h"
#include "buffer.h"

typedef struct bot_t
{
	char username[32];// = "cookiebot4";
	char password[32];// = "secret";
	char cdkey[32];// = "1611360661576";
	char server[32];// = "useast.battle.net";
	char home[32];// = "op {{}}";

	socket_t *socket;
	buffer_t *buffer;
} bot_t;

bot_t *bot_create();

int bot_connect(bot_t *bot);
void bot_recv_loop(bot_t *bot);
#endif
