#include <stdio.h>

#include "../../include/models/user.h"
#include "../../include/use_cases/read_user.h"

#define FILE_PATH "../../data/users.txt"

void read_users() {
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    User u;
    int id;
    while (fscanf(file, "%d;%[^;];%[^;];%[^;];%[^\n]\n", &id, u.name, u.email, u.password, u.role) == 5) {
        u.id = id;
        printf("ID: %d | Nome: %s | Email: %s | Senha: %s | Perfil: %s\n", u.id, u.name, u.email, u.password, u.role);
    }

    fclose(file);
}