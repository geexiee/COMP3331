//This function is intended to find the distance to the nearest buyer of whatever commodity the bot is currently carrying
#include <stdio.h>
#include <string.h>

#include "trader_bot.h"
#include "bot_functions.h"

int nearest_buyer(struct bot *b) {
	int positive_distance = 0, negative_distance = 0, pos_neg_distance;
	struct location *starting_location = b -> location, *bots_location = b -> location;
//First check if current location of bot is a buyer of the commodity its carrying
	if (starting_location -> type == LOCATION_BUYER && (strcmp(starting_location->commodity->name, b->cargo->commodity->name) == 0) && starting_location->quantity != 0) {
		return 0;
	}
//iterate through the world in the positive direction until a buyer of the commodity is found. 
	while ( (bots_location -> type != LOCATION_BUYER) || (bots_location->type == LOCATION_BUYER && strcmp(bots_location->commodity->name, b->cargo->commodity->name) != 0) || (bots_location->type == LOCATION_BUYER && bots_location->quantity == 0) ) {
		positive_distance++;
		bots_location = bots_location -> next;
	}	

	bots_location = starting_location;
//Iterate through the world in the negative direction until a buyer of the commodity is found
	while ( (bots_location -> type != LOCATION_BUYER) || (bots_location->type == LOCATION_BUYER && strcmp(bots_location->commodity->name, b->cargo->commodity->name) != 0) || (bots_location->type == LOCATION_BUYER && bots_location->quantity == 0)) {
		negative_distance--;
		bots_location = bots_location -> previous;
	}
//comparing absolute values of both distances to find which one is closer
	pos_neg_distance = negative_distance * (-1);
	if (pos_neg_distance <= positive_distance) {
		return negative_distance;
	}
	return positive_distance;
}
