#ifndef CREATE_BOARDING_H
#define CREATE_BOARDING_H

#include "../models/boarding.h"

Boarding create_boarding(Boarding b);
Boarding create_boarding_cli();
int get_next_boarding_id();
Boarding save_boarding(Boarding b);
int verify_student_board(int id);

#endif
