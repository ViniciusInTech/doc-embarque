#include <stdio.h>

#include "../../include/models/student.h"
#include "../../include/use_cases/read_student.h"

#include "paths.h"


void read_students() {
    FILE *file = fopen(STUDENTS_FILE_PATH, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Student s;
    while (fscanf(file, "%d;%d;%[^;];%[^;];%[^;];%[^\n]\n", &s.id, &s.classroom_id, s.name, s.rg, s.cpf, s.birth_date) == 6) {
        printf("ID: %d | Turma: %d | Nome: %s | RG: %s | CPF: %s | Nascimento: %s\n", s.id, s.classroom_id, s.name, s.rg, s.cpf, s.birth_date);
    }

    fclose(file);
}
