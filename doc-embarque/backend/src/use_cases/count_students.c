#include <stdio.h>
#include "../../include/models/student.h"
#include "../../include/use_cases/count_students.h"
#define FILE_PATH "../../data/students.txt"

int count_students(int class_id) {
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo\n");
        return 0;
    }
    Student s;
    int count = 0;
    while (fscanf(file, "%d;%d;%[^;];%[^;];%[^;];%s\n", &s.id, &s.classroom_id, s.name, s.rg, s.cpf, s.birth_date) == 6) {
        if (class_id == s.classroom_id) {
            count++;
        }
    }
    fclose(file);
    return count;
}