#include "schools_page.h"
#include "../../backend/include/models/school.h"
#include "../../backend/include/use_cases/create_school.h"
#include "session.h"
#include <gtk/gtk.h>

static GtkWidget *form_box;
static GtkWidget *name_entry;
static GtkWidget *address_entry;

static void show_message(GtkWindow *parent, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(parent,
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "%s",
                                               message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static void on_toggle_form(GtkButton *button, gpointer user_data) {
    if (gtk_widget_get_visible(form_box)) {
        gtk_widget_hide(form_box);
    } else {
        gtk_widget_show_all(form_box);
    }
}

static void on_save_school(GtkButton *button, gpointer user_data) {
    const char *name_text = gtk_entry_get_text(GTK_ENTRY(name_entry));
    const char *addr_text = gtk_entry_get_text(GTK_ENTRY(address_entry));

    if (name_text[0] == '\0' || addr_text[0] == '\0') {
        show_message(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                    "Preencha todos os campos.");
        return;
    }

    if (!is_logged_in()) {
        show_message(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                    "É preciso estar logado.");
        return;
    }

    School s;
    s.user_id = get_current_user().id;
    g_strlcpy(s.name, name_text, sizeof(s.name));
    g_strlcpy(s.address, addr_text, sizeof(s.address));

    s = create_school(s);
    char msg[128];
    snprintf(msg, sizeof(msg), "Escola '%s' cadastrada!", s.name);
    show_message(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))), msg);

    gtk_entry_set_text(GTK_ENTRY(name_entry), "");
    gtk_entry_set_text(GTK_ENTRY(address_entry), "");
    gtk_widget_hide(form_box);
}

GtkWidget *build_schools_ui() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    gtk_widget_set_margin_top(box, 0);
    gtk_widget_set_margin_bottom(box, 0);
    gtk_widget_set_margin_start(box, 0);
    gtk_widget_set_margin_end(box, 0);

    GtkWidget *card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_name(card, "action-card");
    gtk_widget_set_margin_top(card, 0);
    gtk_widget_set_margin_bottom(card, 0);
    gtk_widget_set_margin_start(card, 16);
    gtk_widget_set_margin_end(card, 16);
    gtk_widget_set_halign(card, GTK_ALIGN_FILL);
    gtk_widget_set_valign(card, GTK_ALIGN_START);

    GtkWidget *actions = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_widget_set_margin_top(actions, 16);
    gtk_widget_set_halign(actions, GTK_ALIGN_START);

    const char *labels[] = {
        "📘  Cadastrar nova escola",
        "📋  Listar escolas cadastradas",
        "✏️  Editar informações de escola",
        "🗑️  Deletar escola"
    };

    GtkWidget *buttons[4];
    for (int i = 0; i < 4; i++) {
        GtkWidget *label = gtk_label_new(labels[i]);

        GtkWidget *btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
        gtk_box_pack_start(GTK_BOX(btn_box), label, FALSE, FALSE, 0);

        GtkWidget *btn = gtk_button_new();
        gtk_container_add(GTK_CONTAINER(btn), btn_box);
        gtk_widget_set_name(btn, "action-button");

        gtk_box_pack_start(GTK_BOX(actions), btn, FALSE, FALSE, 0);
        buttons[i] = btn;
    }

    g_signal_connect(buttons[0], "clicked", G_CALLBACK(on_toggle_form), NULL);

    form_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_margin_top(form_box, 16);
    gtk_widget_set_margin_bottom(form_box, 0);
    gtk_widget_set_margin_start(form_box, 16);
    gtk_widget_set_margin_end(form_box, 16);

    GtkWidget *form_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(form_grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(form_grid), 8);
    gtk_box_pack_start(GTK_BOX(form_box), form_grid, FALSE, FALSE, 0);

    GtkWidget *name_label = gtk_label_new("Nome da escola:");
    name_entry = gtk_entry_new();
    gtk_entry_set_width_chars(GTK_ENTRY(name_entry), 20);

    GtkWidget *addr_label = gtk_label_new("Endereço:");
    address_entry = gtk_entry_new();
    gtk_entry_set_width_chars(GTK_ENTRY(address_entry), 40);

    gtk_grid_attach(GTK_GRID(form_grid), name_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(form_grid), name_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(form_grid), addr_label, 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(form_grid), address_entry, 3, 0, 1, 1);

    GtkWidget *save_btn = gtk_button_new_with_label("Salvar");
    gtk_grid_attach(GTK_GRID(form_grid), save_btn, 0, 1, 4, 1);
    g_signal_connect(save_btn, "clicked", G_CALLBACK(on_save_school), NULL);

    gtk_widget_hide(form_box);

    gtk_box_pack_start(GTK_BOX(card), actions, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(card), form_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), card, FALSE, FALSE, 0);

    GtkWidget *label = gtk_label_new("Selecione uma ação para gerenciar escolas.");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_widget_set_margin_top(label, 20);
    gtk_widget_set_margin_start(label, 16);
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);

    return box;
}