#include <stdio.h>
#include "trader_bot.h"

int nearest_fuel(struct bot *b); 
int nearest_fuel(struct bot *b) {
	int positive_distance = 0, negative_distance = 0, pos_neg_distance;
	struct location *starting_location = b -> location, *bots_location = b -> location;
	if (starting_location -> type == LOCATION_PETROL_STATION) {
		return 0;
	}
	while (bots_location -> type != LOCATION_PETROL_STATION) {
		positive_distance++;
		bots_location = bots_location -> next;
	}

	bots_location = starting_location;

	while (bots_location -> type != LOCATION_PETROL_STATION) {
		negative_distance--;
		bots_location = bots_location -> previous;
	}

	pos_neg_distance = negative_distance * (-1);
	if (pos_neg_distance < positive_distance) {
		return negative_distance;
	}
	return positive_distance;
}
		

