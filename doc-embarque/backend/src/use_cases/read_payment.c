#include <stdio.h>
#include <stdlib.h>

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

int load_payments_by_student(int student_id, Payment **payments) {
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        return 0;
    }

    int count = 0;
    Payment tmp;
    int id;
    while (fscanf(file, "%d;%d;%lf;%[^\n]\n", &id, &tmp.student_id, &tmp.amount, tmp.method) == 4) {
        if (student_id == -1 || tmp.student_id == student_id) {
            count++;
        }
    }

    rewind(file);
    *payments = malloc(sizeof(Payment) * count);
    int idx = 0;
    while (fscanf(file, "%d;%d;%lf;%[^\n]\n", &id, &tmp.student_id, &tmp.amount, tmp.method) == 4) {
        if (student_id == -1 || tmp.student_id == student_id) {
            tmp.id = id;
            (*payments)[idx++] = tmp;
        }
    }

    fclose(file);
    return count;
}
