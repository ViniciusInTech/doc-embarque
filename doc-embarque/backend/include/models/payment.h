#ifndef PAYMENT_H
#define PAYMENT_H

typedef struct {
    int id;
    int student_id;
    double amount;
    char method[50];
} Payment;

#endif
