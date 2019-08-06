//This bot is designed to find the nearest seller of any good, purchase as much as it can sell, and then find the nearest buyer of that good to sell to. It also refuels along the way if it's tank is less than 1/2 of its max capacity and there is a petrol station on its way to its destination.
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "bot_functions.h"
#include "trader_bot.h"


void get_action(struct bot *b, int *action, int *n);
char *get_bot_name(void);

char *get_bot_name(void) {
	return "WAM_MASTER_420";
}

void get_action(struct bot *b, int *action, int *n) {
	
	struct location *starting_location = b -> location, *bots_location = b -> location;

//First if statement is checking if there is any fuel left to purchase in the world
	if (check_fuel(b) != 0) {	
//If there is fuel availble to buy, this next if statement checks if the bot is carrying < 1/2 of its max tank of fuel. If it is and it's not currently on a petrol station OR its on a petrol station which has run out of petrol to sell, it'll move to the nearest petrol station which has petrol to sell.
		if ( (b->fuel <= floor(b->fuel_tank_capacity/2) && b->location->type != LOCATION_PETROL_STATION) || (b->fuel <= floor(b->fuel_tank_capacity/2) && b->location->type == LOCATION_PETROL_STATION && b->location->quantity == 0) ) {
			*action = ACTION_MOVE;
			*n = nearest_fuel(b);
			return;
		}	
//If the bot is at a petrol station with a not-full tank and it can afford petrol, it'll purchase enough to fill the tank.
		else if (bots_location->type == LOCATION_PETROL_STATION && bots_location->quantity != 0 && b->fuel < b->fuel_tank_capacity && b->cash > bots_location->price) {
			*action = ACTION_BUY;
			*n = (b->fuel_tank_capacity) - (b->fuel);
			return;
		}
//If the bot is currently carrying no cargo and its at a seller, it'll purchase as much of the commodity as it can sell.
		else if (b->cargo == NULL) {
			if (nearest_seller(b) == 0) {
				if (check_buyers(b) != 0 && b->turns_left > 2) {
					if ( (b->cash) >= (bots_location -> price) ) {
						*action = ACTION_BUY;
//check_buyers(b) is the sellable amount of the commodity that the bot is trying to buy i.e. the maximum quantity of the good being bought that can be resold
						*n = check_buyers(b);
						return;
					}
//If the bot can't afford the commodity and the nearest petrol station is along the way to the next nearest seller and it's tank is less than half full, it'll go to the nearest petrol station first
					else if ( (b -> cash) < (b->location->price) ) {
						if ( (b->fuel <= floor(b->fuel_tank_capacity/2) && nearest_fuel(b) > 0 && nearest_seller(b) > 0 && nearest_fuel(b) < nearest_seller(b)) || (b->fuel <= floor(b->fuel_tank_capacity/2) && nearest_fuel(b) < 0 && nearest_seller(b) < 0 && nearest_fuel(b) > nearest_seller(b)) ) {
							*action = ACTION_MOVE;
							*n = nearest_fuel(b);
							return;
						}
//If there is no petrol station along the way or the tank is not less than a third full, the bot will just go directly to the next nearest seller
						else {
							*action = ACTION_MOVE;
							*n = nearest_seller(b);
							return;
						}
					}
				}
//if the bot is at a seller but the there are no longer any buyers of that commodity, the bot will try to go to the next nearest seller, refuelling along the way if needed (if needed meaning if the tank is less than half full)
				else if (check_buyers(b) == 0 || b->turns_left <= 2) {
					if ( (b->fuel <= floor(b->fuel_tank_capacity/2) && nearest_fuel(b) > 0 && nearest_seller(b) > 0 && nearest_fuel(b) < nearest_seller(b)) || (b->fuel <= floor(b->fuel_tank_capacity/2) && nearest_fuel(b) < 0 && nearest_seller(b) < 0 && nearest_fuel(b) > nearest_seller(b)) ) {
						*action = ACTION_MOVE;
						*n = nearest_fuel(b);
						return;
					}
					else {
						*action = ACTION_MOVE;
						*n = next_nearest_seller(b);
						return;
					}
				}
			}
//If the bot is not at a seller and is carrying no cargo, itll go to the next nearest seller, refuelling along the way if needed
			else if (nearest_seller(b) != 0) {
				if ( (b->fuel <= floor(b->fuel_tank_capacity/2) && nearest_fuel(b) > 0 && nearest_seller(b) > 0 && nearest_fuel(b) < nearest_seller(b)) || (b->fuel <= floor(b->fuel_tank_capacity/2) && nearest_fuel(b) < 0 && nearest_seller(b) < 0 && nearest_fuel(b) > nearest_seller(b)) ) {
					*action = ACTION_MOVE;
					*n = nearest_fuel(b);
					return;
				}
				else {
					*action = ACTION_MOVE;
					*n = nearest_seller(b);
					return;
				}
			}
		}
//Check if the bot is currently carrying cargo and at a buyer of that commodity, it'll sell all the cargo that its carrying.
		else if (b->cargo->quantity != 0) {
			if (nearest_buyer(b) == 0) {
				*action = ACTION_SELL;
				*n = b->cargo->quantity;
				return;
			}
//Otherwise, if its not at a buyer of that commodity, itll move to the nearest buyer of that commodity and refuel along the way if needed
			else if ( (b->fuel <= floor(b->fuel_tank_capacity/2) && nearest_fuel(b) > 0 && nearest_buyer(b) > 0 && nearest_fuel(b) < nearest_buyer(b)) || (b->fuel <= floor(b->fuel_tank_capacity/2) && nearest_fuel(b) < 0 && nearest_buyer(b) < 0 && nearest_fuel(b) > nearest_buyer(b)) ) {
				*action = ACTION_MOVE;
				*n = nearest_fuel(b);
			}
			else {
				*action = ACTION_MOVE;
				*n = nearest_buyer(b);
			return;
			}
		}
		return;
	}
//Everything below this part performs the exact same functions as the code above, except it doesn't try to refuel. This is because the first below here if statement verifies that there is no longer any fuel left to purchase in the world, so the bot just buys and sell to the nearest buyers/sellers without trying to refuel.
	else if (check_fuel(b) == 0) {
		if (b->cargo == NULL) {
			if (nearest_seller(b) == 0) {
				if (check_buyers(b) != 0 && b->turns_left > 2) {
					if ( (b->cash) >= (bots_location -> price) ) {
						*action = ACTION_BUY;
						*n = check_buyers(b);
						return;
					}
					else if ( (b -> cash) < (b->location->price) ) {
						*action = ACTION_MOVE;
						*n = nearest_seller(b);
						return;
					}
				}
				else if (check_buyers(b) == 0 || b->turns_left <= 2) {
					*action = ACTION_MOVE;
					*n = next_nearest_seller(b);
					return;
				}
			}
			else if (nearest_seller(b) != 0) {
				*action = ACTION_MOVE;
				*n = nearest_seller(b);
				return;
			}
		}

		else if (b->cargo->quantity != 0) {
			if (nearest_buyer(b) == 0) {
				*action = ACTION_SELL;
				*n = b->cargo->quantity;
				return;
			}
			*action = ACTION_MOVE;
			*n = nearest_buyer(b);
			return;
		}
	}
	return;
}
