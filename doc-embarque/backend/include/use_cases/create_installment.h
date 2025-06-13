#ifndef CREATE_INSTALLMENT_H
#define CREATE_INSTALLMENT_H

#include "../models/installment.h"

Installment create_installment(Installment i);
Installment create_installment_cli();
int get_next_installment_id();
Installment save_installment(Installment i);
int verify_payment_installment(int id);

#endif
