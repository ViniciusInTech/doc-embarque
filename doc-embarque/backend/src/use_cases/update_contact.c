#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../include/models/contact.h"
#include "../../include/use_cases/update_contact.h"

#include "paths.h"


int update_contact(ContactPerson updated) {
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
    int id;
    int found = 0;

    while (fscanf(file, "%d;%d;%[^;];%[^;];%[^\n]\n", &id, &curr.school_id, curr.name, curr.phone, curr.email) == 5) {
        if (id == updated.id) {
            found = 1;
            if (updated.school_id != 0) curr.school_id = updated.school_id;
            if (strlen(updated.name) > 0) strcpy(curr.name, updated.name);
            if (strlen(updated.phone) > 0) strcpy(curr.phone, updated.phone);
            if (strlen(updated.email) > 0) strcpy(curr.email, updated.email);
        }
        fprintf(temp, "%d;%d;%s;%s;%s\n", id, curr.school_id, curr.name, curr.phone, curr.email);
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
