#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/models/installment.h"
#include "../../include/use_cases/update_installment.h"

#define FILE_PATH "../../data/installments.txt"
#define TEMP_PATH "../../data/installments.tmp"

int update_installment(Installment updated) {
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

    Installment curr;
    int id;
    int found = 0;

    while (fscanf(file, "%d;%d;%lf;%[^;];%d\n", &id, &curr.payment_id, &curr.amount, curr.due_date, &curr.paid) == 5) {
        if (id == updated.id) {
            found = 1;
            if (updated.payment_id != 0) curr.payment_id = updated.payment_id;
            if (updated.amount != 0) curr.amount = updated.amount;
            if (strlen(updated.due_date) > 0) strcpy(curr.due_date, updated.due_date);
            if (updated.paid != -1) curr.paid = updated.paid;
        }
        fprintf(temp, "%d;%d;%.2lf;%s;%d\n", id, curr.payment_id, curr.amount, curr.due_date, curr.paid);
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
