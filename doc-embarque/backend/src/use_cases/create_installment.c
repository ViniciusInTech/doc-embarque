#include <stdio.h>
#include <string.h>

#include "../../include/models/installment.h"
#include "../../include/models/payment.h"
#include "../../include/use_cases/create_installment.h"
#include "../../include/utils.h"

#define FILE_PATH "../../data/installments.txt"
#define PAYMENT_FILE "../../data/payments.txt"

int get_next_installment_id() {
    FILE *file = fopen(FILE_PATH, "r");
    int id = 0, temp;
    Installment i;
    if (file != NULL) {
        while (fscanf(file, "%d;%d;%lf;%[^;];%[^;];%[^;];%d\n", &temp, &i.payment_id,
                       &i.amount, i.due_date, i.proof, i.method, &i.paid) == 7) {
            if (temp > id) id = temp;
                       }
        fclose(file);
    }
    return id + 1;
}

int verify_payment_installment(int id) {
    FILE *file = fopen(PAYMENT_FILE, "r");
    Payment p; int temp; int found = 0;
    if (file != NULL) {
        while (fscanf(file, "%d;%d;%lf;%[^\n]\n", &temp, &p.student_id, &p.amount, p.method) == 4) {
            if (temp == id) { found = 1; break; }
        }
        fclose(file);
    }
    return found;
}

Installment save_installment(Installment i) {
    ensure_data_directory();
    FILE *file = fopen(FILE_PATH, "a");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return i;
    }
    fprintf(file, "%d;%d;%.2lf;%s;%s;%s;%d\n",
            i.id, i.payment_id, i.amount, i.due_date, i.proof, i.method, i.paid);
    fclose(file);
    return i;
}

Installment create_installment(Installment i) {
    i.id = get_next_installment_id();
    return save_installment(i);
}

Installment create_installment_cli() {
    Installment i;
    printf("ID do pagamento: ");
    scanf("%d", &i.payment_id);
    getchar();
    if (!verify_payment_installment(i.payment_id)) {
        printf("Pagamento com ID %d não encontrado.\n", i.payment_id);
        i.id = -1;
        return i;
    }
    printf("Valor da parcela: ");
    scanf("%lf", &i.amount);
    getchar();
    printf("Vencimento: ");
    fgets(i.due_date, sizeof(i.due_date), stdin);
    strtok(i.due_date, "\n");
    printf("Comprovante: ");
    fgets(i.proof, sizeof(i.proof), stdin);
    strtok(i.proof, "\n");
    printf("Metodo: ");
    fgets(i.method, sizeof(i.method), stdin);
    strtok(i.method, "\n");
    i.paid = 0;
    i = create_installment(i);
    if (i.id != -1) printf("Parcela salva com sucesso.\n");
    return i;
}
