#ifndef LOGIN_H
#define LOGIN_H

#include "../models/user.h"

int login(const char *email, const char *password, User *user);
int login_cli(User *user);

#endif