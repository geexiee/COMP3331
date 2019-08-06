#include <stdio.h>
#include <string.h>

#include "trader_bot.h"
#include "bot_functions.h"

int nearest_deal(struct bot *b) {
	struct location *bots_location1 = b->location->next, *bots_location2 = b->location->next, *starting_location = b->location;
	int profit, max_profit = 0;
	while (bots_location1 != starting_location) {
		if (bots_location1->type == LOCATION_SELLER) {
			while (bots_location2 != starting_location) {
				if (bots_location2->type == LOCATION_BUYER && strcmp(bots_location2->commodity->name, bots_location1->commodity->name) == 0) {
					profit = bots_location1->price - bots_location2->price;
					if (profit > max_profit) {
						max_profit = profit;
					}
				}
				bots_location2 = bots_location2->next;
			}
		}
		bots_location1 = bots_location1->next;
	}
	return profit;
}			 
