#include <stdio.h>
#include <string.h>

#ifdef _WIN32
    #include <direct.h>
#else
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

#include "../../include/models/user.h"
#include "../../include/use_cases/create_user.h"

#define DATA_DIR "../../data"
#define FILE_PATH "../../data/users.txt"

static void create_data_directory() {
#ifdef _WIN32
    _mkdir(DATA_DIR);
#else
    mkdir(DATA_DIR, 0777);
#endif
}

static int get_next_user_id() {
    FILE *file = fopen(FILE_PATH, "r");
    int id = 0, temp;
    User u;

    if (file != NULL) {
        while (fscanf(file, "%d;%[^;];%[^;];%[^;];%s\n", &temp, u.name, u.email, u.password, u.role) == 5) {
            if (temp > id) id = temp;
        }
        fclose(file);
    }

    return id + 1;
}

static User save_user(User user) {
    create_data_directory();

    FILE *file = fopen(FILE_PATH, "a");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        user.id = -1;
        return user;
    }

    fprintf(file, "%d;%s;%s;%s;%s\n", user.id, user.name, user.email, user.password, user.role);
    fclose(file);
    return user;
}

User create_user(User user) {
    user.id = get_next_user_id();
    return save_user(user);
}

User create_user_cli() {
    User novo;

    printf("Nome: ");
    fgets(novo.name, sizeof(novo.name), stdin);
    strtok(novo.name, "\n");

    printf("Email: ");
    fgets(novo.email, sizeof(novo.email), stdin);
    strtok(novo.email, "\n");

    printf("Senha: ");
    fgets(novo.password, sizeof(novo.password), stdin);
    strtok(novo.password, "\n");

    strcpy(novo.role, "ADMIN");

    return create_user(novo);
}
