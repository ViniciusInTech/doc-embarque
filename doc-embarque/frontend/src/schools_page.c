#include "schools_page.h"
#include "../../backend/include/models/school.h"
#include "../../backend/include/use_cases/create_school.h"
#include "../../backend/include/use_cases/read_school.h"
#include "../../backend/include/use_cases/update_school.h"
#include "../../backend/include/use_cases/delete_school.h"
#include "session.h"
#include <gtk/gtk.h>
#include <stdlib.h>

typedef struct {
    School *school;
    GtkListBox *list;
    GtkWidget *name_entry;
    GtkWidget *addr_entry;
    GtkWidget *dialog;
} EditSchoolData;

static GtkWidget *form_box;
static GtkWidget *list_container;
static GtkWidget *list_box;
static GtkWidget *name_entry;
static GtkWidget *address_entry;
static GtkWidget *schools_stack;

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

static void show_school_page(GtkButton *button, gpointer user_data) {
    const gchar *page_name = (const gchar *)user_data;
    gtk_stack_set_visible_child_name(GTK_STACK(schools_stack), page_name);
    if (g_strcmp0(page_name, "list") == 0) {
        refresh_school_list(GTK_LIST_BOX(list_box));
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
    gtk_stack_set_visible_child_name(GTK_STACK(schools_stack), "list");
    refresh_school_list(GTK_LIST_BOX(list_box));
}

static void on_details_clicked(GtkButton *button, gpointer user_data) {
    School *s = g_object_get_data(G_OBJECT(button), "school");

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Detalhes da Escola",
        GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
        GTK_DIALOG_MODAL,
        "Fechar", GTK_RESPONSE_CLOSE,
        NULL
    );

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_margin_top(box, 16);
    gtk_widget_set_margin_bottom(box, 16);
    gtk_widget_set_margin_start(box, 16);
    gtk_widget_set_margin_end(box, 16);
    gtk_container_add(GTK_CONTAINER(content), box);

    GtkWidget *icon = gtk_image_new_from_icon_name("dialog-information", GTK_ICON_SIZE_DIALOG);
    gtk_widget_set_halign(icon, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), icon, FALSE, FALSE, 0);

    GtkWidget *name_label = gtk_label_new(NULL);
    gchar *name_markup = g_strdup_printf("<span size='large' weight='bold'>%s</span>", s->name);
    gtk_label_set_markup(GTK_LABEL(name_label), name_markup);
    g_free(name_markup);
    gtk_widget_set_halign(name_label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), name_label, FALSE, FALSE, 0);

    GtkWidget *address_label = gtk_label_new(NULL);
    gchar *address_markup = g_strdup_printf("<span size='medium'>%s</span>", s->address);
    gtk_label_set_markup(GTK_LABEL(address_label), address_markup);
    g_free(address_markup);
    gtk_widget_set_halign(address_label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), address_label, FALSE, FALSE, 0);

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static void on_save_edit_school(GtkButton *button, gpointer user_data) {
    EditSchoolData *data = (EditSchoolData *)user_data;

    School upd = {0};
    upd.id = data->school->id;
    g_strlcpy(upd.name, gtk_entry_get_text(GTK_ENTRY(data->name_entry)), sizeof(upd.name));
    g_strlcpy(upd.address, gtk_entry_get_text(GTK_ENTRY(data->addr_entry)), sizeof(upd.address));

    if (update_school(upd)) {
        show_message(GTK_WINDOW(data->dialog), "Escola atualizada.");
        refresh_school_list(data->list);
    } else {
        show_message(GTK_WINDOW(data->dialog), "Erro ao atualizar.");
    }

    gtk_widget_destroy(data->dialog);
    g_free(data);
}

static void on_edit_clicked(GtkButton *button, gpointer user_data) {
    School *s = g_object_get_data(G_OBJECT(button), "school");
    GtkListBox *list = GTK_LIST_BOX(user_data);

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Editar Escola",
        GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
        GTK_DIALOG_MODAL,
        NULL
    );

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_margin_top(box, 24);
    gtk_widget_set_margin_bottom(box, 24);
    gtk_widget_set_margin_start(box, 24);
    gtk_widget_set_margin_end(box, 24);
    gtk_container_add(GTK_CONTAINER(content), box);

    GtkWidget *title_label = gtk_label_new(NULL);
    gchar *markup = g_strdup_printf("<span size='x-large' weight='bold'>Editar Escola</span>");
    gtk_label_set_markup(GTK_LABEL(title_label), markup);
    g_free(markup);
    gtk_widget_set_halign(title_label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), title_label, FALSE, FALSE, 0);

    GtkWidget *form = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(form), 12);
    gtk_grid_set_column_spacing(GTK_GRID(form), 12);
    gtk_box_pack_start(GTK_BOX(box), form, FALSE, FALSE, 0);

    GtkWidget *name_label = gtk_label_new("Nome:");
    gtk_widget_set_halign(name_label, GTK_ALIGN_END);
    GtkWidget *name_entry_d = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(name_entry_d), s->name);
    gtk_entry_set_width_chars(GTK_ENTRY(name_entry_d), 30);

    GtkWidget *addr_label = gtk_label_new("Endereço:");
    gtk_widget_set_halign(addr_label, GTK_ALIGN_END);
    GtkWidget *addr_entry_d = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(addr_entry_d), s->address);
    gtk_entry_set_width_chars(GTK_ENTRY(addr_entry_d), 30);

    gtk_grid_attach(GTK_GRID(form), name_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(form), name_entry_d, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(form), addr_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(form), addr_entry_d, 1, 1, 1, 1);

    GtkWidget *btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_widget_set_halign(btn_box, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), btn_box, FALSE, FALSE, 0);

    GtkWidget *cancel_btn = gtk_button_new_with_label("❌ Cancelar");
    GtkWidget *save_btn = gtk_button_new_with_label("💾 Salvar");

    gtk_box_pack_start(GTK_BOX(btn_box), cancel_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(btn_box), save_btn, FALSE, FALSE, 0);

    g_signal_connect_swapped(cancel_btn, "clicked", G_CALLBACK(gtk_widget_destroy), dialog);

    EditSchoolData *data = g_malloc(sizeof(EditSchoolData));
    data->school = s;
    data->list = list;
    data->name_entry = name_entry_d;
    data->addr_entry = addr_entry_d;
    data->dialog = dialog;

    g_signal_connect(save_btn, "clicked", G_CALLBACK(on_save_edit_school), data);

    gtk_widget_show_all(dialog);
}

