#include <stdio.h>

#include "../../include/models/class.h"
#include "../../include/use_cases/update_class.h"
#include <stdlib.h>

#include "paths.h"


int update_class(Class updated) {
    FILE *file = fopen(CLASS_FILE_PATH, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    Class *classes = NULL;
    size_t count = 0;
    Class c;
    int found = 0;

    while (fscanf(file, "%d;%d;%[^;];%d\n", &c.class_id, &c.school_id, c.name,
                   &c.students) == 4) {
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
            return 0;
        }
        classes = tmp;
        classes[count++] = to_store;
                   }
    fclose(file);

    if (!found) {
        free(classes);
        printf("Classe com ID %d não encontrada.\n", updated.class_id);
        return 0;
    }

    file = fopen(CLASS_FILE_PATH, "w");
    if (file == NULL) {
        perror("Erro ao escrever no arquivo");
        free(classes);
        return 0;
    }

    for (size_t i = 0; i < count; ++i) {
        fprintf(file, "%d;%d;%s;%d\n", classes[i].class_id, classes[i].school_id,
                classes[i].name, classes[i].students);
    }
    fclose(file);
    free(classes);

    printf("Classe atualizada com sucesso.\n");
    return 1;
}
