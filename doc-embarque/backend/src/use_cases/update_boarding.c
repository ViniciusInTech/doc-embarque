#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/models/boarding.h"
#include "../../include/use_cases/update_boarding.h"

#define FILE_PATH "../../data/boarding.txt"
#define TEMP_PATH "../../data/boarding.tmp"

int update_boarding(Boarding updated) {
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

    Boarding curr;
    int id;
    int found = 0;

    while (fscanf(file, "%d;%d;%[^;];%d\n", &id, &curr.student_id, curr.date, &curr.boarded) == 4) {
        if (id == updated.id) {
            found = 1;
            if (updated.student_id != 0) curr.student_id = updated.student_id;
            if (strlen(updated.date) > 0) strcpy(curr.date, updated.date);
            if (updated.boarded != -1) curr.boarded = updated.boarded;
        }
        fprintf(temp, "%d;%d;%s;%d\n", id, curr.student_id, curr.date, curr.boarded);
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
