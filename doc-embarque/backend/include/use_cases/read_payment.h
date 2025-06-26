#ifndef READ_PAYMENT_H
#define READ_PAYMENT_H

#include "../models/payment.h"

void read_payments();
int load_payments_by_student(int student_id, Payment **payments);

#endif
