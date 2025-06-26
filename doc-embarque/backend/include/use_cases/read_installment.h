#ifndef READ_INSTALLMENT_H
#define READ_INSTALLMENT_H

#include "../models/installment.h"

void read_installments();
int load_installments_by_payment(int payment_id, Installment **installments);


#endif
