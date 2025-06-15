#include <stdio.h>
#include <string.h>

#include "../../include/models/user.h"
#include "../../include/use_cases/get_user.h"

#include "paths.h"


User get_user_by_id(int id) {
    FILE *file = fopen(USERS_FILE_PATH, "r");
    User user;
    memset(&user, 0, sizeof(User));
    user.id = -1;

    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return user;
    }

    int temp;
    User temp_user;
    while (fscanf(file, "%d;%[^;];%[^;];%[^;];%s\n", &temp, temp_user.name, temp_user.email, temp_user.password, temp_user.role) == 5) {
        if (temp == id) {
            temp_user.id = temp;
            user = temp_user;
            break;
        }
    }

    fclose(file);
    return user;
}
