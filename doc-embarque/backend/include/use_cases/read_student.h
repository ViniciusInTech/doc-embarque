#ifndef READ_STUDENT_H
#define READ_STUDENT_H

#include "../models/student.h"

void read_students();
int load_students_by_class(int class_id, Student **students);

#endif