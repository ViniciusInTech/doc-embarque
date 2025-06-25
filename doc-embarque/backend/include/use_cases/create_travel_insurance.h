#ifndef CREATE_TRAVEL_INSURANCE_H
#define CREATE_TRAVEL_INSURANCE_H

#include "../models/travel_insurance.h"

TravelInsurance create_travel_insurance(int cols, TravelInsurance t[cols]);
TravelInsurance create_travel_insurance_cli();
int get_next_travel_insurance_id();
TravelInsurance save_travel_insurance(int cols, TravelInsurance t[cols]);
int verify_class_insurance(int id);

#endif
