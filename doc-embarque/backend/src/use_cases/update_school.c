#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../include/models/school.h"
#include "../../include/use_cases/update_school.h"

#include "paths.h"


int update_school(School updated) {
    FILE *file = fopen(SCHOOLS_FILE_PATH, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    FILE *temp = fopen(SCHOOLS_TEMP_PATH, "w");
    if (!temp) {
        perror("Erro ao criar arquivo temporario");
        fclose(file);
        return 0;
    }

    School curr;
    int id;
    int found = 0;

    while (fscanf(file, "%d;%d;%[^;];%[^\n]\n", &id, &curr.user_id, curr.name, curr.address) == 4) {
        if (id == updated.id) {
            found = 1;
            if (strlen(updated.name) > 0) strcpy(curr.name, updated.name);
            if (strlen(updated.address) > 0) strcpy(curr.address, updated.address);
        }
        fprintf(temp, "%d;%d;%s;%s\n", id, curr.user_id, curr.name, curr.address);
    }

    fclose(file);
    fclose(temp);

    if (!found) {
        remove(SCHOOLS_TEMP_PATH);
        return 0;
    }

    remove(SCHOOLS_FILE_PATH);
    rename(SCHOOLS_TEMP_PATH, SCHOOLS_FILE_PATH);
    return 1;
}
