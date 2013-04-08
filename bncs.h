#ifndef BNCS_H_
#define BNCS_H_

#include "bot.h"
#include "buffer.h"

// routines for sid_auth_info (0x50)
void bncs_send_authinfo(bot_t *bot);
void bncs_handle_authinfo(bot_t *bot);

// routines for sending and receiving packets
void bncs_handle_packet(bot_t *bot);
int bncs_send_packet(bot_t *bot, char packet_id);

#endif
