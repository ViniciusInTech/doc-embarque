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
    gtk_widget_set_valign(sidebar, GTK_ALIGN_FILL);
    gtk_widget_set_halign(sidebar, GTK_ALIGN_FILL);

    const char *menu_items[][2] = {
        {"Dashboard", "assets/dashboard.svg"},
        {"Alunos", "assets/students.svg"},
        {"Escolas", "assets/home.svg"},
        {"Turmas", "assets/classes.svg"},
        {"Embarques", "assets/boarding.svg"},
        {"Contatos", "assets/contacts.svg"},
        {"Pagamentos", "assets/payment.svg"},
        {"Seguro Viagem", "assets/insurance.svg"},
        {"Sair", "assets/logout.svg"}
    };

    int num_items = sizeof(menu_items) / sizeof(menu_items[0]);
    for (int i = 0; i < num_items; i++) {
        GtkWidget *btn = create_menu_button(menu_items[i][0], menu_items[i][1]);
        gtk_box_pack_start(GTK_BOX(sidebar), btn, FALSE, FALSE, 0);

        if (g_strcmp0(menu_items[i][0], "Sair") == 0) {
            g_signal_connect(btn, "clicked", G_CALLBACK(on_logout_clicked), stack);
        }
    }

    GtkWidget *content_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_valign(content_area, GTK_ALIGN_FILL);
    gtk_widget_set_halign(content_area, GTK_ALIGN_FILL);
    gtk_widget_set_margin_top(content_area, 30);
    gtk_widget_set_margin_bottom(content_area, 30);
    gtk_widget_set_margin_start(content_area, 30);
    gtk_widget_set_margin_end(content_area, 30);

    GtkWidget *title = gtk_label_new("DocEmbarque");
    gtk_style_context_add_class(gtk_widget_get_style_context(title), "title");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(content_area), title, FALSE, FALSE, 0);

    GtkWidget *subtitle = gtk_label_new("Bem-vindo ao DocEmbarque\nGerencie excursões escolares com facilidade.");
    gtk_widget_set_halign(subtitle, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(content_area), subtitle, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(main_container), sidebar, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_container), content_area, TRUE, TRUE, 0);

    return main_container;
}
