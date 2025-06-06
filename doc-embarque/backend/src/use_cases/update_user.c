#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../include/models/user.h"
#include "../../include/use_cases/update_user.h"

#define FILE_PATH "../../data/users.txt"
#define TEMP_PATH "../../data/users.tmp"

void remove_newline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

int update_user(User updated) {
    FILE *file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    FILE *temp = fopen(TEMP_PATH, "w");
    if (temp == NULL) {
        perror("Erro ao criar arquivo temporário");
        fclose(file);
        return 0;
    }

    int found = 0;
    User curr;
    int id;

    while (fscanf(file, "%d;%[^;];%[^;];%[^;];%s\n", &id, curr.name, curr.email, curr.password, curr.role) == 5) {
        if (id == updated.id) {
            found = 1;
            if (strlen(updated.name) > 0) strcpy(curr.name, updated.name);
            if (strlen(updated.email) > 0) strcpy(curr.email, updated.email);
            if (strlen(updated.password) > 0) strcpy(curr.password, updated.password);
        }

        remove_newline(curr.name);
        remove_newline(curr.email);
        remove_newline(curr.password);
        remove_newline(curr.role);

        fprintf(temp, "%d;%s;%s;%s;%s\n", id, curr.name, curr.email, curr.password, curr.role);
    }

    fclose(file);
    fclose(temp);

    if (!found) {
        remove(TEMP_PATH);
        return 0;
    }

    remove(FILE_PATH);
    rename(TEMP_PATH, FILE_PATH);
    return 1;
}

User update_user_cli() {
    User updated;
    char buffer[128];

    memset(&updated, 0, sizeof(User));

    printf("ID do usuário: ");
    fgets(buffer, sizeof(buffer), stdin);
    updated.id = atoi(buffer);

    printf("Novo nome (deixe vazio para manter): ");
    fgets(buffer, sizeof(buffer), stdin);
    remove_newline(buffer);
    if (strlen(buffer) > 0) strcpy(updated.name, buffer);

    printf("Novo email (deixe vazio para manter): ");
    fgets(buffer, sizeof(buffer), stdin);
    remove_newline(buffer);
    if (strlen(buffer) > 0) strcpy(updated.email, buffer);

    printf("Nova senha (deixe vazio para manter): ");
    fgets(buffer, sizeof(buffer), stdin);
    remove_newline(buffer);
    if (strlen(buffer) > 0) strcpy(updated.password, buffer);

    if (!update_user(updated)) {
        printf("Usuário não encontrado.\n");
        updated.id = -1;
    } else {
        printf("Usuário atualizado com sucesso.\n");
    }

    return updated;
}
