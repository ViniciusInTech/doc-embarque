#include "login_window.h"
#include "register_dialog.h"
#include "../../backend/include/models/user.h"
#include "../../backend/include/use_cases/login.h"
#include <string.h>

typedef struct {
    GtkWidget *email_entry;
    GtkWidget *password_entry;
} LoginWidgets;

static void show_message(GtkWindow *parent, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(parent,
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static void on_login_clicked(GtkButton *button, gpointer user_data) {
    LoginWidgets *w = user_data;
    const char *email = gtk_entry_get_text(GTK_ENTRY(w->email_entry));
    const char *password = gtk_entry_get_text(GTK_ENTRY(w->password_entry));

    User logged;
    if (login(email, password, &logged)) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Bem-vindo, %s!", logged.name);
        show_message(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))), msg);
    } else {
        show_message(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))), "Falha no login");
    }
}

static void on_register_clicked(GtkButton *button, gpointer user_data) {
    GtkWindow *parent = GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button)));
    show_register_dialog(parent);
}

static void load_css(void) {
    GtkCssProvider *provider = gtk_css_provider_new();
    GError *err = NULL;
    gtk_css_provider_load_from_path(provider, "style.css", &err);
    if (err) {
        g_warning("%s", err->message);
        g_clear_error(&err);
    }
    GdkScreen *screen = gdk_screen_get_default();
    gtk_style_context_add_provider_for_screen(screen,
                                              GTK_STYLE_PROVIDER(provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

void show_login_window(void) {
    load_css();
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "DocEmbarque Login");
    gtk_window_set_default_size(GTK_WINDOW(window), 320, 220);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget *frame = gtk_frame_new(NULL);
    gtk_style_context_add_class(gtk_widget_get_style_context(frame), "login-card");
    gtk_container_add(GTK_CONTAINER(box), frame);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(frame), grid);

    GtkWidget *title_label = gtk_label_new("DocEmbarque");
    gtk_widget_set_halign(title_label, GTK_ALIGN_CENTER);

    GtkWidget *email_label = gtk_label_new("Email:");
    GtkWidget *email_entry = gtk_entry_new();
    GtkWidget *password_label = gtk_label_new("Senha:");
    GtkWidget *password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);

    GtkWidget *login_button = gtk_button_new_with_label("Login");
    GtkWidget *register_button = gtk_button_new_with_label("Cadastrar");
    gtk_widget_set_hexpand(login_button, TRUE);
    gtk_widget_set_hexpand(register_button, TRUE);

    gtk_grid_attach(GTK_GRID(grid), title_label, 0,0,2,1);
    gtk_grid_attach(GTK_GRID(grid), email_label, 0,1,1,1);
    gtk_grid_attach(GTK_GRID(grid), email_entry, 1,1,1,1);
    gtk_grid_attach(GTK_GRID(grid), password_label, 0,2,1,1);
    gtk_grid_attach(GTK_GRID(grid), password_entry, 1,2,1,1);
    gtk_grid_attach(GTK_GRID(grid), login_button, 0,3,2,1);
    gtk_grid_attach(GTK_GRID(grid), register_button, 0,4,2,1);

    LoginWidgets *widgets = g_malloc(sizeof(LoginWidgets));
    widgets->email_entry = email_entry;
    widgets->password_entry = password_entry;

    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_clicked), widgets);
    g_signal_connect(register_button, "clicked", G_CALLBACK(on_register_clicked), widgets);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
}