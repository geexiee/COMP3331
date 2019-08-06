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
	int seller_distance = nearest_seller(b);
	struct location *starting_location = b -> location, *bots_location = b -> location;
	if (seller_distance == 0) {
		if ( (b->cash) >= (bots_location -> price) ) {
			*action = ACTION_BUY;
			*n = floor((b->cash)/(bots_location->price));
			return;
		}
		if ( (b -> cash) < (bots_location -> price) ) {
			*action = ACTION_MOVE;
			*n = next_nearest_seller(b);
			return;
		}
	}

	else if (seller_distance != 0) {
		if (seller_distance > 0 && seller_distance <= b -> fuel) {
			*action = ACTION_MOVE;
			*n = seller_distance;
			return;
		}
//		else if (petrol_distance > 0 && petrol_distance >= b->maximum_move) {
//			*action = ACTION_MOVE;
//			*n = b->maximum_move;
//			return;
//		}
		else if (seller_distance < 0 && (seller_distance * -1) <= b->fuel) {
			*action = ACTION_MOVE;
			*n = seller_distance;
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

