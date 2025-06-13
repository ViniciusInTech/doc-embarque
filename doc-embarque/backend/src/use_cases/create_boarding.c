#include <stdio.h>
#include <string.h>

#include "../../include/models/boarding.h"
#include "../../include/models/student.h"
#include "../../include/use_cases/create_boarding.h"
#include "../../include/utils.h"

#define FILE_PATH "../../data/boarding.txt"
#define STUDENT_FILE "../../data/students.txt"

int get_next_boarding_id() {
    FILE *file = fopen(FILE_PATH, "r");
    int id = 0, temp;
    Boarding b;
    if (file != NULL) {
        while (fscanf(file, "%d;%d;%[^;];%d\n", &temp, &b.student_id, b.date, &b.boarded) == 4) {
            if (temp > id) id = temp;
        }
        fclose(file);
    }
    return id + 1;
}

int verify_student_board(int id) {
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

Boarding save_boarding(Boarding b) {
    ensure_data_directory();
    FILE *file = fopen(FILE_PATH, "a");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return b;
    }
    fprintf(file, "%d;%d;%s;%d\n", b.id, b.student_id, b.date, b.boarded);
    fclose(file);
    return b;
}

Boarding create_boarding(Boarding b) {
    b.id = get_next_boarding_id();
    return save_boarding(b);
}

Boarding create_boarding_cli() {
    Boarding b;
    printf("ID do aluno: ");
    scanf("%d", &b.student_id);
    getchar();
    if (!verify_student_board(b.student_id)) {
        printf("Aluno com ID %d não encontrado.\n", b.student_id);
        b.id = -1;
        return b;
    }
    printf("Data do embarque: ");
    fgets(b.date, sizeof(b.date), stdin);
    strtok(b.date, "\n");
    b.boarded = 1;
    b = create_boarding(b);
    if (b.id != -1) printf("Embarque salvo com sucesso.\n");
    return b;
}
