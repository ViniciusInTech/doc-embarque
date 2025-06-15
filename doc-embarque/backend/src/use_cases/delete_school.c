#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/models/school.h"
#include "../../include/use_cases/delete_school.h"

#include "paths.h"


int delete_school(int id) {
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
    int curr_id;
    int found = 0;

    while (fscanf(file, "%d;%d;%[^;];%[^\n]\n", &curr_id, &curr.user_id, curr.name, curr.address) == 4) {
        if (curr_id == id) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d;%d;%s;%s\n", curr_id, curr.user_id, curr.name, curr.address);
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
