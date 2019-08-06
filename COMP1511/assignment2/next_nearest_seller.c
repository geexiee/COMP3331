//This function is intended to find the nearest seller while disregarding the current seller that the bot is located at. This function is only called when the bot needs to move away from the seller its currently at, but the nearest_seller function keeps returning the position that the bot is already at.
#include <stdio.h>

#include "trader_bot.h"
#include "bot_functions.h"

int next_nearest_seller(struct bot *b) {
	int positive_distance = 1, negative_distance = -1, pos_neg_distance;
	struct location *starting_location = b -> location, *bots_location = b -> location->next;
	if (starting_location -> type == LOCATION_SELLER) {
		positive_distance++;
		bots_location = bots_location -> next;
	}
	while (bots_location -> type != LOCATION_SELLER) {
		positive_distance++;
		bots_location = bots_location -> next;
	}

	bots_location = starting_location->previous;

	while (bots_location -> type != LOCATION_SELLER) {
		negative_distance--;
		bots_location = bots_location -> previous;
	}

	pos_neg_distance = negative_distance * (-1);
	if (pos_neg_distance < positive_distance) {
		return negative_distance;
	}
	return positive_distance;
}
