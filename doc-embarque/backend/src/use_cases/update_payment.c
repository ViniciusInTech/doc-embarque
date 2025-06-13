#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/models/payment.h"
#include "../../include/use_cases/update_payment.h"

#define FILE_PATH "../../data/payments.txt"
#define TEMP_PATH "../../data/payments.tmp"

int update_payment(Payment updated) {
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

    Payment curr;
    int id;
    int found = 0;

    while (fscanf(file, "%d;%d;%lf;%[^\n]\n", &id, &curr.student_id, &curr.amount, curr.method) == 4) {
        if (id == updated.id) {
            found = 1;
            if (updated.student_id != 0) curr.student_id = updated.student_id;
            if (updated.amount != 0) curr.amount = updated.amount;
            if (strlen(updated.method) > 0) strcpy(curr.method, updated.method);
        }
        fprintf(temp, "%d;%d;%.2lf;%s\n", id, curr.student_id, curr.amount, curr.method);
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
