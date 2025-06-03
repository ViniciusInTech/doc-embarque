#include <stdio.h>
#include <string.h>
#include "../../include/models/school.h"
#include "../../include/services/school_service.h"
#include "../../include/use_cases/create_school.h"

School create_school(School school) {
    school.id = get_next_school_id();
    School new_school = school;
    if (new_school.id == -1) {
        printf("Erro. A escola não foi cadastrada.\n");
        return new_school;
    }

    save_school(new_school);
    return new_school;
}

School create_school_cli() {
    School escola;

    printf("Nome da escola: ");
    fgets(escola.name, sizeof(escola.name), stdin);
    strtok(escola.name, "\n");

    printf("Endereço da escola: ");
    fgets(escola.address, sizeof(escola.address), stdin);
    strtok(escola.address, "\n");

    escola.user_id = 1;

    return create_school(escola);
}