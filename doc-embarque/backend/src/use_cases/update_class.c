
#include <stdio.h>
#include "../../include/models/class.h"
#define DATA_CLASS "../../data/class.txt"
#define TEMP_FILE "../../data/temp.txt"


void update_Class(Class updated) {
    FILE *file = fopen(DATA_CLASS, "r");
    FILE *temp = fopen(TEMP_FILE, "w");

    if (file == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    Class c;
    int found = 0;

    while (fscanf(file, "%d;%d;%[^;];%d;%d\n", &c.class_id, &c.school_id, c.name, &c.students, &c.confirmed_students) == 5) {
        if (c.class_id == updated.class_id) {

            fprintf(temp, "%d;%d;%s;%d;%d\n", updated.class_id, updated.school_id, updated.name, updated.students, updated.confirmed_students);
            found = 1;
        } else {

            fprintf(temp, "%d;%d;%s;%d;%d\n", c.class_id, c.school_id, c.name, c.students, c.confirmed_students);
        }
    }

    fclose(file);
    fclose(temp);


    remove(DATA_CLASS);
    rename(TEMP_FILE, DATA_CLASS);

    if (found)
        printf("Classe atualizada com sucesso.\n");
    else
        printf("Classe com ID %d não encontrada.\n", updated.class_id);
}

// Created by gpslg on 08/06/2025.
//

