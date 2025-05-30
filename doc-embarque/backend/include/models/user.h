#ifndef USER_H
#define USER_H

typedef struct {
    int id;
    char name[100];
    char email[100];
    char password[100];
    char role[20];
} User;

#endif
