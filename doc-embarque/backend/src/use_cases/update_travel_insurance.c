#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/models/travel_insurance.h"
#include "../../include/use_cases/update_travel_insurance.h"

#define FILE_PATH "../../data/travel_insurance.txt"
#define TEMP_PATH "../../data/travel_insurance.tmp"

int update_travel_insurance(TravelInsurance updated) {
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

    TravelInsurance curr;
    int id;
    int found = 0;

    while (fscanf(file, "%d;%d;%[^;];%[^\n]\n", &id, &curr.student_id, curr.policy_number, curr.provider) == 4) {
        if (id == updated.id) {
            found = 1;
            if (updated.student_id != 0) curr.student_id = updated.student_id;
            if (strlen(updated.policy_number) > 0) strcpy(curr.policy_number, updated.policy_number);
            if (strlen(updated.provider) > 0) strcpy(curr.provider, updated.provider);
        }
        fprintf(temp, "%d;%d;%s;%s\n", id, curr.student_id, curr.policy_number, curr.provider);
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
