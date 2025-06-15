#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/models/contact.h"
#include "../../include/use_cases/delete_contact.h"

#include "paths.h"


int delete_contact(int id) {
    FILE *file = fopen(CONTACTS_FILE_PATH, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    FILE *temp = fopen(CONTACTS_TEMP_PATH, "w");
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
        remove(CONTACTS_TEMP_PATH);
        return 0;
    }

    remove(CONTACTS_FILE_PATH);
    rename(CONTACTS_TEMP_PATH, CONTACTS_FILE_PATH);
    return 1;
}
