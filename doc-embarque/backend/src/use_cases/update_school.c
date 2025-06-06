#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/models/school.h"
#include "../../include/use_cases/update_school.h"

#define FILE_PATH "../../data/schools.txt"
#define TEMP_FILE "../../data/schools.tmp"

void remove_newline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

int update_school(School updated) {
    FILE *file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    FILE *temp = fopen(TEMP_FILE, "w");
    if (temp == NULL) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    int found = 0;
    School curr;
    int id;

    while (fscanf(file, "%d;%d;%[^;];%[^;]", &id, &curr.user_id, curr.name, curr.address) == 4) {
        if (id == updated.id) {
            found = 1;
            if (strlen(updated.name) > 0) strcpy(curr.name, updated.name);
            if (strlen(updated.address) > 0) strcpy(curr.address, updated.address);
        }

        remove_newline(curr.name);
        remove_newline(curr.address);

        fprintf(temp, "%d;%d;%s;%s;\n", id, curr.user_id, curr.name, curr.address);
    }

    fclose(file);
    fclose(temp);

    if (!found) {
        remove(TEMP_FILE);
        return 0;
    }

    remove(FILE_PATH);
    rename(TEMP_FILE, FILE_PATH);

    return 1;
}

School update_school_cli() {
    School updated;
    char buffer[128];

    memset(&updated, 0, sizeof(School));

    printf("ID da escola: ");
    fgets(buffer, 128, stdin);
    updated.id = atoi(buffer);

    printf("Novo nome (deixe vazio para manter): ");
    fgets(buffer, 128, stdin);
    remove_newline(buffer);
    if (strlen(buffer) > 0) strcpy(updated.name, buffer);

    printf("Novo endereço (deixe vazio para manter): ");
    fgets(buffer, 128, stdin);
    remove_newline(buffer);
    if (strlen(buffer) > 0) strcpy(updated.address, buffer);

    if (!update_school(updated)) {
        printf("Escola não encontrada.\n");
        updated.id = -1;
    }
    else {
        printf("Escola atualizada com sucesso!\n");
    }

    return updated;
}