typedef struct {
    GtkWidget *dialog;
    GtkListBox *list;
    School *school;
} DeleteSchoolData;

static void on_confirm_delete(GtkButton *button, gpointer user_data) {
    DeleteSchoolData *data = (DeleteSchoolData *)user_data;

    if (delete_school(data->school->id)) {
        refresh_school_list(data->list);
        show_message(GTK_WINDOW(data->dialog), "Escola removida.");
    } else {
        show_message(GTK_WINDOW(data->dialog), "Escola não encontrada.");
    }

    gtk_widget_destroy(data->dialog);
    g_free(data);
}

static void on_delete_clicked(GtkButton *button, gpointer user_data) {
    School *s = g_object_get_data(G_OBJECT(button), "school");
    GtkListBox *list = GTK_LIST_BOX(user_data);

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Remover Escola",
        GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
        GTK_DIALOG_MODAL,
        NULL
    );

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_margin_top(box, 24);
    gtk_widget_set_margin_bottom(box, 24);
    gtk_widget_set_margin_start(box, 24);
    gtk_widget_set_margin_end(box, 24);
    gtk_container_add(GTK_CONTAINER(content), box);

    GtkWidget *icon = gtk_image_new_from_icon_name("dialog-question", GTK_ICON_SIZE_DIALOG);
    gtk_widget_set_halign(icon, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), icon, FALSE, FALSE, 0);

    char question[128];
    snprintf(question, sizeof(question), "Remover escola '%s'?", s->name);
    GtkWidget *question_label = gtk_label_new(question);
    gtk_widget_set_halign(question_label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), question_label, FALSE, FALSE, 0);

    GtkWidget *btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_widget_set_halign(btn_box, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), btn_box, FALSE, FALSE, 0);

    GtkWidget *cancel_btn = gtk_button_new_with_label("❌ Cancelar");
    GtkWidget *confirm_btn = gtk_button_new_with_label("🗑️ Confirmar");

    gtk_box_pack_start(GTK_BOX(btn_box), cancel_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(btn_box), confirm_btn, FALSE, FALSE, 0);

    g_signal_connect_swapped(cancel_btn, "clicked", G_CALLBACK(gtk_widget_destroy), dialog);

    DeleteSchoolData *data = g_malloc(sizeof(DeleteSchoolData));
    data->dialog = dialog;
    data->list = list;
    data->school = s;

    g_signal_connect(confirm_btn, "clicked", G_CALLBACK(on_confirm_delete), data);

    gtk_widget_show_all(dialog);
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

GtkWidget *build_schools_ui() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *card = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_name(card, "action-card");
    gtk_widget_set_margin_start(card, 16);
    gtk_widget_set_margin_end(card, 16);
    gtk_widget_set_halign(card, GTK_ALIGN_FILL);
    gtk_widget_set_valign(card, GTK_ALIGN_START);

    GtkWidget *actions = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_widget_set_margin_top(actions, 16);
    gtk_widget_set_halign(actions, GTK_ALIGN_START);

    GtkWidget *register_btn = gtk_button_new_with_label("📘  Cadastrar nova escola");
    GtkWidget *list_btn = gtk_button_new_with_label("📋  Listar escolas cadastradas");

    g_signal_connect(register_btn, "clicked", G_CALLBACK(show_school_page), "form");
    g_signal_connect(list_btn, "clicked", G_CALLBACK(show_school_page), "list");

    gtk_box_pack_start(GTK_BOX(actions), register_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(actions), list_btn, FALSE, FALSE, 0);

    schools_stack = gtk_stack_new();

    GtkWidget *form_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(form_grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(form_grid), 8);

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

    GtkWidget *form_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_box_pack_start(GTK_BOX(form_container), form_grid, FALSE, FALSE, 0);

    list_container = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(list_container), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    list_box = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(list_container), list_box);

    gtk_stack_add_titled(GTK_STACK(schools_stack), list_container, "list", "Listagem");
    gtk_stack_add_titled(GTK_STACK(schools_stack), form_container, "form", "Cadastro");

    gtk_box_pack_start(GTK_BOX(card), actions, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(card), schools_stack, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), card, FALSE, FALSE, 0);

    gtk_stack_set_visible_child_name(GTK_STACK(schools_stack), "list");
    refresh_school_list(GTK_LIST_BOX(list_box));

    return box;
}