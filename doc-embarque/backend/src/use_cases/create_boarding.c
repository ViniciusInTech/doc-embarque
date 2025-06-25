#include <stdio.h>
#include <string.h>

#include "../../include/models/boarding.h"
#include "../../include/models/student.h"
#include "../../include/models/class.h"
#include "../../include/use_cases/create_boarding.h"
#include "../../include/utils.h"
#include "../../include/use_cases/count_students.h"

#define FILE_PATH "../../data/boarding.txt"
#define STUDENT_FILE "../../data/students.txt"
#define CLASS_FILE "../../data/class.txt"

int get_next_boarding_id() {
    FILE *file = fopen(FILE_PATH, "r");
    int id = 0, temp;
    Boarding b;
    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%s\n", &temp, &b.class_id, b.student_name, b.student_rg) == 4) {
            if (temp > id) id = temp;
        }
        fclose(file);
    }
    return id + 1;
}

int verify_class_board(int id) {
    FILE *file = fopen(CLASS_FILE, "r");
    Class c; int temp; int found = 0;
    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%d\n", &temp, &c.school_id, c.name, &c.students) == 4) {
            if (temp == id) { found = 1; break; }
        }
        fclose(file);
    }
    return found;
}

Boarding save_boarding(int cols, Boarding b[cols]) {
    ensure_data_directory();
    FILE *file = fopen(FILE_PATH, "a");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return b[0];
    }
    int i;
    for (i = 0; i < cols; i++) {
        fprintf(file, "%d;%d;%s;%s\n", b[i].id, b[i].class_id, b[i].student_name, b[i].student_rg);
    }

    fclose(file);
    return b[0];
}

Boarding create_boarding(int cols, Boarding b[cols]) {
    int i;
    for (i = 0; i < cols; i++) {
        b[i].id = get_next_boarding_id();
    }
    return save_boarding(cols, b);
}


Boarding create_boarding_cli() {
    Boarding b;
    int class_id;
    printf("ID da turma: ");
    scanf("%d", &class_id);
    getchar();
    if (!verify_class_board(class_id)) {
        printf("Turma com ID %d não encontrada.\n", class_id);
        class_id = -1;
        return b;
    }
    int cs = count_students(class_id);
    Boarding b1[cs];

    int c;
    for (c = 0; c < cs; c++) {
        b1[c].class_id = class_id ;
    }

    FILE *file = fopen(STUDENT_FILE, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo\n");
        return b;
    }
    int temp;
    Student s;
    int i = 0;
    while (fscanf(file, "%d;%d;%[^;];%[^;];%[^;];%s\n", &s.id, &temp, s.name, s.rg, s.cpf, s.birth_date) == 6) {
        if (temp == class_id) {
            strcpy(b1[i].student_name, s.name);
            strcpy(b1[i].student_rg, s.rg);
            i++;
        }
    }

    fclose(file);

    if (class_id != -1) printf("Embarque salvo com sucesso.\n");
    return create_boarding(cs, b1);
}

