#include "home_window.h"
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

static void on_logout_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *stack = GTK_WIDGET(user_data);
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "login");
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

GtkWidget *build_home_ui(GtkWidget *stack) {
    load_css();

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

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

    GtkWidget *title_label = gtk_label_new("HOME");
    gtk_style_context_add_class(gtk_widget_get_style_context(title_label), "title");
    gtk_widget_set_halign(title_label, GTK_ALIGN_CENTER);
    gtk_grid_attach(GTK_GRID(form_grid), title_label, 0, 0, 2, 1);

    GtkWidget *welcome_label = gtk_label_new("Bem-vindo ao DocEmbarque!");
    gtk_widget_set_halign(welcome_label, GTK_ALIGN_START);
    gtk_grid_attach(GTK_GRID(form_grid), welcome_label, 0, 1, 2, 1);

    GtkWidget *logout_button = gtk_button_new_with_label("Sair");
    gtk_widget_set_hexpand(logout_button, TRUE);
    gtk_grid_attach(GTK_GRID(form_grid), logout_button, 0, 2, 2, 1);

    g_signal_connect(logout_button, "clicked", G_CALLBACK(on_logout_clicked), stack);

    return main_box;
}