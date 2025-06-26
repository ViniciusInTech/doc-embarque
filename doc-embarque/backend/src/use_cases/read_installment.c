#include <stdio.h>
#include <stdlib.h>

#include "../../include/models/installment.h"
#include "../../include/use_cases/read_installment.h"

#define FILE_PATH "../../data/installments.txt"

void read_installments() {
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Installment i;
    while (fscanf(file, "%d;%d;%lf;%[^;];%[^;];%[^;];%d\n",
                  &i.id, &i.payment_id, &i.amount,
                  i.due_date, i.proof, i.method, &i.paid) == 7) {
        printf("ID: %d | Pagamento: %d | Valor: %.2lf | Vencimento: %s | Comprovante: %s | Metodo: %s | Pago: %d\n",
               i.id, i.payment_id, i.amount, i.due_date, i.proof, i.method, i.paid);
                  }

    fclose(file);
}

int load_installments_by_payment(int payment_id, Installment **installments) {
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) {
        return 0;
    }

    int count = 0;
    Installment tmp;
    int id;
    while (fscanf(file, "%d;%d;%lf;%[^;];%[^;];%[^;];%d\n",
                  &id, &tmp.payment_id, &tmp.amount,
                  tmp.due_date, tmp.proof, tmp.method, &tmp.paid) == 7) {
        if (payment_id == -1 || tmp.payment_id == payment_id) {
            count++;
        }
                  }

    rewind(file);
    *installments = malloc(sizeof(Installment) * count);
    int idx = 0;
    while (fscanf(file, "%d;%d;%lf;%[^;];%[^;];%[^;];%d\n",
                  &id, &tmp.payment_id, &tmp.amount,
                  tmp.due_date, tmp.proof, tmp.method, &tmp.paid) == 7) {
        if (payment_id == -1 || tmp.payment_id == payment_id) {
            tmp.id = id;
            (*installments)[idx++] = tmp;
        }
                  }

    fclose(file);
    return count;
}