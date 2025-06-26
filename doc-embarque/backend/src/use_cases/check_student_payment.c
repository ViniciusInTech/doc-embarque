#include "../../include/use_cases/check_student_payment.h"
#include "../../include/use_cases/read_payment.h"
#include "../../include/use_cases/read_installment.h"
#include <stdlib.h>

int is_student_paid(int student_id) {
    Payment *payments = NULL;
    int pcount = load_payments_by_student(student_id, &payments);
    int paid = 0;
    for (int i = 0; i < pcount; i++) {
        Installment *insts = NULL;
        int icount = load_installments_by_payment(payments[i].id, &insts);
        double total = 0;
        for (int j = 0; j < icount; j++) {
            if (insts[j].paid) total += insts[j].amount;
        }
        if (total >= payments[i].amount) {
            paid = 1;
            free(insts);
            break;
        }
        free(insts);
    }
    free(payments);
    return paid;
}