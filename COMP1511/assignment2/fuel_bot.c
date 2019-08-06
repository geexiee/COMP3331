#include <stdio.h>
#include "trader_bot.h"
#include <stdlib.h>
char *get_bot_name(void);
void get_action(struct bot *b, int *action, int *n);

char *get_bot_name(void) {
    return "Fuel Bot";
}

void get_action(struct bot *b, int *action, int *n) {
	int petrol_distance = nearest_fuel(b);
	if (petrol_distance == 0) {
		if ( (b->fuel) < (b -> fuel_tank_capacity) ) {
			*action = ACTION_BUY;
			*n = (b -> fuel_tank_capacity) - (b -> fuel);
			return;
		}
		if ( (b -> fuel) == (b -> fuel_tank_capacity) ) {
			*action = ACTION_MOVE;
			*n = b -> maximum_move;
			return;
		}
	}

	else if (petrol_distance != 0) {
		if (petrol_distance > 0 && petrol_distance < b->maximum_move) {
			*action = ACTION_MOVE;
			*n = petrol_distance;
			return;
		}
		else if (petrol_distance > 0 && petrol_distance >= b->maximum_move) {
			*action = ACTION_MOVE;
			*n = b->maximum_move;
			return;
		}
		else if (petrol_distance < 0 && (petrol_distance * -1) < b->maximum_move) {
			*action = ACTION_MOVE;
			*n = petrol_distance;
			return;
		}
		else if (petrol_distance < 0 && (petrol_distance * -1) >= b->maximum_move) {
			*action = ACTION_MOVE;
			*n = (b->maximum_move) * -1;
			return;
		}
	}
	return;
}
