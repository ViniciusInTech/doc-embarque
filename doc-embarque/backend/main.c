#include <stdio.h>

#include "user.h"
#include "include/use_cases/create_user.h"

int main() {
    User user = create_user_cli();
    printf("Usuário '%s' cadastrado com ID %d.\n", user.name, user.id);
    return 0;
}
