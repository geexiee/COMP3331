//function used to find the distance to the nearest petrol station with fuel to sell
#include <stdio.h>

#include "trader_bot.h"
#include "bot_functions.h"

int nearest_fuel(struct bot *b) {
	int positive_distance = 0, negative_distance = 0, pos_neg_distance;
	struct location *starting_location = b -> location, *bots_location = b -> location;
//check if current location is a petrol station with fuel to sell
	if (starting_location -> type == LOCATION_PETROL_STATION && starting_location->quantity != 0) {
		return 0;
	}
//else, iterate through the world until it finds a petrol station which has fuel to sell
	while ((bots_location -> type != LOCATION_PETROL_STATION) || (bots_location->type == LOCATION_PETROL_STATION && bots_location->quantity == 0)) {
		positive_distance++;
		bots_location = bots_location -> next;
	}

	bots_location = starting_location;
//Same iteration as above but in the opposite direction
	while ((bots_location -> type != LOCATION_PETROL_STATION) || (bots_location->type == LOCATION_PETROL_STATION && bots_location->quantity == 0)) {
		negative_distance--;
		bots_location = bots_location -> previous;
	}
//Find which distance (positive or negative) has the smaller absolute value and return that number as it represents the distance to the nearest petrol station
	pos_neg_distance = negative_distance * (-1);
	if (pos_neg_distance < positive_distance) {
		return negative_distance;
	}
	return positive_distance;
}
