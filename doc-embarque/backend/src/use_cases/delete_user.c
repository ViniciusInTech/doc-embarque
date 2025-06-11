#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/models/user.h"
#include "../../include/use_cases/delete_user.h"

#define FILE_PATH "../../data/users.txt"
#define TEMP_PATH "../../data/users.tmp"

static void remove_newline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

int delete_user(int id) {
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
    int curr_id;

    while (fscanf(file, "%d;%[^;];%[^;];%[^;];%s\n", &curr_id, curr.name, curr.email, curr.password, curr.role) == 5) {
        if (curr_id == id) {
            found = 1;
            continue;
        }
        remove_newline(curr.name);
        remove_newline(curr.email);
        remove_newline(curr.password);
        remove_newline(curr.role);
        fprintf(temp, "%d;%s;%s;%s;%s\n", curr_id, curr.name, curr.email, curr.password, curr.role);
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

int delete_user_cli() {
    char buffer[128];
    printf("ID do usuário a ser removido: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return 0;
    }
    int id = atoi(buffer);
    if (delete_user(id)) {
        printf("Usuário removido com sucesso.\n");
        return 1;
    }
    printf("Usuário não encontrado.\n");
    return 0;
}