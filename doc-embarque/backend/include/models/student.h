#ifndef STUDENT_H
#define STUDENT_H

typedef struct {
    int id;
    int classroom_id;
    char name[100];
    char rg[20];
    char cpf[20];
    char birth_date[20];
} Student;

#endif