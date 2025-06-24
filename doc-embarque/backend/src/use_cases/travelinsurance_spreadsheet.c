#include <stdio.h>
#include <string.h>
#include "../../include/models/travel_insurance.h"
#include "../../include/use_cases/travelinsurance_spreadsheet.h"
#define FILE_PATH "../../data/travel_insurance.txt"
#define CSV_PATH "../../data/travel_insurance.csv"

int verify_tv_id(int id) {
    FILE *file = fopen(FILE_PATH, "r");
    TravelInsurance t;
    int temp, found = 0;
    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%[^;];%[^;];%s\n", &temp, &t.class_id, t.student_name, t.student_rg, t.student_cpf, t.student_bdate) == 6) {
            if (temp == id) {
                found = 1;
                break;
            }
        }
        fclose(file);
    }
    return found;
}

int count_students_tv(int id) {
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo.\n");
        return 0;
    }
    TravelInsurance t;
    int count = 0;
    while (fscanf(file, "%d;%d;%[^;];%[^;];%[^;];%s\n", &t.id, &t.class_id, t.student_name, t.student_rg, t.student_cpf, t.student_bdate) == 6) {
        if (t.id == id) {
            count++;
        }
    }
    fclose(file);
    return count;
}

TravelInsurance travel_insurance_spreadsheet() {
    int id;
    TravelInsurance t;
    printf("ID do seguro viagem: ");
    scanf("%d", &id);
    getchar();
    if (!verify_tv_id(id)) {
        printf("Seguro viagem de ID %d não encontrado.\n", id);
        return t;
    }

    int cs = count_students_tv(id);
    TravelInsurance t1[1][cs];
    TravelInsurance t2[1][cs];
    int i = 0, j = 0;

    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo.\n");
        return t;
    }
    while(fscanf(file, "%d;%d;%[^;];%[^;];%[^;];%s\n", &t1[0][i], &t1[0][i].class_id, t1[0][i].student_name, t1[0][i].student_rg, t1[0][i].student_cpf, t1[0][i].student_bdate) == 6) {
        if (t1[0][i].id == id) {
            t2[0][j].id = t1[0][i].id;
            t2[0][j].class_id = t1[0][i].class_id;
            strcpy(t2[0][j].student_name, t1[0][i].student_name);
            strcpy(t2[0][j].student_rg, t1[0][i].student_rg);
            strcpy(t2[0][j].student_cpf, t1[0][i].student_cpf);
            strcpy(t2[0][j].student_bdate, t1[0][i].student_bdate);
            j++;
        }
        i++;
    }
    fclose(file);

    FILE *file2 = fopen(CSV_PATH, "w");
    if (!file2) {
        perror("Erro ao abrir o arquivo.\n");
        return t;
    }
    fprintf(file2, "NOME; RG; CPF; DATA DE NASCIMENTO\n");
    int a;
    for (a = 0; a < cs; a++) {
        fprintf(file2, "%s;%s;%s;%s\n", t2[0][a].student_name, t2[0][a].student_rg, t2[0][a].student_cpf, t2[0][a].student_bdate);
    }
    fclose(file2);
    return t;
}