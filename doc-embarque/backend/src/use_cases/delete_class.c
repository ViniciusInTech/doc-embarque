#include <stdio.h>

#include "../../include/models/class.h"
#include "../../include/use_cases/create_class.h"
#include "../../include/use_cases/delete_class.h"
#include <stdlib.h>

#define FILE_PATH "../../data/class.txt"

int delete_class(int class_id) {
    if (class_id <= 0 || !class_exists(class_id)) {
        printf("ID inválido ou classe não existe.\n");
        return 0;
    }

    FILE *file = fopen(FILE_PATH, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    Class *classes = NULL;
    size_t count = 0;
    Class c;

    while (fscanf(file, "%d;%d;%[^;];%d;%d\n", &c.class_id, &c.school_id, c.name,
                   &c.students, &c.confirmed_students) == 5) {
        if (c.class_id != class_id) {
            Class *tmp = realloc(classes, (count + 1) * sizeof(Class));
            if (!tmp) {
                perror("Erro de memória");
                free(classes);
                fclose(file);
                return 0;
            }
            classes = tmp;
            classes[count++] = c;
        }
                   }
    fclose(file);

    file = fopen(FILE_PATH, "w");
    if (file == NULL) {
        perror("Erro ao escrever no arquivo");
        free(classes);
        return 0;
    }

    for (size_t i = 0; i < count; ++i) {
        fprintf(file, "%d;%d;%s;%d;%d\n", classes[i].class_id, classes[i].school_id,
                classes[i].name, classes[i].students, classes[i].confirmed_students);
    }

    fclose(file);
    free(classes);

    printf("Classe com ID %d deletada com sucesso.\n", class_id);
    return 1;
}
