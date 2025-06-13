#include <stdio.h>

#include "../../include/models/travel_insurance.h"
#include "../../include/use_cases/read_travel_insurance.h"

#define FILE_PATH "../../data/travel_insurance.txt"

void read_travel_insurances() {
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    TravelInsurance t;
    while (fscanf(file, "%d;%d;%[^;];%[^\n]\n", &t.id, &t.student_id, t.policy_number, t.provider) == 4) {
        printf("ID: %d | Aluno: %d | Apolice: %s | Seguradora: %s\n", t.id, t.student_id, t.policy_number, t.provider);
    }

    fclose(file);
}
