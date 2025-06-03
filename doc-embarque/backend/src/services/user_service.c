#include <stdio.h>
#include <string.h>

#ifdef _WIN32
    #include <direct.h>
#else
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

#include "../../include/models/user.h"
#include "../../include/services/user_service.h"

#define DATA_DIR "../../data"
#define FILE_PATH "../../data/users.txt"

int get_next_user_id() {
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

void create_data_directory() {
#ifdef _WIN32
    _mkdir(DATA_DIR);
#else
    mkdir(DATA_DIR, 0777);
#endif
}

User save_user(User user) {
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
