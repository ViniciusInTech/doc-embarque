#include "../include/session.h"

static User current_user;
static int logged_in = 0;

void set_current_user(User user) {
    current_user = user;
    logged_in = 1;
}

User get_current_user(void) {
    return current_user;
}

int is_logged_in(void) {
    return logged_in;
}