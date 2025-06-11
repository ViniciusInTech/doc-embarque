#include <stdio.h>
#include <string.h>
#include "../../include/models/student.h"
#include "../../include/use_cases/get_student.h"

#define FILE_PATH "../../data/students.txt"

Student get_student_by_id(int id) {
    FILE *file = fopen(FILE_PATH, "r");
    Student student;
    memset(&student, 0, sizeof(Student));
    student.id = -1;

    if (file == NULL) {
        perror("Erro ao abrir o arquivo.\n");
        return student;
    }

    int temp;
    Student temp_student;
    while (fscanf(file, "%d;%d;%[^;];%[^;];%[^;];%s\n", &temp, &temp_student.classroom_id, temp_student.name, temp_student.rg, temp_student.cpf, temp_student.birth_date) == 6) {
        if (temp == id) {
            temp_student.id = temp;
            student = temp_student;
            break;
        }
    }

    fclose(file);
    return student;
}