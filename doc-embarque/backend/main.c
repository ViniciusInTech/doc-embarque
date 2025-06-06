#include <stdio.h>
#include "user.h"
#include "include/models/school.h"
#include "include/use_cases/create_user.h"
#include "include/use_cases/login.h"
#include "include/use_cases/update_user.h"
#include "include/use_cases/create_school.h"

int main() {
    User user = create_user_cli();
    printf("Usuário '%s' cadastrado com ID %d.\n", user.name, user.id);

    update_user_cli();
  
    School escola;

    if (verify_user(1)) {
        escola = create_school_cli();
        printf("Escola '%s' cadastrada com ID %d.\n", escola.name, escola.id);
    }
    else {
        printf("Usuário não encontrado.\n");

        User logged;
        if (login_cli(&logged)) {
            printf("Login realizado com sucesso. Bem-vindo, %s!\n", logged.name);
        } else {
            printf("Falha no login.\n");
        }

        return 0;
    }
}