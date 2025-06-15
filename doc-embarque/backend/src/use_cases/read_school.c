#include <stdio.h>
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
