#ifndef CREATE_PAYMENT_H
#define CREATE_PAYMENT_H

#include "../models/payment.h"

Payment create_payment(Payment payment);
Payment create_payment_cli();
int get_next_payment_id();
Payment save_payment(Payment p);
int verify_student_payment(int id);

#endif
