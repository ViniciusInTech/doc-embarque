#include <stdio.h>

#include "../../include/models/boarding.h"
#include "../../include/use_cases/read_boarding.h"

#define FILE_PATH "../../data/boarding.txt"

void read_boardings() {
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Boarding b;
    while (fscanf(file, "%d;%d;%[^;];%d\n", &b.id, &b.student_id, b.date, &b.boarded) == 4) {
        printf("ID: %d | Aluno: %d | Data: %s | Embarcou: %d\n", b.id, b.student_id, b.date, b.boarded);
    }

    fclose(file);
}
