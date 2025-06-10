#include <stdio.h>

#include "../../include/models/class.h"
#include "../../include/use_cases/update_class.h"
#include <stdlib.h>

#define DATA_CLASS "../../data/class.txt"

void update_class(Class updated) {
    FILE *file = fopen(DATA_CLASS, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Class *classes = NULL;
    size_t count = 0;
    Class c;
    int found = 0;

    while (fscanf(file, "%d;%d;%[^;];%d;%d\n", &c.class_id, &c.school_id, c.name,
                   &c.students, &c.confirmed_students) == 5) {
        Class to_store = c;
        if (c.class_id == updated.class_id) {
            to_store = updated;
            found = 1;
        }

        Class *tmp = realloc(classes, (count + 1) * sizeof(Class));
        if (!tmp) {
            perror("Erro de memória");
            free(classes);
            fclose(file);
            return;
        }
        classes = tmp;
        classes[count++] = to_store;
                   }
    fclose(file);

    if (!found) {
        free(classes);
        printf("Classe com ID %d não encontrada.\n", updated.class_id);
        return;
    }

    file = fopen(DATA_CLASS, "w");
    if (file == NULL) {
        perror("Erro ao escrever no arquivo");
        free(classes);
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        fprintf(file, "%d;%d;%s;%d;%d\n", classes[i].class_id, classes[i].school_id,
                classes[i].name, classes[i].students, classes[i].confirmed_students);
    }
    fclose(file);
    free(classes);

    printf("Classe atualizada com sucesso.\n");
}