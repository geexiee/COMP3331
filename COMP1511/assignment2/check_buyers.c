//This function is intended to iterate through the world and count the total aount of a good that buyers are wiling to buy. This function is only called when the bot is already on a seller, which is why it refers to the commodity of the current location of the bot as a comparison with potential buyers.
#include <stdio.h>
#include <string.h>
#include "bot_functions.h"
#include "trader_bot.h"

int check_buyers(struct bot *b) {
	int sellable_total = 0;
	struct location *current_location = b->location, *bots_location = b->location->next;
//iterate through the world, summing the quantity of buyers of the same commodity that the current location is looking to sell
	while (bots_location != current_location) {
		if (bots_location->type == LOCATION_BUYER && (strcmp(bots_location->commodity->name, current_location->commodity->name) == 0)) {
			sellable_total = sellable_total + bots_location->quantity;
		}
		bots_location = bots_location->next;
	}
	return sellable_total;
}
