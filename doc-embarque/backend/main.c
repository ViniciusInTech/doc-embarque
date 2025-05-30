#include <stdio.h>
#include <string.h>
#include "include/models/user.h"
#include "include/services/user_service.h"

int main() {
    User novo;

    novo.id = get_next_user_id();

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

    save_user(novo);
    printf("Usuário cadastrado com sucesso!\n");

    return 0;
}
