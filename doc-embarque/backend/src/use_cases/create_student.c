#include <stdio.h>
#include <string.h>

#include "../../include/models/student.h"
#include "../../include/models/class.h"
#include "../../include/use_cases/create_student.h"

#include "paths.h"
#include "../../include/utils.h"


static int get_next_student_id() {
    FILE *file = fopen(STUDENTS_FILE_PATH, "r");
    int id = 0, temp;
    Student s;
    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%[^;];%[^;];%[^\n]\n", &temp, &s.classroom_id, s.name, s.rg, s.cpf, s.birth_date) == 6) {
            if (temp > id) id = temp;
        }
        fclose(file);
    }
    return id + 1;
}

static int verify_classroom(int id) {
    FILE *file = fopen(CLASS_FILE_PATH, "r");
    Class c;
    int temp;
    int found = 0;
    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%d;%d\n", &temp, &c.school_id, c.name, &c.students, &c.confirmed_students) == 5) {
            if (temp == id) {
                found = 1;
                break;
            }
        }
        fclose(file);
    }
    return found;
}

static Student save_student(Student s) {
    ensure_data_directory();
    FILE *file = fopen(STUDENTS_FILE_PATH, "a");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        s.id = -1;
        return s;
    }
    fprintf(file, "%d;%d;%s;%s;%s;%s\n", s.id, s.classroom_id, s.name, s.rg, s.cpf, s.birth_date);
    fclose(file);
    return s;
}

Student create_student(Student s) {
    s.id = get_next_student_id();
    return save_student(s);
}

Student create_student_cli() {
    Student s;
    printf("ID da turma: ");
    scanf("%d", &s.classroom_id);
    getchar();
    if (!verify_classroom(s.classroom_id)) {
        printf("Turma com ID %d não encontrada.\n", s.classroom_id);
        s.id = -1;
        return s;
    }

    printf("Nome do aluno: ");
    fgets(s.name, sizeof(s.name), stdin);
    strtok(s.name, "\n");

    printf("RG: ");
    fgets(s.rg, sizeof(s.rg), stdin);
    strtok(s.rg, "\n");

    printf("CPF: ");
    fgets(s.cpf, sizeof(s.cpf), stdin);
    strtok(s.cpf, "\n");

    printf("Data de nascimento: ");
    fgets(s.birth_date, sizeof(s.birth_date), stdin);
    strtok(s.birth_date, "\n");

    s = create_student(s);
    if (s.id != -1) {
        printf("Aluno salvo com sucesso.\n");
    }
    return s;
}