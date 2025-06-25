#ifndef CREATE_BOARDING_H
#define CREATE_BOARDING_H

#include "../models/boarding.h"

Boarding create_boarding(int cols, Boarding b[cols]);
Boarding create_boarding_cli();
int get_next_boarding_id();
Boarding save_boarding(int cols, Boarding b[cols]);
int verify_class_board(int id);

#endif
