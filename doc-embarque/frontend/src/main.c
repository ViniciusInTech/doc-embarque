#include "login_window.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    show_login_window();
    gtk_main();
    return 0;
}