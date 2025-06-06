#include <stdio.h>
#include "include/models/student.h"
#include "include/use_cases/create_student.h"

int main() {
    Student aluno;

    aluno = create_student_cli();
    printf("Aluno '%s' cadastrado com sucesso!\n", aluno.name);

    return 0;
}