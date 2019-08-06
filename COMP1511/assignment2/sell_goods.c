#include "bot_functions.h"
#include <stdio.h>
#include "trader_bot.h"
#include <math.h>
void get_action(struct bot *b, int *action, int *n);
char *get_bot_name(void);

char *get_bot_name(void) {
	return "Buyer Bot";
}

void get_action(struct bot *b, int *action, int *n) {
	int buyer_distance = nearest_buyer(b);
	struct location *starting_location = b -> location, *bots_location = b -> location;
	if (buyer_distance == 0) {
		if ( (b->cargo->commodity->name) == (bots_location -> commodity->name) ) {
			*action = ACTION_SELL;
			*n = b->cargo->commodity->volume;
			return;
		}
		*action = ACTION_MOVE;
		*n = next_nearest_buyer(b);
		return;
	}

	else if (buyer_distance != 0) {
		if (buyer_distance > 0 && buyer_distance <= b -> fuel) {
			*action = ACTION_MOVE;
			*n = buyer_distance;
			return;
		}
//		else if (petrol_distance > 0 && petrol_distance >= b->maximum_move) {
//			*action = ACTION_MOVE;
//			*n = b->maximum_move;
//			return;
//		}
		else if (buyer_distance < 0 && (buyer_distance * -1) <= b->fuel) {
			*action = ACTION_MOVE;
			*n = buyer_distance;
			return;
		}
//		else if (seller_distance < 0 && (seller_distance * -1) > b->fuel) {
//			*action = ACTION_MOVE;
//			*n = (b->maximum_move) * -1;
//			return;
//		}
	}
	return;
}
