#include <stdio.h>
#include "trader_bot.h"
void print_world(struct bot *b);

void print_world(struct bot *b) {
	struct location *bots_location = b -> location;
	char *starting_location = b->location;
	if (bots_location -> type == LOCATION_START) {
		printf("%s: start\n", bots_location -> name);
	}
	else if (bots_location -> type == LOCATION_SELLER) {
		printf("%s: will sell %d units of %s for $%d\n", bots_location -> name, bots_location -> quantity, bots_location -> commodity -> name, bots_location -> price);
	}
	else if (bots_location -> type == LOCATION_BUYER) {
		printf("%s: will buy %d units of %s for $%d\n", bots_location -> name, bots_location -> quantity, bots_location -> commodity -> name, bots_location -> price);
	}
	else if (bots_location -> type == LOCATION_PETROL_STATION) {
		printf("%s: Petrol station %d units of available fuel for $%d\n", bots_location -> name, bots_location -> quantity, bots_location -> price);
	}
	else if (bots_location -> type == LOCATION_DUMP) {
		printf("Dump: dump\n");
	}
	else if (bots_location -> type == LOCATION_OTHER) {
		printf("%s: other\n", bots_location -> name);
	}

	bots_location = bots_location -> next;

	while (bots_location != starting_location) {
		if (bots_location -> type == LOCATION_START) {
			printf("%s: start\n", bots_location -> name);
        }
        else if (bots_location -> type == LOCATION_SELLER) {
            printf("%s: will sell %d units of %s for $%d\n", bots_location -> name, bots_location -> quantity, bots_location -> commodity -> name, bots_location -> price);
        }
        else if (bots_location -> type == LOCATION_BUYER) {
            printf("%s: will buy %d units of %s for $%d\n", bots_location -> name, bots_location -> quantity, bots_location -> commodity -> name, bots_location -> price);
        }
        else if (bots_location -> type == LOCATION_PETROL_STATION) {
            printf("%s: Petrol station %d units of available fuel for $%d\n", bots_location -> name, bots_location -> quantity, bots_location -> price);
        }
        else if (bots_location -> type == LOCATION_DUMP) {
            printf("Dump: dump\n");
        }
		else if (bots_location -> type == LOCATION_OTHER) {
			printf("%s: other\n", bots_location -> name);
		}
		bots_location = bots_location -> next;
	}
}
