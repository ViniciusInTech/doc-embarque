#include <stdio.h>

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
    while (fscanf(file, "%d;%d;%lf;%[^;];%d\n", &i.id, &i.payment_id, &i.amount, i.due_date, &i.paid) == 5) {
        printf("ID: %d | Pagamento: %d | Valor: %.2lf | Vencimento: %s | Pago: %d\n", i.id, i.payment_id, i.amount, i.due_date, i.paid);
    }

    fclose(file);
}
