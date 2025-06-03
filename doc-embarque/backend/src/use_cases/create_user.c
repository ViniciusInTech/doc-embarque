#include <stdio.h>
#include <string.h>
#include "../../include/models/user.h"
#include "../../include/services/user_service.h"
#include "../../include/use_cases/create_user.h"

User create_user(User user) {
    user.id = get_next_user_id();
    User new_user = save_user(user);
    if (new_user.id == -1) {
        printf("Erro: o usuário não foi cadastrado.\n");
        return new_user;
    }
    return new_user;
}

User create_user_cli() {
    User novo;

    printf("Nome: ");
    fgets(novo.name, sizeof(novo.name), stdin);
    strtok(novo.name, "\n");

    printf("Email: ");
    fgets(novo.email, sizeof(novo.email), stdin);
    strtok(novo.email, "\n");

    printf("Senha: ");
    fgets(novo.password, sizeof(novo.password), stdin);
    strtok(novo.password, "\n");

    strcpy(novo.role, "ADMIN");

    return create_user(novo);
}
