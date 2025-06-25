#include <stdio.h>

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

    while (fscanf(file, "%d;%d;%[^;];%d\n", &c.class_id, &c.school_id, c.name,
                   &c.students) == 4) {
        printf("ID da classe: %d | Escola: %d | Nome da classe: %s | Total de alunos: %d\n",
               c.class_id, c.school_id, c.name, c.students);
                   }

    fclose(file);
}
