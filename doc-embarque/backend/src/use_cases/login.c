#include <stdio.h>
#include <string.h>

#include "../../include/models/user.h"
#include "../../include/use_cases/login.h"

#include "paths.h"


int login(const char *email, const char *password, User *user) {
    FILE *file = fopen(USERS_FILE_PATH, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    User temp;
    int id;
    int result = 0;

    while (fscanf(file, "%d;%[^;];%[^;];%[^;];%s\n", &id, temp.name, temp.email, temp.password, temp.role) == 5) {
        if (strcmp(temp.email, email) == 0 && strcmp(temp.password, password) == 0) {
            temp.id = id;
            if (user) {
                *user = temp;
            }
            result = 1;
            break;
        }
    }

    fclose(file);
    return result;
}

int login_cli(User *user) {
    char email[100];
    char password[100];

    printf("Email: ");
    fgets(email, sizeof(email), stdin);
    strtok(email, "\n");

    printf("Senha: ");
    fgets(password, sizeof(password), stdin);
    strtok(password, "\n");

    return login(email, password, user);
}