#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "bot_functions.h"
#include "trader_bot.h"


void get_action(struct bot *b, int *action, int *n);
char *get_bot_name(void);

char *get_bot_name(void) {
	return "Busted Bot";
}

void get_action(struct bot *b, int *action, int *n) {
	*action = ACTION_MOVE;
	*n = nearest_deal(b);
}
