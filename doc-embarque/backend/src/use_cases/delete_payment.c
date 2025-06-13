#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/models/payment.h"
#include "../../include/use_cases/delete_payment.h"

#define FILE_PATH "../../data/payments.txt"
#define TEMP_PATH "../../data/payments.tmp"

int delete_payment(int id) {
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    FILE *temp = fopen(TEMP_PATH, "w");
    if (!temp) {
        perror("Erro ao criar arquivo temporario");
        fclose(file);
        return 0;
    }

    Payment p;
    int curr_id;
    int found = 0;

    while (fscanf(file, "%d;%d;%lf;%[^\n]\n", &curr_id, &p.student_id, &p.amount, p.method) == 4) {
        if (curr_id == id) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d;%d;%.2lf;%s\n", curr_id, p.student_id, p.amount, p.method);
    }

    fclose(file);
    fclose(temp);

    if (!found) {
        remove(TEMP_PATH);
        return 0;
    }

    remove(FILE_PATH);
    rename(TEMP_PATH, FILE_PATH);
    return 1;
}
