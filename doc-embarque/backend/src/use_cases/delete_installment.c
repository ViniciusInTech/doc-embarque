#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/models/installment.h"
#include "../../include/use_cases/delete_installment.h"

#define FILE_PATH "../../data/installments.txt"
#define TEMP_PATH "../../data/installments.tmp"

int delete_installment(int id) {
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

    Installment i;
    int curr_id;
    int found = 0;

    while (fscanf(file, "%d;%d;%lf;%[^;];%d\n", &curr_id, &i.payment_id, &i.amount, i.due_date, &i.paid) == 5) {
        if (curr_id == id) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d;%d;%.2lf;%s;%d\n", curr_id, i.payment_id, i.amount, i.due_date, i.paid);
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
