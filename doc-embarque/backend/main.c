#include <stdio.h>
#include "include/models/school.h"
#include "include/services/school_service.h"
#include "include/use_cases/create_school.h"

int main() {
    School escola;

    if (verify_user(1)) {
        escola = create_school_cli();
        printf("Escola '%s' cadastrada com ID %d.\n", escola.name, escola.id);
    }
    else {
        printf("Usuário não encontrado.\n");
    }

    return 0;
}