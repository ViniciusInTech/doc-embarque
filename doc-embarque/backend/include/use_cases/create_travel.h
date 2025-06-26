#ifndef CREATE_TRAVEL_H
#define CREATE_TRAVEL_H

#include "../models/travel.h"

int get_next_travel_id();
Travel save_travel(Travel t);
Travel create_travel(Travel t);
Travel create_travel_cli();

#endif