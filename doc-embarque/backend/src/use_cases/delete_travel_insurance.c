#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/models/travel_insurance.h"
#include "../../include/use_cases/delete_travel_insurance.h"

#define FILE_PATH "../../data/travel_insurance.txt"
#define TEMP_PATH "../../data/travel_insurance.tmp"

int delete_travel_insurance(int id) {
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

    TravelInsurance t;
    int curr_id;
    int found = 0;

    while (fscanf(file, "%d;%d;%[^;];%[^\n]\n", &curr_id, &t.student_id, t.policy_number, t.provider) == 4) {
        if (curr_id == id) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d;%d;%s;%s\n", curr_id, t.student_id, t.policy_number, t.provider);
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
