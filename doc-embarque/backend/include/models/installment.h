#ifndef INSTALLMENT_H
#define INSTALLMENT_H

typedef struct {
    int id;
    int payment_id;
    double amount;
    char due_date[20];
    char proof[100];
    char method[20];
    int paid;
} Installment;

#endif
