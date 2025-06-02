#include <stdio.h>
#include <string.h>
#include "include/models/school.h"
#include "include/services/school_service.h"

int main() {
    School escola;

    escola.user_id = 1;

    if (verify_user(escola.user_id)) {
        escola.id = get_next_school_id();

        printf("Nome da escola: ");
        fgets(escola.name, sizeof(escola.name), stdin);
        strtok(escola.name, "\n");

        printf("Endereco da escola: ");
        fgets(escola.address, sizeof(escola.address), stdin);
        strtok(escola.address, "\n");

        save_school(escola);
        printf("Escola cadastrada com sucesso!\n");
    }
    else {
        printf("ERRO. Usuario nao encontrado.\n");
    }

    return 0;
}