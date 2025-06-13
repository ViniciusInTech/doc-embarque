#include <stdio.h>
#include <string.h>

#include "../../include/models/payment.h"
#include "../../include/models/student.h"
#include "../../include/use_cases/create_payment.h"
#include "../../include/utils.h"

#define FILE_PATH "../../data/payments.txt"
#define STUDENT_FILE "../../data/students.txt"

int get_next_payment_id() {
    FILE *file = fopen(FILE_PATH, "r");
    int id = 0, temp;
    Payment p;

    if (file != NULL) {
        while (fscanf(file, "%d;%d;%lf;%[^\n]\n", &temp, &p.student_id, &p.amount, p.method) == 4) {
            if (temp > id) id = temp;
        }
        fclose(file);
    }
    return id + 1;
}

int verify_student_payment(int id) {
    FILE *file = fopen(STUDENT_FILE, "r");
    Student s; int temp; int found = 0;
    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%[^;];%[^;];%[^\n]\n", &temp, &s.classroom_id, s.name, s.rg, s.cpf, s.birth_date) == 6) {
            if (temp == id) { found = 1; break; }
        }
        fclose(file);
    }
    return found;
}

Payment save_payment(Payment p) {
    ensure_data_directory();
    FILE *file = fopen(FILE_PATH, "a");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return p;
    }
    fprintf(file, "%d;%d;%.2lf;%s\n", p.id, p.student_id, p.amount, p.method);
    fclose(file);
    return p;
}

Payment create_payment(Payment p) {
    p.id = get_next_payment_id();
    return save_payment(p);
}

Payment create_payment_cli() {
    Payment p;
    printf("ID do aluno: ");
    scanf("%d", &p.student_id);
    getchar();
    if (!verify_student_payment(p.student_id)) {
        printf("Aluno com ID %d não encontrado.\n", p.student_id);
        p.id = -1;
        return p;
    }
    printf("Valor: ");
    scanf("%lf", &p.amount);
    getchar();
    printf("Metodo de pagamento: ");
    fgets(p.method, sizeof(p.method), stdin);
    strtok(p.method, "\n");
    p = create_payment(p);
    if (p.id != -1) printf("Pagamento salvo com sucesso.\n");
    return p;
}
