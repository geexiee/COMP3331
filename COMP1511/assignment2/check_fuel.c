//This function is used to find the total amount of fuel that can be purchase in the world, or if there even is any fuel to be purchased
#include <stdio.h>
#include <string.h>
#include "bot_functions.h"
#include "trader_bot.h"

int check_fuel(struct bot *b) {
	int buyable_total = 0;
	struct location *current_location = b->location, *bots_location = b->location->next;

//iterates through the world, summing the quantity of all petrol stations along the way
	while (bots_location != current_location) {
		if (bots_location->type == LOCATION_PETROL_STATION) {
			buyable_total = buyable_total + bots_location->quantity;
		}
		bots_location = bots_location->next;
	}
//When it reaches the current location of the bot, it checks if it's also a petrol station and sums the quantity if it is.
	if (bots_location == current_location) {
		if (bots_location->type == LOCATION_PETROL_STATION) {
			buyable_total = buyable_total + bots_location->quantity; 
		}
	}
	return buyable_total;
}
