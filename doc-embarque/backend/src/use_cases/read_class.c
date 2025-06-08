#include <stdio.h>
#include "../../include/models/class.h"

#define DATA_CLASS "../../data/class.txt"

void read_classes() {
    FILE *file = fopen(DATA_CLASS, "r");
    Class c;

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    while (fscanf(file, "%d;%d;%[^;];%d;%d\n", &c.class_id, &c.school_id, c.name, &c.students, &c.confirmed_students) == 5) {
        printf("ID da classe: %d | Escola: %d | Nome da classe: %s | Total de alunos: %d | Total de alunos confirmados: %d\n",
               c.class_id, c.school_id, c.name, c.students, c.confirmed_students);
    }

    fclose(file);
}
