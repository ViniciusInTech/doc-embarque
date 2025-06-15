#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/models/student.h"
#include "../../include/use_cases/update_student.h"

#include "paths.h"


int update_student(Student updated) {
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
    int id;
    int found = 0;

    while (fscanf(file, "%d;%d;%[^;];%[^;];%[^;];%[^\n]\n", &id, &curr.classroom_id, curr.name, curr.rg, curr.cpf, curr.birth_date) == 6) {
        if (id == updated.id) {
            found = 1;
            if (strlen(updated.name) > 0) strcpy(curr.name, updated.name);
            if (strlen(updated.rg) > 0) strcpy(curr.rg, updated.rg);
            if (strlen(updated.cpf) > 0) strcpy(curr.cpf, updated.cpf);
            if (strlen(updated.birth_date) > 0) strcpy(curr.birth_date, updated.birth_date);
            if (updated.classroom_id != 0) curr.classroom_id = updated.classroom_id;
        }
        fprintf(temp, "%d;%d;%s;%s;%s;%s\n", id, curr.classroom_id, curr.name, curr.rg, curr.cpf, curr.birth_date);
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
