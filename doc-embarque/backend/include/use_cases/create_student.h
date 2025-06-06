#ifndef CREATE_STUDENT_H
#define CREATE_STUDENT_H

#include "../../include/models/student.h"

int get_next_student_id();
Student save_student(Student student);
int verify_classroom(int id);
Student create_student(Student student);
Student create_student_cli();

#endif //CREATE_STUDENT_H