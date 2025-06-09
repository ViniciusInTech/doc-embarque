#include <stdio.h>
#include "../../include/models/class.h"
#define DATA_CLASS "../../data/class.txt"

void delete_class(int class_id) {
    if (class_id <= 0 || class_exists(class_id)) {
        printf("ID inválido ou classe não existe.\n");
        return;
    }

    FILE *file = fopen(DATA_CLASS, "r");
    FILE *temp = fopen("../../data/temp.txt", "w");

    if (!file || !temp) {
        printf("Erro ao acessar os arquivos.\n");
        return;
    }

    Class c;
    while (fscanf(file, "%d %d %s", &c.class_id, &c.school_id, c.name) == 3) {
        if (c.class_id != class_id) {
            fprintf(temp, "%d %d %s\n", c.class_id, c.school_id, c.name);
        }
    }

    fclose(file);
    fclose(temp);

    remove(DATA_CLASS);
    rename("../../data/temp.txt", DATA_CLASS);

    printf("Classe com ID %d deletada com sucesso.\n", class_id);
}
