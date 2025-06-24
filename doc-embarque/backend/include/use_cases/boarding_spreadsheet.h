#ifndef BOARDING_SPREADSHEET_H
#define BOARDING_SPREADSHEET_H

#include "../../include/models/boarding.h"

Boarding boarding_spreadsheet();
int verify_boarding_id(int id);
int count_students_boarding(int id);

#endif //BOARDING_SPREADSHEET_H
