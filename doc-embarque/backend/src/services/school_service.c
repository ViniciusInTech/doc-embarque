#include <stdio.h>
#include "../../include/models/school.h"
#include "../../include/services/school_service.h"
#include "../../include/models/user.h"
#define FILE_PATH "../../data/schools.txt"
#define FILE_PATH_USER "../../data/users.txt"

int get_next_school_id() {
    FILE *file = fopen(FILE_PATH, "r");
    int id = 0, temp;
    School s;

    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%s\n", &temp, &s.user_id, s.name, s.address) == 4) {
            if (temp > id) {
                id = temp;
            }
        }
        fclose(file);
    }

    return id + 1;
}

void save_school(School school) {
    FILE *file = fopen(FILE_PATH, "a");

    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(file, "%d;%d;%s;%s\n", school.id, school.user_id, school.name, school.address);
    fclose(file);
}

int verify_user(int id) {
    FILE *file = fopen(FILE_PATH_USER, "r");
    User u;
    int temp;
    int found = 0;

    if (file != NULL) {
        while (fscanf(file, "%d;%[^;];%[^;];%[^;];%s\n", &temp, u.name, u.email, u.password, u.role) == 5) {
            if (temp == id) {
                found = 1;
                break;
            }
        }
        fclose(file);
    }

    return found;
}
