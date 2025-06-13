#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/models/boarding.h"
#include "../../include/use_cases/delete_boarding.h"

#define FILE_PATH "../../data/boarding.txt"
#define TEMP_PATH "../../data/boarding.tmp"

int delete_boarding(int id) {
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

    Boarding b;
    int curr_id;
    int found = 0;

    while (fscanf(file, "%d;%d;%[^;];%d\n", &curr_id, &b.student_id, b.date, &b.boarded) == 4) {
        if (curr_id == id) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d;%d;%s;%d\n", curr_id, b.student_id, b.date, b.boarded);
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
