#include "register_window.h"
#include "login_window.h"
#include "../../backend/include/models/user.h"
#include "../../backend/include/use_cases/create_user.h"
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <string.h>

typedef struct {
    GtkWidget *entry;
    GtkImage *icon;
    GdkPixbuf *lock_closed;
    GdkPixbuf *lock_open;
    gboolean visible;
} PasswordToggle;

static void show_message(GtkWindow *parent, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(parent,
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static void on_save_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget **w = user_data;
    User u;
    g_strlcpy(u.name, gtk_entry_get_text(GTK_ENTRY(w[0])), sizeof(u.name));
    g_strlcpy(u.email, gtk_entry_get_text(GTK_ENTRY(w[1])), sizeof(u.email));
    g_strlcpy(u.password, gtk_entry_get_text(GTK_ENTRY(w[2])), sizeof(u.password));
    strcpy(u.role, "ADMIN");
    u = create_user(u);
    if (u.id != -1) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Usuário %s cadastrado!", u.name);
        show_message(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))), msg);
    } else {
        show_message(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))), "Erro ao cadastrar usuário");
    }
}

static void on_back_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(button));
    gtk_widget_hide(window);
    show_login_window();
}

static void on_toggle_password(GtkButton *button, gpointer user_data) {
    PasswordToggle *toggle = user_data;
    toggle->visible = !toggle->visible;
    gtk_entry_set_visibility(GTK_ENTRY(toggle->entry), toggle->visible);
    gtk_image_set_from_pixbuf(toggle->icon,
                              toggle->visible ? toggle->lock_open : toggle->lock_closed);
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

void show_register_window(void) {
    load_css();
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Cadastro de Usuário");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 470, 380);

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(left_box, 188, -1);
    gtk_widget_set_valign(left_box, GTK_ALIGN_CENTER);
    GtkWidget *image = gtk_image_new_from_file("assets/bem-vindo.svg");
    gtk_widget_set_size_request(image, 100, 100);
    gtk_box_pack_start(GTK_BOX(left_box), image, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), left_box, FALSE, FALSE, 0);

    GtkWidget *form_grid = gtk_grid_new();
    gtk_widget_set_size_request(form_grid, 282, -1);
    gtk_grid_set_row_spacing(GTK_GRID(form_grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(form_grid), 5);
    gtk_widget_set_margin_top(form_grid, 20);
    gtk_widget_set_margin_bottom(form_grid, 20);
    gtk_widget_set_margin_start(form_grid, 20);
    gtk_widget_set_margin_end(form_grid, 20);
    gtk_widget_set_valign(form_grid, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(form_grid, TRUE);
    gtk_box_pack_start(GTK_BOX(main_box), form_grid, TRUE, TRUE, 0);

    GtkWidget *title_label = gtk_label_new("CADASTRO");
    gtk_style_context_add_class(gtk_widget_get_style_context(title_label), "title");
    gtk_widget_set_halign(title_label, GTK_ALIGN_CENTER);
    gtk_grid_attach(GTK_GRID(form_grid), title_label, 0, 0, 2, 1);

    GdkPixbuf *user_pixbuf = gdk_pixbuf_new_from_file_at_scale("assets/user.svg", 16, 16, TRUE, NULL);
    GdkPixbuf *email_pixbuf = gdk_pixbuf_new_from_file_at_scale("assets/email.svg", 16, 16, TRUE, NULL);

    GtkWidget *name_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(name_box, "entry-with-icon");
    gtk_widget_set_halign(name_box, GTK_ALIGN_FILL);
    gtk_widget_set_hexpand(name_box, TRUE);
    GtkWidget *name_icon = gtk_image_new_from_pixbuf(user_pixbuf);
    gtk_widget_set_margin_start(name_icon, 8);
    gtk_widget_set_margin_end(name_icon, 4);
    gtk_widget_set_valign(name_icon, GTK_ALIGN_CENTER);
    GtkWidget *name_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(name_entry), "Nome");
    gtk_widget_set_valign(name_entry, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(name_entry, TRUE);
    gtk_entry_set_has_frame(GTK_ENTRY(name_entry), FALSE);
    GtkWidget *name_spacer = gtk_button_new();
    gtk_button_set_relief(GTK_BUTTON(name_spacer), GTK_RELIEF_NONE);
    gtk_widget_set_name(name_spacer, "icon-button");
    gtk_widget_set_size_request(name_spacer, 24, 24);
    gtk_box_pack_start(GTK_BOX(name_box), name_icon, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(name_box), name_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(name_box), name_spacer, FALSE, FALSE, 0);
    gtk_grid_attach(GTK_GRID(form_grid), name_box, 0, 1, 2, 1);

    GtkWidget *email_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(email_box, "entry-with-icon");
    gtk_widget_set_halign(email_box, GTK_ALIGN_FILL);
    gtk_widget_set_hexpand(email_box, TRUE);
    GtkWidget *email_icon = gtk_image_new_from_pixbuf(email_pixbuf);
    gtk_widget_set_margin_start(email_icon, 8);
    gtk_widget_set_margin_end(email_icon, 4);
    gtk_widget_set_valign(email_icon, GTK_ALIGN_CENTER);
    GtkWidget *email_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(email_entry), "Email");
    gtk_widget_set_valign(email_entry, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(email_entry, TRUE);
    gtk_entry_set_has_frame(GTK_ENTRY(email_entry), FALSE);
    GtkWidget *email_spacer = gtk_button_new();
    gtk_button_set_relief(GTK_BUTTON(email_spacer), GTK_RELIEF_NONE);
    gtk_widget_set_name(email_spacer, "icon-button");
    gtk_widget_set_size_request(email_spacer, 24, 24);
    gtk_box_pack_start(GTK_BOX(email_box), email_icon, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(email_box), email_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(email_box), email_spacer, FALSE, FALSE, 0);
    gtk_grid_attach(GTK_GRID(form_grid), email_box, 0, 2, 2, 1);

    GtkWidget *password_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(password_box, "entry-with-icon");
    gtk_widget_set_halign(password_box, GTK_ALIGN_FILL);
    gtk_widget_set_hexpand(password_box, TRUE);
    GtkWidget *password_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Senha");
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_widget_set_valign(password_entry, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(password_entry, TRUE);
    gtk_entry_set_has_frame(GTK_ENTRY(password_entry), FALSE);
    GdkPixbuf *lock_closed = gdk_pixbuf_new_from_file_at_scale("assets/lock-closed.svg", 16, 16, TRUE, NULL);
    GdkPixbuf *lock_open = gdk_pixbuf_new_from_file_at_scale("assets/lock-open.svg", 16, 16, TRUE, NULL);
    GtkWidget *lock_image = gtk_image_new_from_pixbuf(lock_closed);
    GtkWidget *lock_button = gtk_button_new();
    gtk_button_set_relief(GTK_BUTTON(lock_button), GTK_RELIEF_NONE);
    gtk_container_add(GTK_CONTAINER(lock_button), lock_image);
    gtk_widget_set_margin_start(lock_button, 8);
    gtk_widget_set_margin_end(lock_button, 4);
    gtk_widget_set_valign(lock_button, GTK_ALIGN_CENTER);
    gtk_widget_set_name(lock_button, "icon-button");
    PasswordToggle *toggle_data = g_malloc(sizeof(PasswordToggle));
    toggle_data->entry = password_entry;
    toggle_data->icon = GTK_IMAGE(lock_image);
    toggle_data->lock_closed = lock_closed;
    toggle_data->lock_open = lock_open;
    toggle_data->visible = FALSE;
    g_signal_connect(lock_button, "clicked", G_CALLBACK(on_toggle_password), toggle_data);
    gtk_box_pack_start(GTK_BOX(password_box), lock_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(password_box), password_entry, TRUE, TRUE, 0);
    gtk_grid_attach(GTK_GRID(form_grid), password_box, 0, 3, 2, 1);

    GtkWidget *save_button = gtk_button_new_with_label("Cadastrar");
    gtk_widget_set_hexpand(save_button, TRUE);
    gtk_grid_attach(GTK_GRID(form_grid), save_button, 0, 4, 2, 1);

    GtkWidget *back_button = gtk_button_new_with_label("Voltar ao Login");
    gtk_widget_set_hexpand(back_button, TRUE);
    gtk_grid_attach(GTK_GRID(form_grid), back_button, 0, 5, 2, 1);

    GtkWidget **widgets = g_malloc(sizeof(GtkWidget *) * 3);
    widgets[0] = name_entry;
    widgets[1] = email_entry;
    widgets[2] = password_entry;

    g_signal_connect(save_button, "clicked", G_CALLBACK(on_save_clicked), widgets);
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_clicked), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
}
