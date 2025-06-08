#include <stdio.h>
#include "user.h"
#include "include/models/school.h"
#include "include/use_cases/create_user.h"
#include "include/use_cases/login.h"
#include "include/use_cases/update_user.h"
#include "include/use_cases/create_school.h"
#include "include/use_cases/delete_user.h"
#include "include/models/class.h"
#include "include/use_cases/create_class.h"





int main() {
    User user = create_user_cli();
    printf("Usuário '%s' cadastrado com ID %d.\n", user.name, user.id);
    delete_user_cli();

    update_user_cli();
  
    School escola;
    Class c;

    if (verify_user(1)) {
        escola = create_school_cli();
        printf("Escola '%s' cadastrada com ID %d.\n", escola.name, escola.id);

        c = create_class_cli();
        printf("Turma '%s' criada com ID %d para a escola %d.\n",
               c.name, c.class_id, c.school_id);
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