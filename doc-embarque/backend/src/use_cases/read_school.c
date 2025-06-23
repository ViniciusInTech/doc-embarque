#include <stdio.h>
#include <stdlib.h>
#include "../../include/models/school.h"
#include "../../include/use_cases/read_school.h"

#include "paths.h"


void read_schools() {
    FILE *file = fopen(SCHOOLS_FILE_PATH, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    School s;
    while (fscanf(file, "%d;%d;%[^;];%[^\n]\n", &s.id, &s.user_id, s.name, s.address) == 4) {
        printf("ID: %d | Usuário: %d | Nome: %s | Endereço: %s\n", s.id, s.user_id, s.name, s.address);
    }

    fclose(file);
}

int load_schools(School **schools) {
    FILE *file = fopen(SCHOOLS_FILE_PATH, "r");
    if (!file) {
        return 0;
    }

    int count = 0;
    School temp;
    int id;

    while (fscanf(file, "%d;%d;%[^;];%[^\n]\n", &id, &temp.user_id, temp.name, temp.address) == 4) {
        count++;
    }

    rewind(file);
    *schools = malloc(sizeof(School) * count);
    int idx = 0;
    while (fscanf(file, "%d;%d;%[^;];%[^\n]\n", &id, &temp.user_id, temp.name, temp.address) == 4) {
        temp.id = id;
        (*schools)[idx++] = temp;
    }

    fclose(file);
    return count;
}