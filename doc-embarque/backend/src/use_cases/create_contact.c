#include <stdio.h>
#include <string.h>

#include "../../include/models/contact.h"
#include "../../include/models/school.h"
#include "../../include/use_cases/create_contact.h"

#include "paths.h"
#include "../../include/use_cases/create_class.h" // para funcao verify_school
#include "../../include/utils.h"


int get_next_contact_id() {
    FILE *file = fopen(CONTACTS_FILE_PATH, "r");
    int id = 0, temp;
    ContactPerson c;

    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%[^;];%[^\n]\n", &temp, &c.school_id, c.name, c.phone, c.email) == 5) {
            if (temp > id) {
                id = temp;
            }
        }
        fclose(file);
    }
    return id + 1;
}

int contact_exists(int id) {
    FILE *file = fopen(CONTACTS_FILE_PATH, "r");
    ContactPerson c;
    int temp;
    int found = 0;

    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%[^;];%[^\n]\n", &temp, &c.school_id, c.name, c.phone, c.email) == 5) {
            if (temp == id) {
                found = 1;
                break;
            }
        }
        fclose(file);
    }
    return found;
}


ContactPerson save_contact(ContactPerson c) {
    ensure_data_directory();
    FILE *file = fopen(CONTACTS_FILE_PATH, "r");

    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return c;
    }

    fprintf(file, "%d;%d;%s;%s;%s\n", c.id, c.school_id, c.name, c.phone, c.email);
    fclose(file);
    return c;
}

ContactPerson create_contact(ContactPerson c) {
    c.id = get_next_contact_id();
    return save_contact(c);
}

ContactPerson create_contact_cli() {
    ContactPerson c;

    printf("ID da escola: ");
    scanf("%d", &c.school_id);
    getchar();
    if (!verify_school(c.school_id)) {
        printf("Escola com ID %d não encontrada.\n", c.school_id);
        c.id = -1;
        return c;
    }

    printf("Nome do contato: ");
    fgets(c.name, sizeof(c.name), stdin);
    strtok(c.name, "\n");

    printf("Telefone: ");
    fgets(c.phone, sizeof(c.phone), stdin);
    strtok(c.phone, "\n");

    printf("Email: ");
    fgets(c.email, sizeof(c.email), stdin);
    strtok(c.email, "\n");

    c = create_contact(c);
    if (c.id != -1) {
        printf("Contato salvo com sucesso.\n");
    }
    return c;
}