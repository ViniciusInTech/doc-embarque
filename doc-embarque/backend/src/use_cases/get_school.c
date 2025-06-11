#include <stdio.h>
#include <string.h>
#include "../../include/models/school.h"
#include "../../include/use_cases/get_school.h"

#define FILE_PATH "../../data/schools.txt"

School get_school_by_id(int id) {
    FILE *file = fopen(FILE_PATH, "r");
    School school;
    memset(&school, 0, sizeof(school));
    school.id = -1;

    if (file == NULL) {
        perror("Erro ao abrir o arquivo.\n");
        return school;
    }

    int temp;
    School temp_school;
    while (fscanf(file, "%d;%d;%[^;];%s\n", &temp, &temp_school.user_id, temp_school.name, temp_school.address) == 4) {
        if (temp == id) {
            temp_school.id = temp;
            school = temp_school;
            break;
        }
    }

    fclose(file);
    return school;
}