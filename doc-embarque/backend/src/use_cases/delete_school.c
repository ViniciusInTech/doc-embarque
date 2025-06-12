#include <stdio.h>
#include <stdlib.h>
#include "../../include/models/school.h"
#include "../../include/use_cases/delete_school.h"
#include "../../include/use_cases/create_school.h"

#define FILE_PATH "../../data/schools.txt"

void delete_school(int school_id) {
    if (school_id <= 0 || !school_exists(school_id)) {
        printf("ID inválido ou escola não existe.\n");
        return;
    }

    FILE *file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo.\n");
        return;
    }

    School *schools = NULL;
    size_t count = 0;
    School s;

    while (fscanf(file, "%d;%d;%[^;];%s\n", &s.id, &s.user_id, s.name, s.address) == 4) {
        if (s.id != school_id) {
            School *tmp = realloc(schools,(count + 1) * sizeof(School));
            if (!tmp) {
                perror("Erro de memória.\n");
                free(schools);
                fclose(file);
                return;
            }
            schools = tmp;
            schools[count++] = s;
        }
    }
    fclose(file);

    file = fopen(FILE_PATH, "w");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo.\n");
        free(schools);
        return;
    }

    for (size_t i = 0; i < count; i++) {
        fprintf(file, "%d;%d;%s;%s\n", schools[i].id, schools[i].user_id, schools[i].name, schools[i].address);
    }

    fclose(file);
    free(schools);

    printf("Escola com ID %d deletada com sucesso.\n", school_id);
}

