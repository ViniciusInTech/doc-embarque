#include <stdio.h>
#include <stdlib.h>

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

int load_students_by_class(int class_id, Student **students) {
    FILE *file = fopen(STUDENTS_FILE_PATH, "r");
    if (!file) {
        return 0;
    }

    int count = 0;
    Student tmp;
    int id;

    while (fscanf(file, "%d;%d;%[^;];%[^;];%[^;];%[^\n]\n", &id, &tmp.classroom_id,
                   tmp.name, tmp.rg, tmp.cpf, tmp.birth_date) == 6) {
        if (tmp.classroom_id == class_id) {
            count++;
        }
                   }

    rewind(file);
    *students = malloc(sizeof(Student) * count);
    int idx = 0;
    while (fscanf(file, "%d;%d;%[^;];%[^;];%[^;];%[^\n]\n", &id, &tmp.classroom_id,
                   tmp.name, tmp.rg, tmp.cpf, tmp.birth_date) == 6) {
        if (tmp.classroom_id == class_id) {
            tmp.id = id;
            (*students)[idx++] = tmp;
        }
                   }

    fclose(file);
    return count;
}