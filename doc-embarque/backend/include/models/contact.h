#ifndef CONTACT_H
#define CONTACT_H

typedef struct {
    int id;
    int school_id;
    char name[100];
    char phone[20];
    char email[100];
} ContactPerson;

#endif