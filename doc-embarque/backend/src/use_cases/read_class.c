#include <stdio.h>
#include <stdlib.h>

#include "../../include/models/class.h"
#include "../../include/use_cases/read_class.h"

#include "paths.h"


void read_classes() {
    FILE *file = fopen(CLASS_FILE_PATH, "r");
    Class c;

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    while (fscanf(file, "%d;%d;%[^;];%d;%d\n", &c.class_id, &c.school_id, c.name,
                   &c.students, &c.confirmed_students) == 5) {
        printf("ID da classe: %d | Escola: %d | Nome da classe: %s | Total de alunos: %d | Total de alunos confirmados: %d\n",
               c.class_id, c.school_id, c.name, c.students, c.confirmed_students);
                   }

    fclose(file);
}

int load_classes(Class **classes) {
    FILE *file = fopen(CLASS_FILE_PATH, "r");
    if (!file) {
        return 0;
    }

    int count = 0;
    Class tmp;
    int id;

    while (fscanf(file, "%d;%d;%[^;];%d;%d\n", &id, &tmp.school_id, tmp.name,
                   &tmp.students, &tmp.confirmed_students) == 5) {
        count++;
                   }

    rewind(file);
    *classes = malloc(sizeof(Class) * count);
    int idx = 0;
    while (fscanf(file, "%d;%d;%[^;];%d;%d\n", &id, &tmp.school_id, tmp.name,
                   &tmp.students, &tmp.confirmed_students) == 5) {
        tmp.class_id = id;
        (*classes)[idx++] = tmp;
                   }

    fclose(file);
    return count;
}