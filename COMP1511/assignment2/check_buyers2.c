#include <stdio.h>
#include "bot_functions.h"
#include "trader_bot.h"
#include <string.h>

int check_buyers2(struct bot *b) {
	int sellable_total = 0;
	struct location *current_location = b->location, *bots_location = b->location->next;
	while (bots_location != current_location) {
		if (bots_location->type == LOCATION_BUYER && (strcmp(bots_location->commodity->name, current_location->commodity->name) == 0)) {
			sellable_total = sellable_total + bots_location->quantity;
		}
		bots_location = bots_location->next;
	}
	return sellable_total;
}
