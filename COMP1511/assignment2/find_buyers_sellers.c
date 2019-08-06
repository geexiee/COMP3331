#include <stdio.h>
#include "bot_functions.h"

void find_pairs();
void find_pairs() {
	while (bots_location -> type != LOCATION_BUYER) {
		bots_location = bots_location -> next;
	}
	if (bots_location -> type == LOCATION_BUYER) {
		if (b -> cargo -> commodity -> name != bots_location -> commondity -> name) {
			bots_location = bots_location -> next;
		}
		else if (b -> cargo -> commodity -> name == bots_location -> commodity -> name) {
			*action = ACTION_SELL;
			*n = b -> cargo -> commodity -> quantity;
		}
	}
