#include <stdio.h>
#include <string.h>
#include "../../include/models/boarding.h"
#include "../../include/use_cases/boarding_spreadsheet.h"
#define FILE_PATH "../../data/boarding.txt"
#define CSV_PATH "../../data/boarding.csv"

int verify_boarding_id(int id) {
    FILE *file = fopen(FILE_PATH, "r");
    Boarding b;
    int temp;
    int found = 0;
    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%s\n", &temp, &b.class_id, b.student_name, b.student_rg) == 4) {
            if (temp == id) {
                found = 1;
                break;
            }
        }
        fclose(file);
    }
    return found;
}

int count_students_boarding(int id) {
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo\n");
        return 0;
    }
    Boarding b;
    int count = 0;
    while (fscanf(file, "%d;%d;%[^;];%s\n", &b.id, &b.class_id, b.student_name, b.student_rg) == 4) {
        if (b.id == id) {
            count++;
        }
    }
    fclose(file);
    return count;
}

Boarding boarding_spreadsheet() {
    int id;
    Boarding b;
    printf("ID do Embarque: ");
    scanf("%d", &id);
    getchar();
    if (!verify_boarding_id(id)) {
        printf("Embarque de ID %d não encontrado.\n", id);
        return b;
    }

   int cs = count_students_boarding(id);
    Boarding b1[1][cs];
    Boarding b2[1][cs];
    int i = 0, j = 0;

    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo\n");
        return b;
    }
    while (fscanf(file, "%d;%d;%[^;];%s\n", &b1[0][i].id, &b1[0][i].class_id, b1[0][i].student_name, b1[0][i].student_rg) == 4) {
        if (b1[0][i].id == id) {
            b2[0][j].id = b1[0][i].id;
            b2[0][j].class_id = b1[0][i].class_id;
            strcpy(b2[0][j].student_name, b1[0][i].student_name);
            strcpy(b2[0][j].student_rg, b1[0][i].student_rg);
            j++;
        }
        i++;
    }
    fclose(file);

    FILE *file2 = fopen(CSV_PATH, "w");
    if (!file2) {
        perror("Erro ao abrir o arquivo\n");
        return b;
    }
    fprintf(file2, "NOME; RG; EMBARCOU\n");
    int a;
    for (a = 0; a < cs; a++) {
        fprintf(file2, "%s;%s\n", b2[0][a].student_name, b2[0][a].student_rg);
    }
    fclose(file2);
    return b;
}
