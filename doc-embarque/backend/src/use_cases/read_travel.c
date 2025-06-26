#include <stdio.h>
#include <stdlib.h>

#include "../../include/models/travel.h"
#include "../../include/use_cases/read_travel.h"
#include "paths.h"

#define FILE_PATH "../../data/travels.txt"

int load_travels(Travel **travels) {
    FILE *file = fopen(FILE_PATH, "r");
    if (!file) return 0;
    int count = 0; int id; Travel tmp;
    while (fscanf(file, "%d;%d;%[^;];%lf\n", &id, &tmp.class_id, tmp.name, &tmp.base_cost) == 4) {
        count++; }
    rewind(file);
    *travels = malloc(sizeof(Travel)*count);
    int idx=0;
    while (fscanf(file, "%d;%d;%[^;];%lf\n", &id, &tmp.class_id, tmp.name, &tmp.base_cost) == 4) {
        tmp.id = id;
        (*travels)[idx++] = tmp;
    }
    fclose(file);
    return count;
}