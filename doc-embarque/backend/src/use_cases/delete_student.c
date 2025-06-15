#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/models/student.h"
#include "../../include/use_cases/delete_student.h"

#include "paths.h"


int delete_student(int id) {
    FILE *file = fopen(STUDENTS_FILE_PATH, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    FILE *temp = fopen(STUDENTS_TEMP_PATH, "w");
    if (!temp) {
        perror("Erro ao criar arquivo temporario");
        fclose(file);
        return 0;
    }

    Student curr;
    int curr_id;
    int found = 0;

    while (fscanf(file, "%d;%d;%[^;];%[^;];%[^;];%[^\n]\n", &curr_id, &curr.classroom_id, curr.name, curr.rg, curr.cpf, curr.birth_date) == 6) {
        if (curr_id == id) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d;%d;%s;%s;%s;%s\n", curr_id, curr.classroom_id, curr.name, curr.rg, curr.cpf, curr.birth_date);
    }

    fclose(file);
    fclose(temp);

    if (!found) {
        remove(STUDENTS_TEMP_PATH);
        return 0;
    }

    remove(STUDENTS_FILE_PATH);
    rename(STUDENTS_TEMP_PATH, STUDENTS_FILE_PATH);
    return 1;
}
