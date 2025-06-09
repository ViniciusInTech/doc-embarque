
#include <stdio.h>
#include <string.h>
#include "../../include/models/class.h"


#define DATA_CLASS "../../data/class.txt"

int class_exists(int id) {
    FILE *file = fopen(DATA_CLASS, "r");
    Class c;
    int found = 0;

    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%d;%d\n", &c.class_id, &c.school_id, c.name, &c.students, &c.confirmed_students) == 5) {
            if (c.class_id == id) {
                found = 1;
                break;
            }
        }
        fclose(file);
    }

    return found;
}

void save_class(Class c) {
    FILE *file = fopen(DATA_CLASS, "a");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fprintf(file, "%d;%d;%s;%d;%d\n", c.class_id, c.school_id, c.name, c.students, c.confirmed_students);
    fclose(file);
}

Class create_class_cli() {
    Class c;

    printf("ID da turma: ");
    scanf("%d", &c.class_id);
    getchar();

    if (class_exists(c.class_id)) {
        printf("Turma já existe!\n");
        return c;
    }

    printf("ID da escola: ");
    scanf("%d", &c.school_id);
    getchar();

    printf("Nome da turma: ");
    fgets(c.name, sizeof(c.name), stdin);
    strtok(c.name, "\n");

    printf("Total de alunos: ");
    scanf("%d", &c.students);
    getchar();

    printf("Alunos confirmados: ");
    scanf("%d", &c.confirmed_students);
    getchar();

    save_class(c);
    printf("Turma criada com sucesso!\n");

    return c;
}

//
// Created by gpslg on 08/06/2025.
//
