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
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_widget_set_size_request(window, 467, 320);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_valign(left_box, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(left_box, GTK_ALIGN_FILL);
    gtk_widget_set_hexpand(left_box, TRUE);
    gtk_widget_set_vexpand(left_box, TRUE);
    gtk_widget_set_size_request(left_box, 187, -1);

    GtkWidget *image = gtk_image_new_from_file("assets/login_imagem.svg");
    gtk_widget_set_size_request(image, 133, 133);
    gtk_box_pack_start(GTK_BOX(left_box), image, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), left_box, TRUE, TRUE, 0);

    GtkWidget *form_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_valign(form_box, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(form_box, GTK_ALIGN_FILL);
    gtk_widget_set_hexpand(form_box, TRUE);
    gtk_widget_set_vexpand(form_box, TRUE);
    gtk_widget_set_size_request(form_box, 280, -1);
    gtk_widget_set_margin_top(form_box, 40);
    gtk_widget_set_margin_bottom(form_box, 40);
    gtk_widget_set_margin_start(form_box, 40);
    gtk_widget_set_margin_end(form_box, 40);
    gtk_box_pack_start(GTK_BOX(main_box), form_box, TRUE, TRUE, 0);

    GtkWidget *title_label = gtk_label_new("LOGIN");
    gtk_style_context_add_class(gtk_widget_get_style_context(title_label), "title");
    gtk_widget_set_halign(title_label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(form_box), title_label, FALSE, FALSE, 0);

    GtkWidget *email_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(email_entry), "Email");
    GtkWidget *password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Senha");
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);

    GtkWidget *login_button = gtk_button_new_with_label("Entrar");
    gtk_widget_set_hexpand(login_button, TRUE);

    GtkWidget *signup_label = gtk_label_new("Não tem uma conta?");
    GtkWidget *register_button = gtk_button_new_with_label("Cadastrar");

    gtk_box_pack_start(GTK_BOX(form_box), email_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form_box), password_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form_box), login_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form_box), signup_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(form_box), register_button, FALSE, FALSE, 0);

    LoginWidgets *widgets = g_malloc(sizeof(LoginWidgets));
    widgets->email_entry = email_entry;
    widgets->password_entry = password_entry;

    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_clicked), widgets);
    g_signal_connect(register_button, "clicked", G_CALLBACK(on_register_clicked), widgets);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
}