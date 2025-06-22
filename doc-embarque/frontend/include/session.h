#ifndef SESSION_H
#define SESSION_H

#include "../../backend/include/models/user.h"

void set_current_user(User user);
User get_current_user(void);
int is_logged_in(void);

#endif