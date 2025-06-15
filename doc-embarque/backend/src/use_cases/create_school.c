#include <stdio.h>
#include <string.h>
#include "../../include/models/school.h"
#include "../../include/use_cases/create_school.h"

#include "paths.h"
#include "../../include/utils.h"
#include "../../include/models/user.h"

int get_next_school_id() {
    FILE *file = fopen(SCHOOLS_FILE_PATH, "r");
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

int school_exists(int id) {
    FILE *file = fopen(SCHOOLS_FILE_PATH, "r");
    School s;
    int temp;
    int found = 0;

    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%s\n", &temp, &s.user_id, s.name, s.address) == 4) {
            if (temp == id) {
                found = 1;
                break;
            }
        }
        fclose(file);
    }

    return found;
}

School save_school(School school) {
    ensure_data_directory();
    FILE *file = fopen(SCHOOLS_FILE_PATH, "a");

    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return school;
    }

    fprintf(file, "%d;%d;%s;%s\n", school.id, school.user_id, school.name, school.address);
    fclose(file);
    return school;
}

int verify_user(int id) {
    FILE *file = fopen(USERS_FILE_PATH, "r");
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

School create_school(School school) {
    school.id = get_next_school_id();
    return save_school(school);
}

School create_school_cli() {
    School escola;
    printf("ID do usuário: ");
    scanf("%d", &escola.user_id);
    getchar();

    if (!verify_user(escola.user_id)) {
        printf("Usuário com ID %d não encontrado.\n", escola.user_id);
        escola.user_id = -1;
        return escola;
    }

    printf("Nome da escola: ");
    fgets(escola.name, sizeof(escola.name), stdin);
    strtok(escola.name, "\n");

    printf("Endereço da escola: ");
    fgets(escola.address, sizeof(escola.address), stdin);
    strtok(escola.address, "\n");

    escola = save_school(escola);
    if (escola.user_id != -1) {
        printf("Escola salva com sucesso.\n");
    }

    return escola;
}