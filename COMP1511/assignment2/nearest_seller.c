//This function is intended to find the distance to the nearest seller of any commodity which has a positive quantity to sell
#include "trader_bot.h"
#include "bot_functions.h"

#include <stdio.h>

int nearest_seller(struct bot *b) {
	int positive_distance = 0, negative_distance = 0, pos_neg_distance;
	struct location *starting_location = b -> location, *bots_location = b -> location;
//check if current location is a seller with a positive quantity to sell
	if (starting_location -> type == LOCATION_SELLER && starting_location->quantity != 0) {
		return 0;
	}
//iterate through the world in the positive direction until a seller with goods to sell is found
	while ( (bots_location -> type != LOCATION_SELLER) || (bots_location->type == LOCATION_SELLER && bots_location->quantity == 0)) {
		positive_distance++;
		bots_location = bots_location -> next;
	}

	bots_location = b->location;
//Same as above, but in the negative direction
	while ( (bots_location -> type != LOCATION_SELLER) || (bots_location->type == LOCATION_SELLER && bots_location->quantity == 0) ) {
		negative_distance--;
		bots_location = bots_location -> previous;
	}
//compare absolute values of positive and negative direction to find the nearest seller
	pos_neg_distance = negative_distance * (-1);
	if (pos_neg_distance < positive_distance) {
		return negative_distance;
	}
	return positive_distance;
}
