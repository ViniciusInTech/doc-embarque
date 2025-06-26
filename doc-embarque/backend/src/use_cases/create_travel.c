#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../include/models/travel.h"
#include "../../include/models/student.h"
#include "../../include/models/payment.h"
#include "../../include/use_cases/create_travel.h"
#include "../../include/use_cases/create_payment.h"
#include "../../include/use_cases/read_student.h"
#include "../../include/utils.h"
#include "paths.h"

#define FILE_PATH "../../data/travels.txt"

int get_next_travel_id() {
    FILE *file = fopen(FILE_PATH, "r");
    int id = 0, temp; Travel t;
    if (file) {
        while (fscanf(file, "%d;%d;%[^;];%lf\n", &temp, &t.class_id, t.name, &t.base_cost) == 4) {
            if (temp > id) id = temp;
        }
        fclose(file);
    }
    return id + 1;
}

Travel save_travel(Travel t) {
    ensure_data_directory();
    FILE *file = fopen(FILE_PATH, "a");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return t;
    }
    fprintf(file, "%d;%d;%s;%.2lf\n", t.id, t.class_id, t.name, t.base_cost);
    fclose(file);
    return t;
}

Travel create_travel(Travel t) {
    t.id = get_next_travel_id();
    t = save_travel(t);

    Student *students = NULL;
    int count = load_students_by_class(t.class_id, &students);
    for (int i = 0; i < count; i++) {
        Payment p = {0};
        p.student_id = students[i].id;
        p.amount = t.base_cost;
        strcpy(p.method, "PENDENTE");
        create_payment(p);
    }
    free(students);
    return t;
}

Travel create_travel_cli() {
    Travel t;
    printf("ID da turma: ");
    scanf("%d", &t.class_id); getchar();
    printf("Descricao da viagem: ");
    fgets(t.name, sizeof(t.name), stdin); strtok(t.name, "\n");
    printf("Custo base: ");
    scanf("%lf", &t.base_cost); getchar();
    t = create_travel(t);
    if (t.id != -1) printf("Viagem criada com sucesso.\n");
    return t;
}