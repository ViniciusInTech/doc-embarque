#include <stdio.h>
#include  <string.h>
#include "../../include/models/student.h"
#include "../../include/use_cases/create_student.h"
#define FILE_PATH "../../data/students.txt"

int get_next_student_id() {
    FILE *file = fopen(FILE_PATH, "r");
    int id = 0, temp;
    Student s;

    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%[^;];%[^;];%[^;]\n", &temp, &s.class_id, s.name, s.rg, s.cpf, s.birth_date) == 6) {
            if (temp > id) {
                id = temp;
            }
        }

        fclose(file);
    }

    return id + 1;
}

Student save_student(Student student) {
    FILE *file = fopen(FILE_PATH, "a");

    if (file == NULL) {
        perror("Erro ao abrir o arquivo.\n");
        return student;
    }

    fprintf(file, "%d;%d;%s;%s;%s;%s;\n", student.id, student.class_id, student.name, student.rg, student.cpf, student.birth_date);
    fclose(file);
    return student;
}

// verify_classroom precisa dos arquivos referentes as turmas

Student create_student(Student student) {
    student.id = get_next_student_id();
    return save_student(student);
}

Student create_student_cli() {
    Student aluno;

    printf("Nome do aluno: ");
    fgets(aluno.name, sizeof(aluno.name), stdin);
    strtok(aluno.name, "\n");

    printf("RG do aluno: ");
    fgets(aluno.rg, sizeof(aluno.rg), stdin);
    strtok(aluno.rg, "\n");

    printf("CPF do aluno: ");
    fgets(aluno.cpf, sizeof(aluno.cpf), stdin);
    strtok(aluno.cpf, "\n");

    printf("Data de nascimento do aluno: ");
    fgets(aluno.birth_date, sizeof(aluno.birth_date), stdin);
    strtok(aluno.birth_date, "\n");

    aluno.class_id = 1;

    return create_student(aluno);
}