#ifndef SCHOOL_SERVICE_H
#define SCHOOL_SERVICE_H

#include "../../include/models/school.h"

int get_next_school_id();
void save_school(School school);
int verify_user(int id);

#endif //SCHOOL_SERVICE_H
