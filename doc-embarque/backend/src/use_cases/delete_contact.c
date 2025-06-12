#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/models/contact.h"
#include "../../include/use_cases/delete_contact.h"

#define FILE_PATH "../../data/contacts.txt"
#define TEMP_PATH "../../data/contacts.tmp"

int delete_contact(int id) {
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    FILE *temp = fopen(TEMP_PATH, "w");
    if (!temp) {
        perror("Erro ao criar arquivo temporario");
        fclose(file);
        return 0;
    }

    ContactPerson curr;
    int curr_id;
    int found = 0;

    while (fscanf(file, "%d;%d;%[^;];%[^;];%[^\n]\n", &curr_id, &curr.school_id, curr.name, curr.phone, curr.email) == 5) {
        if (curr_id == id) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d;%d;%s;%s;%s\n", curr_id, curr.school_id, curr.name, curr.phone, curr.email);
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