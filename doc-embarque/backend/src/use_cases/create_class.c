#include <stdio.h>
#include <string.h>

#include "../../include/models/class.h"
#include "../../include/models/school.h"
#include "../../include/use_cases/create_class.h"

#include "paths.h"
#include "../../include/utils.h"

int get_next_class_id() {
    FILE *file = fopen(CLASS_FILE_PATH, "r");
    int id = 0, temp;
    Class c;

    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%d;%d\n", &temp, &c.school_id, c.name,
                      &c.students, &c.confirmed_students) == 5) {
            if (temp > id) {
                id = temp;
            }
        }
        fclose(file);
    }

    return id + 1;
}

int class_exists(int id) {
    FILE *file = fopen(CLASS_FILE_PATH, "r");
    Class c;
    int temp;
    int found = 0;

    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%d;%d\n", &temp, &c.school_id, c.name,
                      &c.students, &c.confirmed_students) == 5) {
            if (temp == id) {
                found = 1;
                break;
            }
        }
        fclose(file);
    }

    return found;
}

int verify_school(int id) {
    FILE *file = fopen(SCHOOLS_FILE_PATH, "r");
    School s;
    int temp;
    int found = 0;

    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%[^\n]\n", &temp, &s.user_id, s.name, s.address) == 4) {
            if (temp == id) {
                found = 1;
                break;
            }
        }
        fclose(file);
    }

    return found;
}

Class save_class(Class c) {
    ensure_data_directory();
    FILE *file = fopen(CLASS_FILE_PATH, "a");
    c.class_id = get_next_class_id();

    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        c.class_id = -1;
        return c;
    }

    fprintf(file, "%d;%d;%s;%d;%d\n", c.class_id, c.school_id, c.name,
            c.students, c.confirmed_students);
    fclose(file);
    return c;
}

Class create_class_cli() {
    Class c;

    printf("ID da escola: ");
    scanf("%d", &c.school_id);
    getchar();

    if (!verify_school(c.school_id)) {
        printf("Escola com ID %d não encontrada.\n", c.school_id);
        c.class_id = -1;
        return c;
    }

    printf("Nome da turma: ");
    fgets(c.name, sizeof(c.name), stdin);
    strtok(c.name, "\n");

    printf("Total de alunos: ");
    scanf("%d", &c.students);
    getchar();

    printf("Alunos confirmados: ");
    scanf("%d", &c.confirmed_students);
    getchar();

    c = save_class(c);
    if (c.class_id != -1) {
        printf("turma salva com sucesso.\n");
    }
    return c;
}