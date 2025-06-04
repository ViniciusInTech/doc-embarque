#include <stdio.h>

#include "user.h"
#include "include/use_cases/create_user.h"
#include "include/use_cases/login.h"

int main() {
    User user = create_user_cli();
    printf("Usuário '%s' cadastrado com ID %d.\n", user.name, user.id);

    User logged;
    if (login_cli(&logged)) {
        printf("Login realizado com sucesso. Bem-vindo, %s!\n", logged.name);
    } else {
        printf("Falha no login.\n");
    }

    return 0;
}