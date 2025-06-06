#ifndef STUDENT_H
#define STUDENT_H

typedef struct {
    int id;
    int class_id;
    char name[100];
    char rg[100];
    char cpf[100];
    char birth_date[100];
} Student;

#endif //STUDENT_H