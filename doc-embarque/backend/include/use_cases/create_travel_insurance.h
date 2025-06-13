#ifndef CREATE_TRAVEL_INSURANCE_H
#define CREATE_TRAVEL_INSURANCE_H

#include "../models/travel_insurance.h"

TravelInsurance create_travel_insurance(TravelInsurance t);
TravelInsurance create_travel_insurance_cli();
int get_next_travel_insurance_id();
TravelInsurance save_travel_insurance(TravelInsurance t);
int verify_student_insurance(int id);

#endif
