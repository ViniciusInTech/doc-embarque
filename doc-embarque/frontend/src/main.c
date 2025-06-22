#include <gtk/gtk.h>
#include "login_window.h"
#include "register_window.h"
#include "home_window.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    GtkSettings *settings = gtk_settings_get_default();
    g_object_set(settings, "gtk-application-prefer-dark-theme", TRUE, NULL);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "DocEmbarque");
    gtk_window_set_default_size(GTK_WINDOW(window), 470, 320);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget *stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_stack_set_transition_duration(GTK_STACK(stack), 300);
    gtk_container_add(GTK_CONTAINER(window), stack);

    GtkWidget *login_ui = build_login_ui(stack);
    GtkWidget *register_ui = build_register_ui(stack);
    GtkWidget *home_ui = build_home_ui(stack);

    gtk_stack_add_named(GTK_STACK(stack), login_ui, "login");
    gtk_stack_add_named(GTK_STACK(stack), register_ui, "register");
    gtk_stack_add_named(GTK_STACK(stack), home_ui, "home");
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "login");

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
