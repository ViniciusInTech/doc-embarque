#include "home_window.h"
#include "dashboard_page.h"
#include "schools_page.h"
#include "classes_page.h"
#include "travels_page.h"
#include "payments_page.h"
#include "boarding_page.h"
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

static void on_logout_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *stack = GTK_WIDGET(user_data);
    gtk_stack_set_visible_child_name(GTK_STACK(stack), "login");
}

static void on_navigate_clicked(GtkButton *btn, gpointer user_data) {
    GtkStack *cstack = GTK_STACK(user_data);
    const char *page = g_object_get_data(G_OBJECT(btn), "target-page");
    gtk_stack_set_visible_child_name(cstack, page);
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

static GtkWidget *create_menu_button(const char *label_text, const char *icon_path) {
    GtkWidget *button = gtk_button_new();
    gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);
    gtk_widget_set_name(button, "menu-button");

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_halign(box, GTK_ALIGN_FILL);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_start(box, 12);
    gtk_widget_set_margin_end(box, 12);
    gtk_widget_set_margin_top(box, 6);
    gtk_widget_set_margin_bottom(box, 6);

    GError *error = NULL;
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_scale(icon_path, 24, 24, TRUE, &error);
    GtkWidget *icon = NULL;

    if (pixbuf) {
        icon = gtk_image_new_from_pixbuf(pixbuf);
        g_object_unref(pixbuf);
    } else {
        g_warning("Erro ao carregar ícone: %s. Usando fallback. Erro: %s", icon_path, error->message);
        g_clear_error(&error);
        icon = gtk_image_new_from_icon_name("image-missing", GTK_ICON_SIZE_BUTTON);
    }

    gtk_widget_set_valign(icon, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), icon, FALSE, FALSE, 0);

    GtkWidget *label = gtk_label_new(label_text);
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_widget_set_valign(label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(button), box);
    return button;
}

GtkWidget *build_home_ui(GtkWidget *stack) {
    load_css();

    GtkWidget *main_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_size_request(sidebar, 200, -1);
    gtk_style_context_add_class(gtk_widget_get_style_context(sidebar), "sidebar");

    GtkWidget *content_stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(content_stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_widget_set_vexpand(content_stack, TRUE);
    gtk_widget_set_hexpand(content_stack, TRUE);

    GtkWidget *dashboard_page = build_dashboard_ui();
    GtkWidget *schools_page = build_schools_ui();
    GtkWidget *classes_page = build_classes_ui();
    GtkWidget *travels_page = build_travels_ui();
    GtkWidget *payments_page = build_payments_ui();
    GtkWidget *boarding_page = build_boarding_ui();

    gtk_stack_add_named(GTK_STACK(content_stack), dashboard_page, "dashboard");
    gtk_stack_add_named(GTK_STACK(content_stack), schools_page, "schools");
    gtk_stack_add_named(GTK_STACK(content_stack), classes_page, "classes");
    gtk_stack_add_named(GTK_STACK(content_stack), travels_page, "travels");
    gtk_stack_add_named(GTK_STACK(content_stack), payments_page, "payments");
    gtk_stack_add_named(GTK_STACK(content_stack), boarding_page, "boarding");


    const struct {
        const char *label;
        const char *icon;
        const char *page;
    } menu_items[] = {
        {"Dashboard", "assets/dashboard.svg", "dashboard"},
        {"Escolas", "assets/home.svg", "schools"},
        {"Turmas", "assets/classes.svg", "classes"},
        {"Viagens", "assets/travel.svg", "travels"},
        {"Pagamentos", "assets/payment.svg", "payments"},
        {"Embarque", "assets/boarding.svg", "boarding"},
        {"Sair", "assets/logout.svg", "login"},
    };

    for (int i = 0; i < 7; i++) {
        GtkWidget *btn = create_menu_button(menu_items[i].label, menu_items[i].icon);

        if (g_strcmp0(menu_items[i].label, "Sair") == 0) {
            gtk_widget_set_name(btn, "menu-button logout");
            g_signal_connect(btn, "clicked", G_CALLBACK(on_logout_clicked), stack);
        } else {
            g_signal_connect(btn, "clicked", G_CALLBACK(on_navigate_clicked), content_stack);
            g_object_set_data(G_OBJECT(btn), "target-page", (gpointer)menu_items[i].page);
        }

        gtk_box_pack_start(GTK_BOX(sidebar), btn, FALSE, FALSE, 0);
    }

    gtk_box_pack_start(GTK_BOX(main_container), sidebar, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_container), content_stack, TRUE, TRUE, 0);

    return main_container;
}
