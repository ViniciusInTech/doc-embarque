#ifndef CREATE_CONTACT_H
#define CREATE_CONTACT_H

#include "../models/contact.h"

ContactPerson create_contact(ContactPerson contact);
ContactPerson create_contact_cli();
int get_next_contact_id();
int contact_exists(int id);
ContactPerson save_contact(ContactPerson c);

#endif