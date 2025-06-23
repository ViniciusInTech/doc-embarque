#include "schools_page.h"
#include "../../backend/include/models/school.h"
#include "../../backend/include/use_cases/create_school.h"
#include "../../backend/include/use_cases/read_school.h"
#include "../../backend/include/use_cases/update_school.h"
#include "../../backend/include/use_cases/delete_school.h"
#include "session.h"
#include <gtk/gtk.h>
#include <stdlib.h>

static GtkWidget *form_box;
static GtkWidget *list_container;
static GtkWidget *list_box;
static GtkWidget *name_entry;
static GtkWidget *address_entry;

static void refresh_school_list(GtkListBox *list);

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

static void show_form_only(void) {
    gtk_widget_hide(list_container);
    gtk_widget_show_all(form_box);
}

static void show_list_only(void) {
    refresh_school_list(GTK_LIST_BOX(list_box));
    gtk_widget_hide(form_box);
    gtk_widget_show_all(list_container);
}

static void on_toggle_form(GtkButton *button, gpointer user_data) {
    show_form_only();
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



static void on_details_clicked(GtkButton *button, gpointer user_data) {
    School *s = g_object_get_data(G_OBJECT(button), "school");
    char msg[256];
    snprintf(msg, sizeof(msg), "ID: %d\nNome: %s\nEndereço: %s", s->id, s->name, s->address);
    show_message(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))), msg);
}

static void on_edit_clicked(GtkButton *button, gpointer user_data) {
    School *s = g_object_get_data(G_OBJECT(button), "school");
    GtkListBox *list = GTK_LIST_BOX(user_data);

    GtkWidget *dialog = gtk_dialog_new_with_buttons("Editar Escola",
        GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
        GTK_DIALOG_MODAL,
        "Cancelar", GTK_RESPONSE_CANCEL,
        "Salvar", GTK_RESPONSE_OK,
        NULL);

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 8);
    gtk_container_add(GTK_CONTAINER(content), grid);

    GtkWidget *name_entry_d = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(name_entry_d), s->name);
    GtkWidget *addr_entry_d = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(addr_entry_d), s->address);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Nome:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry_d, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Endereço:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), addr_entry_d, 1, 1, 1, 1);

    gtk_widget_show_all(dialog);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        School upd = {0};
        upd.id = s->id;
        g_strlcpy(upd.name, gtk_entry_get_text(GTK_ENTRY(name_entry_d)), sizeof(upd.name));
        g_strlcpy(upd.address, gtk_entry_get_text(GTK_ENTRY(addr_entry_d)), sizeof(upd.address));
        if (update_school(upd)) {
            show_message(GTK_WINDOW(dialog), "Escola atualizada.");
            refresh_school_list(list);
        } else {
            show_message(GTK_WINDOW(dialog), "Erro ao atualizar.");
        }
    }
    gtk_widget_destroy(dialog);
}

static void on_delete_clicked(GtkButton *button, gpointer user_data) {
    School *s = g_object_get_data(G_OBJECT(button), "school");
    GtkListBox *list = GTK_LIST_BOX(user_data);

    char question[128];
    snprintf(question, sizeof(question), "Remover escola '%s'?", s->name);
    GtkWidget *confirm = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_QUESTION,
                                               GTK_BUTTONS_YES_NO,
                                               "%s", question);
    if (gtk_dialog_run(GTK_DIALOG(confirm)) == GTK_RESPONSE_YES) {
        if (delete_school(s->id)) {
            show_message(GTK_WINDOW(confirm), "Escola removida.");
            refresh_school_list(list);
        } else {
            show_message(GTK_WINDOW(confirm), "Escola não encontrada.");
        }
    }
    gtk_widget_destroy(confirm);
}

static void refresh_school_list(GtkListBox *list) {
    GList *children = gtk_container_get_children(GTK_CONTAINER(list));
    for (GList *c = children; c != NULL; c = c->next) {
        gtk_widget_destroy(GTK_WIDGET(c->data));
    }
    g_list_free(children);

    School *schools = NULL;
    int count = load_schools(&schools);
    for (int i = 0; i < count; i++) {
        GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
        char idbuf[16];
        snprintf(idbuf, sizeof(idbuf), "%d", schools[i].id);
        gtk_box_pack_start(GTK_BOX(row), gtk_label_new(idbuf), FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(row), gtk_label_new(schools[i].name), TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(row), gtk_label_new(schools[i].address), TRUE, TRUE, 0);

        GtkWidget *detail_btn = gtk_button_new_with_label("Detalhes");
        GtkWidget *edit_btn = gtk_button_new_with_label("Editar");
        GtkWidget *del_btn = gtk_button_new_with_label("Deletar");

        School *copy = g_memdup(&schools[i], sizeof(School));
        g_object_set_data_full(G_OBJECT(detail_btn), "school", copy, g_free);
        g_object_set_data(G_OBJECT(edit_btn), "school", copy);
        g_object_set_data(G_OBJECT(del_btn), "school", copy);

        g_signal_connect(detail_btn, "clicked", G_CALLBACK(on_details_clicked), NULL);
        g_signal_connect(edit_btn, "clicked", G_CALLBACK(on_edit_clicked), list);
        g_signal_connect(del_btn, "clicked", G_CALLBACK(on_delete_clicked), list);

        gtk_box_pack_start(GTK_BOX(row), detail_btn, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(row), edit_btn, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(row), del_btn, FALSE, FALSE, 0);

        GtkWidget *row_wrap = gtk_list_box_row_new();
        gtk_container_add(GTK_CONTAINER(row_wrap), row);
        gtk_container_add(GTK_CONTAINER(list), row_wrap);
    }
    free(schools);
    gtk_widget_show_all(GTK_WIDGET(list));
}

static void on_list_schools(GtkButton *button, gpointer user_data) {
    (void)button;
    show_list_only();
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
        "📋  Listar escolas cadastradas"
    };

    GtkWidget *buttons[2];
    for (int i = 0; i < 2; i++) {
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
    g_signal_connect(buttons[1], "clicked", G_CALLBACK(on_list_schools), NULL);

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

    list_container = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(list_container), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_margin_top(list_container, 16);
    gtk_widget_set_margin_bottom(list_container, 0);
    gtk_widget_set_margin_start(list_container, 16);
    gtk_widget_set_margin_end(list_container, 16);

    list_box = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(list_container), list_box);
    gtk_widget_set_size_request(list_container, -1, 200);
    gtk_widget_hide(list_container);

    gtk_box_pack_start(GTK_BOX(card), actions, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(card), form_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(card), list_container, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), card, FALSE, FALSE, 0);

    GtkWidget *label = gtk_label_new("Selecione uma ação para gerenciar escolas.");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_widget_set_margin_top(label, 20);
    gtk_widget_set_margin_start(label, 16);
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);

    return box;
}