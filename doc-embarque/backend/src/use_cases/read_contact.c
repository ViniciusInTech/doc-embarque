#include <stdio.h>

#include "../../include/models/contact.h"
#include "../../include/use_cases/read_contact.h"

#include "paths.h"


void read_contacts() {
    FILE *file = fopen(CONTACTS_FILE_PATH, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    ContactPerson c;
    while (fscanf(file, "%d;%d;%[^;];%[^;];%[^\n]\n", &c.id, &c.school_id, c.name, c.phone, c.email) == 5) {
        printf("ID: %d | Escola: %d | Nome: %s | Telefone: %s | Email: %s\n", c.id, c.school_id, c.name, c.phone, c.email);
    }

    fclose(file);
}