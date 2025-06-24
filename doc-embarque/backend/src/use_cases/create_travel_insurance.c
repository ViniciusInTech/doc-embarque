#include <stdio.h>
#include <string.h>

#include "../../include/models/travel_insurance.h"
#include "../../include/models/student.h"
#include "../../include/use_cases/create_travel_insurance.h"
#include "../../include/utils.h"
#include "../../include/models/class.h"
#include "../../include/use_cases/count_students.h"

#define FILE_PATH "../../data/travel_insurance.txt"
#define STUDENT_FILE "../../data/students.txt"
#define CLASS_FILE "../../data/class.txt"

int get_next_travel_insurance_id() {
    FILE *file = fopen(FILE_PATH, "r");
    int id = 0, temp;
    TravelInsurance t;
    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%[^;];%[^;];%s\n", &temp, &t.class_id, t.student_name, t.student_rg, t.student_cpf, t.student_bdate) == 6) {
            if (temp > id) id = temp;
        }
        fclose(file);
    }
    return id + 1;
}

int verify_class_insurance(int id) {
    FILE *file = fopen(CLASS_FILE, "r");
    Class c; int temp; int found = 0;
    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%d;%d\n", &temp, &c.school_id, c.name, &c.students, &c.confirmed_students) == 5) {
            if (temp == id) { found = 1; break; }
        }
        fclose(file);
    }
    return found;
}

TravelInsurance save_travel_insurance(int cols, TravelInsurance t[1][cols]) {
    ensure_data_directory();
    FILE *file = fopen(FILE_PATH, "a");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return t[0][0];
    }
    int i;
    for (i = 0; i < cols; i++) {
        fprintf(file, "%d;%d;%s;%s;%s;%s\n", t[0][i].id, t[0][i].class_id, t[0][i].student_name, t[0][i].student_rg, t[0][i].student_cpf, t[0][i].student_bdate);
    }
    fclose(file);
    return t[0][0];
}

TravelInsurance create_travel_insurance(int cols, TravelInsurance t[1][cols]) {
    int i;
    for (i = 0; i < cols; i++) {
        t[0][i].id = get_next_travel_insurance_id();
    }
    return save_travel_insurance(cols, t);
}

TravelInsurance create_travel_insurance_cli() {
    TravelInsurance t;
    int class_id;
    printf("ID da turma: ");
    scanf("%d", &class_id);
    getchar();
    if (!verify_class_insurance(class_id)) {
        printf("Turma com ID %d não encontrada.\n", class_id);
        class_id = -1;
        return t;
    }

    int cs = count_students(class_id);
    TravelInsurance t1[1][cs];
    int c;
    for (c = 0; c < cs; c++) {
        t1[0][c].class_id = class_id;
    }

    FILE *file = fopen(STUDENT_FILE, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo\n");
        return t;
    }

    int temp, i = 0;
    Student s;
    while (fscanf(file, "%d;%d;%[^;];%[^;];%[^;];%s\n", &s.id, &temp, s.name, s.rg, s.cpf, s.birth_date) == 6) {
        if (temp == class_id) {
            strcpy(t1[0][i].student_name, s.name);
            strcpy(t1[0][i].student_rg, s.rg);
            strcpy(t1[0][i].student_cpf, s.cpf);
            strcpy(t1[0][i].student_bdate, s.birth_date);
            i ++;
        }
    }

    fclose(file);
    if (class_id != -1) printf("Seguro Viagem salvo com sucesso.\n");
    return create_travel_insurance(cs, t1);
}
