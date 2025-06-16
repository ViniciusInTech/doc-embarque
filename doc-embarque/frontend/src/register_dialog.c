#include "register_dialog.h"
#include "../../backend/include/models/user.h"
#include "../../backend/include/use_cases/create_user.h"
#include <string.h>

static void show_message(GtkWindow *parent, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(parent,
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void show_register_dialog(GtkWindow *parent) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Cadastro",
                                                    parent,
                                                    GTK_DIALOG_MODAL,
                                                    "_Cancelar", GTK_RESPONSE_CANCEL,
                                                    "_Salvar", GTK_RESPONSE_OK,
                                                    NULL);
    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *frame = gtk_frame_new(NULL);
    gtk_style_context_add_class(gtk_widget_get_style_context(frame), "login-card");
    gtk_widget_set_halign(frame, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(frame, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(content), frame);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_container_add(GTK_CONTAINER(frame), grid);

    GtkWidget *name_label = gtk_label_new("Nome:");
    GtkWidget *name_entry = gtk_entry_new();
    GtkWidget *email_label = gtk_label_new("Email:");
    GtkWidget *email_entry = gtk_entry_new();
    GtkWidget *password_label = gtk_label_new("Senha:");
    GtkWidget *password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);

    gtk_grid_attach(GTK_GRID(grid), name_label, 0,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid), email_label, 0,1,1,1);
    gtk_grid_attach(GTK_GRID(grid), email_entry, 1,1,1,1);
    gtk_grid_attach(GTK_GRID(grid), password_label, 0,2,1,1);
    gtk_grid_attach(GTK_GRID(grid), password_entry, 1,2,1,1);

    gtk_widget_show_all(dialog);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        User u;
        g_strlcpy(u.name, gtk_entry_get_text(GTK_ENTRY(name_entry)), sizeof(u.name));
        g_strlcpy(u.email, gtk_entry_get_text(GTK_ENTRY(email_entry)), sizeof(u.email));
        g_strlcpy(u.password, gtk_entry_get_text(GTK_ENTRY(password_entry)), sizeof(u.password));
        strcpy(u.role, "ADMIN");
        u = create_user(u);
        if (u.id != -1) {
            char buf[256];
            snprintf(buf, sizeof(buf), "Usuário %s cadastrado!", u.name);
            show_message(GTK_WINDOW(dialog), buf);
        } else {
            show_message(GTK_WINDOW(dialog), "Erro ao cadastrar usuário");
        }
    }
    gtk_widget_destroy(dialog);
}