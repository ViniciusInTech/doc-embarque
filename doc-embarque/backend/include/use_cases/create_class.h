#ifndef CREATE_CLASS_H
#define CREATE_CLASS_H

#include "../models/class.h"

int get_next_class_id();
int class_exists(int id);
int verify_school(int id);
Class save_class(Class c);
Class create_class_cli();

#endif //CREATE_CLASS_H