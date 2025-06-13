#include <stdio.h>

#include "../../include/models/payment.h"
#include "../../include/use_cases/read_payment.h"

#define FILE_PATH "../../data/payments.txt"

void read_payments() {
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Payment p;
    while (fscanf(file, "%d;%d;%lf;%[^\n]\n", &p.id, &p.student_id, &p.amount, p.method) == 4) {
        printf("ID: %d | Aluno: %d | Valor: %.2lf | Metodo: %s\n", p.id, p.student_id, p.amount, p.method);
    }

    fclose(file);
}
