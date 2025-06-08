#include <stdio.h>
#include "../../include/models/class.h"
#include "../../include/use_cases/create_class.h"
#define DATA_CLASS "../../data/class.txt"



void delete_class(int class_id) {
    if (class_id <= 0 || !class_exists(class_id)) {
        printf("ID inválido ou classe não existe.\n");
        return;
    }

    FILE *file = fopen(DATA_CLASS, "r");
    FILE *temp = fopen("temp.txt", "w");
    Class c;

    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir arquivo.\n");
        if (file) fclose(file);
        if (temp) fclose(temp);
        return;
    }

    while (fscanf(file, "%d;%d;%[^;];%d;%d\n", &c.class_id, &c.school_id, c.name, &c.students, &c.confirmed_students) == 5) {
        if (c.class_id != class_id) {
            fprintf(temp, "%d;%d;%s;%d;%d\n", c.class_id, c.school_id, c.name, c.students, c.confirmed_students);
        }
    }

    fclose(file);
    fclose(temp);

    remove(DATA_CLASS);
    rename("temp.txt", DATA_CLASS);

    printf("Classe deletada com sucesso!\n");
}
