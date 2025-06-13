#include <stdio.h>
#include <string.h>

#include "../../include/models/travel_insurance.h"
#include "../../include/models/student.h"
#include "../../include/use_cases/create_travel_insurance.h"
#include "../../include/utils.h"

#define FILE_PATH "../../data/travel_insurance.txt"
#define STUDENT_FILE "../../data/students.txt"

int get_next_travel_insurance_id() {
    FILE *file = fopen(FILE_PATH, "r");
    int id = 0, temp;
    TravelInsurance t;
    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%[^\n]\n", &temp, &t.student_id, t.policy_number, t.provider) == 4) {
            if (temp > id) id = temp;
        }
        fclose(file);
    }
    return id + 1;
}

int verify_student_insurance(int id) {
    FILE *file = fopen(STUDENT_FILE, "r");
    Student s; int temp; int found = 0;
    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%[^;];%[^;];%[^\n]\n", &temp, &s.classroom_id, s.name, s.rg, s.cpf, s.birth_date) == 6) {
            if (temp == id) { found = 1; break; }
        }
        fclose(file);
    }
    return found;
}

TravelInsurance save_travel_insurance(TravelInsurance t) {
    ensure_data_directory();
    FILE *file = fopen(FILE_PATH, "a");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return t;
    }
    fprintf(file, "%d;%d;%s;%s\n", t.id, t.student_id, t.policy_number, t.provider);
    fclose(file);
    return t;
}

TravelInsurance create_travel_insurance(TravelInsurance t) {
    t.id = get_next_travel_insurance_id();
    return save_travel_insurance(t);
}

TravelInsurance create_travel_insurance_cli() {
    TravelInsurance t;
    printf("ID do aluno: ");
    scanf("%d", &t.student_id);
    getchar();
    if (!verify_student_insurance(t.student_id)) {
        printf("Aluno com ID %d não encontrado.\n", t.student_id);
        t.id = -1;
        return t;
    }
    printf("Numero da apolice: ");
    fgets(t.policy_number, sizeof(t.policy_number), stdin);
    strtok(t.policy_number, "\n");
    printf("Seguradora: ");
    fgets(t.provider, sizeof(t.provider), stdin);
    strtok(t.provider, "\n");
    t = create_travel_insurance(t);
    if (t.id != -1) printf("Seguro salvo com sucesso.\n");
    return t;
}
