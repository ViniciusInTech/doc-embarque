#ifndef CREATE_SCHOOL_H
#define CREATE_SCHOOL_H
#include "../../include/models/school.h"

School create_school();
School create_school_cli();
int get_next_school_id();
School save_school(School school);
int verify_user(int id);

#endif //CREATE_SCHOOL_H
