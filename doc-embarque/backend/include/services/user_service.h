#ifndef USER_SERVICE_H
#define USER_SERVICE_H

#include "../../include/models/user.h"

User save_user(User user);
int get_next_user_id();
User* load_users(int* total);

#endif
