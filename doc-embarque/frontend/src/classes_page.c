#include "classes_page.h"
#include "../../backend/include/models/class.h"
#include "../../backend/include/models/student.h"
#include "../../backend/include/use_cases/create_class.h"
#include "../../backend/include/use_cases/read_class.h"
#include "../../backend/include/use_cases/update_class.h"
#include "../../backend/include/use_cases/delete_class.h"
#include "../../backend/include/models/school.h"
#include "../../backend/include/use_cases/read_school.h"
#include "../../backend/include/use_cases/create_student.h"
#include "../../backend/include/use_cases/read_student.h"
#include "../../backend/include/use_cases/delete_student.h"
#include "session.h"
#include <gtk/gtk.h>
#include <stdlib.h>

static int current_page = 1;
static int items_per_page = 10;
static GtkWidget *prev_button;
static GtkWidget *next_button;
static GtkWidget *page_label;

typedef struct {
    int class_id;
    GtkListBox *list;
    GtkWidget *name_entry;
    GtkWidget *rg_entry;
    GtkWidget *cpf_entry;
    GtkWidget *birth_entry;
} StudentManagerData;

typedef struct {
    Class *class;
    GtkListBox *list;
    GtkWidget *name_entry;
    GtkWidget *school_combo;
    GtkWidget *dialog;
} EditClassData;

typedef struct {
    GtkWidget *dialog;
    GtkListBox *list;
    Class *class;
} DeleteClassData;

static GtkWidget *name_entry;
static GtkWidget *school_combo;
static GtkWidget *school_search_entry;
static School *schools = NULL;
static int school_count = 0;
static GtkWidget *class_stack;
static GtkWidget *class_list_box;
static GtkWidget *filter_search_entry;
static GtkWidget *filter_combo;
static int filter_school_id = -1;

static void refresh_class_list(GtkListBox *list);
static void refresh_student_list(StudentManagerData *data);
static void show_class_page(GtkButton *button, gpointer user_data);
static void refresh_school_combo(GtkComboBoxText *combo, const char *filter, gboolean include_all);
static void on_school_search_changed(GtkEditable *entry, gpointer user_data);
static void on_filter_search_changed(GtkEditable *entry, gpointer user_data);
static void on_filter_changed(GtkComboBox *combo, gpointer user_data);
void classes_page_refresh(void);

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

static void on_save_student(GtkButton *button, gpointer user_data) {
    StudentManagerData *data = user_data;
    const char *name = gtk_entry_get_text(GTK_ENTRY(data->name_entry));
    const char *rg = gtk_entry_get_text(GTK_ENTRY(data->rg_entry));
    const char *cpf = gtk_entry_get_text(GTK_ENTRY(data->cpf_entry));
    const char *birth = gtk_entry_get_text(GTK_ENTRY(data->birth_entry));

    if (!*name || !*rg || !*cpf || !*birth) {
        show_message(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                     "Preencha todos os campos.");
        return;
    }

    Student s = {0};
    s.classroom_id = data->class_id;
    g_strlcpy(s.name, name, sizeof(s.name));
    g_strlcpy(s.rg, rg, sizeof(s.rg));
    g_strlcpy(s.cpf, cpf, sizeof(s.cpf));
    g_strlcpy(s.birth_date, birth, sizeof(s.birth_date));

    create_student(s);
    gtk_entry_set_text(GTK_ENTRY(data->name_entry), "");
    gtk_entry_set_text(GTK_ENTRY(data->rg_entry), "");
    gtk_entry_set_text(GTK_ENTRY(data->cpf_entry), "");
    gtk_entry_set_text(GTK_ENTRY(data->birth_entry), "");
    refresh_student_list(data);
}

static void on_delete_student(GtkButton *button, gpointer user_data) {
    StudentManagerData *data = user_data;
    Student *s = g_object_get_data(G_OBJECT(button), "student");
    if (delete_student(s->id)) {
        refresh_student_list(data);
    }
}

static void refresh_student_list(StudentManagerData *data) {
    if (!GTK_IS_WIDGET(data->list)) {
        return;
    }

    GList *children = gtk_container_get_children(GTK_CONTAINER(data->list));
    for (GList *c = children; c; c = c->next) {
        gtk_widget_destroy(GTK_WIDGET(c->data));
    }
    g_list_free(children);

    Student *students = NULL;
    int count = load_students_by_class(data->class_id, &students);
    for (int i = 0; i < count; i++) {
        GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);

        char idbuf[16];
        snprintf(idbuf, sizeof(idbuf), "%d", students[i].id);
        gtk_box_pack_start(GTK_BOX(row), gtk_label_new(idbuf), FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(row), gtk_label_new(students[i].name), TRUE, TRUE, 0);

        GtkWidget *del_btn = gtk_button_new_with_label("Deletar");
        Student *copy = g_memdup2(&students[i], sizeof(Student));
        g_object_set_data_full(G_OBJECT(del_btn), "student", copy, g_free);
        g_signal_connect(del_btn, "clicked", G_CALLBACK(on_delete_student), data);

        gtk_box_pack_start(GTK_BOX(row), del_btn, FALSE, FALSE, 0);

        GtkWidget *wrap = gtk_list_box_row_new();
        gtk_container_add(GTK_CONTAINER(wrap), row);
        gtk_container_add(GTK_CONTAINER(data->list), wrap);
    }
    free(students);

    if (GTK_IS_WIDGET(data->list)) {
        gtk_widget_show_all(GTK_WIDGET(data->list));
    }
}


static void reload_school_data(void) {
    if (schools) {
        free(schools);
        schools = NULL;
        school_count = 0;
    }
    school_count = load_schools(&schools);
}

static void refresh_school_combo(GtkComboBoxText *combo, const char *filter, gboolean include_all) {
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(combo));
    if (include_all) {
        gtk_combo_box_text_append(combo, "", "Todas");
    }
    for (int i = 0; i < school_count; i++) {
        const char *name = schools[i].name;
        if (!filter || g_strrstr(g_ascii_strdown(name, -1),
                                 g_ascii_strdown(filter, -1))) {
            char idbuf[16];
            snprintf(idbuf, sizeof(idbuf), "%d", schools[i].id);
            gtk_combo_box_text_append(combo, idbuf, name);
        }
    }
    if (gtk_tree_model_iter_n_children(gtk_combo_box_get_model(GTK_COMBO_BOX(combo)), NULL) > 0)
        gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
}

static void on_school_search_changed(GtkEditable *entry, gpointer user_data) {
    GtkComboBoxText *combo = GTK_COMBO_BOX_TEXT(user_data);
    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
    refresh_school_combo(combo, *text ? text : NULL, FALSE);
}

static void on_filter_search_changed(GtkEditable *entry, gpointer user_data) {
    GtkComboBoxText *combo = GTK_COMBO_BOX_TEXT(user_data);
    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
    refresh_school_combo(combo, *text ? text : NULL, TRUE);
}

static void on_filter_changed(GtkComboBox *combo, gpointer user_data) {
    const char *sid = gtk_combo_box_get_active_id(combo);
    if (sid && *sid) {
        filter_school_id = atoi(sid);
    } else {
        filter_school_id = -1;
    }
    refresh_class_list(GTK_LIST_BOX(class_list_box));
}

static void show_class_page(GtkButton *button, gpointer user_data) {
    const gchar *page_name = (const gchar *)user_data;
    gtk_stack_set_visible_child_name(GTK_STACK(class_stack), page_name);
    if (g_strcmp0(page_name, "list") == 0) {
        refresh_class_list(GTK_LIST_BOX(class_list_box));
    } else if (g_strcmp0(page_name, "form") == 0) {
        reload_school_data();
        refresh_school_combo(GTK_COMBO_BOX_TEXT(school_combo), NULL, FALSE);
    }
}

static void on_manage_students(GtkButton *button, gpointer user_data) {
    Class *c = g_object_get_data(G_OBJECT(button), "class");
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Alunos",
        GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
        GTK_DIALOG_MODAL,
        "Fechar", GTK_RESPONSE_CLOSE,
        NULL);

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_margin_top(box, 16);
    gtk_widget_set_margin_bottom(box, 16);
    gtk_widget_set_margin_start(box, 16);
    gtk_widget_set_margin_end(box, 16);
    gtk_container_add(GTK_CONTAINER(content), box);

    GtkWidget *list = gtk_list_box_new();
    gtk_widget_set_size_request(list, 400, 200);
    gtk_box_pack_start(GTK_BOX(box), list, TRUE, TRUE, 0);

    GtkWidget *form = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(form), 8);
    gtk_grid_set_column_spacing(GTK_GRID(form), 8);
    gtk_box_pack_start(GTK_BOX(box), form, FALSE, FALSE, 0);

    GtkWidget *name_l = gtk_label_new("Nome:");
    GtkWidget *name_e = gtk_entry_new();
    GtkWidget *rg_l = gtk_label_new("RG:");
    GtkWidget *rg_e = gtk_entry_new();
    GtkWidget *cpf_l = gtk_label_new("CPF:");
    GtkWidget *cpf_e = gtk_entry_new();
    GtkWidget *birth_l = gtk_label_new("Nascimento:");
    GtkWidget *birth_e = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(form), name_l, 0,0,1,1);
    gtk_grid_attach(GTK_GRID(form), name_e, 1,0,1,1);
    gtk_grid_attach(GTK_GRID(form), rg_l, 0,1,1,1);
    gtk_grid_attach(GTK_GRID(form), rg_e, 1,1,1,1);
    gtk_grid_attach(GTK_GRID(form), cpf_l, 0,2,1,1);
    gtk_grid_attach(GTK_GRID(form), cpf_e, 1,2,1,1);
    gtk_grid_attach(GTK_GRID(form), birth_l, 0,3,1,1);
    gtk_grid_attach(GTK_GRID(form), birth_e, 1,3,1,1);

    GtkWidget *save_btn = gtk_button_new_with_label("Adicionar");
    gtk_grid_attach(GTK_GRID(form), save_btn, 0,4,2,1);

    StudentManagerData *data = g_malloc(sizeof(StudentManagerData));
    data->class_id = c->class_id;
    data->list = GTK_LIST_BOX(list);
    data->name_entry = name_e;
    data->rg_entry = rg_e;
    data->cpf_entry = cpf_e;
    data->birth_entry = birth_e;

    g_signal_connect(save_btn, "clicked", G_CALLBACK(on_save_student), data);

    refresh_student_list(data);

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    //g_free(data);
}

static void on_save_class(GtkButton *button, gpointer user_data) {
    const char *name = gtk_entry_get_text(GTK_ENTRY(name_entry));
    const char *sid = gtk_combo_box_get_active_id(GTK_COMBO_BOX(school_combo));

    if (!*name || sid == NULL) {
        show_message(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                     "Preencha todos os campos.");
        return;
    }

    Class c = {0};
    c.school_id = atoi(sid);
    g_strlcpy(c.name, name, sizeof(c.name));
    c.students = 0;
    c.confirmed_students = 0;

    c = save_class(c);
    char msg[128];
    snprintf(msg, sizeof(msg), "Turma '%s' cadastrada!", c.name);
    show_message(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))), msg);

    gtk_entry_set_text(GTK_ENTRY(name_entry), "");
    gtk_entry_set_text(GTK_ENTRY(school_search_entry), "");
    if (gtk_tree_model_iter_n_children(gtk_combo_box_get_model(GTK_COMBO_BOX(school_combo)), NULL) > 0)
        gtk_combo_box_set_active(GTK_COMBO_BOX(school_combo), 0);
    gtk_stack_set_visible_child_name(GTK_STACK(class_stack), "list");
    refresh_class_list(GTK_LIST_BOX(class_list_box));
}

static void on_save_edit_class(GtkButton *button, gpointer user_data) {
    EditClassData *data = user_data;

    Class upd = {0};
    upd.class_id = data->class->class_id;
    const char *sid = gtk_combo_box_get_active_id(GTK_COMBO_BOX(data->school_combo));
    upd.school_id = sid ? atoi(sid) : data->class->school_id;
    g_strlcpy(upd.name, gtk_entry_get_text(GTK_ENTRY(data->name_entry)), sizeof(upd.name));
    upd.students = data->class->students;
    upd.confirmed_students = data->class->confirmed_students;

    if (update_class(upd)) {
        show_message(GTK_WINDOW(data->dialog), "Turma atualizada.");
        refresh_class_list(data->list);
    } else {
        show_message(GTK_WINDOW(data->dialog), "Erro ao atualizar.");
    }

    gtk_widget_destroy(data->dialog);
    g_free(data);
}

static void on_edit_clicked(GtkButton *button, gpointer user_data) {
    Class *c = g_object_get_data(G_OBJECT(button), "class");
    GtkListBox *list = GTK_LIST_BOX(user_data);

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Editar Turma",
        GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
        GTK_DIALOG_MODAL,
        NULL);

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_margin_top(box, 24);
    gtk_widget_set_margin_bottom(box, 24);
    gtk_widget_set_margin_start(box, 24);
    gtk_widget_set_margin_end(box, 24);
    gtk_container_add(GTK_CONTAINER(content), box);

    GtkWidget *form = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(form), 12);
    gtk_grid_set_column_spacing(GTK_GRID(form), 12);
    gtk_box_pack_start(GTK_BOX(box), form, FALSE, FALSE, 0);

    GtkWidget *name_label = gtk_label_new("Nome:");
    gtk_widget_set_halign(name_label, GTK_ALIGN_END);
    GtkWidget *name_e = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(name_e), c->name);
    gtk_entry_set_width_chars(GTK_ENTRY(name_e), 30);

    GtkWidget *school_label = gtk_label_new("Escola:");
    gtk_widget_set_halign(school_label, GTK_ALIGN_END);
    GtkWidget *search_e = gtk_search_entry_new();
    GtkWidget *school_combo_e = gtk_combo_box_text_new();
    gtk_widget_set_hexpand(school_combo_e, FALSE);
    reload_school_data();
    refresh_school_combo(GTK_COMBO_BOX_TEXT(school_combo_e), NULL, FALSE);
    /* set active to current school's id */
    for (int i = 0; i < school_count; i++) {
        if (schools[i].id == c->school_id) {
            char idbuf[16];
            snprintf(idbuf, sizeof(idbuf), "%d", schools[i].id);
            gtk_combo_box_set_active_id(GTK_COMBO_BOX(school_combo_e), idbuf);
            break;
        }
    }

    g_signal_connect(search_e, "search-changed", G_CALLBACK(on_school_search_changed), school_combo_e);

    gtk_grid_attach(GTK_GRID(form), name_label, 0,0,1,1);
    gtk_grid_attach(GTK_GRID(form), name_e, 1,0,1,1);
    gtk_grid_attach(GTK_GRID(form), school_label, 0,1,1,1);
    gtk_grid_attach(GTK_GRID(form), search_e, 1,1,1,1);
    gtk_grid_attach(GTK_GRID(form), school_combo_e, 2,1,1,1);

    GtkWidget *btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_widget_set_halign(btn_box, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(box), btn_box, FALSE, FALSE, 0);

    GtkWidget *cancel_btn = gtk_button_new_with_label("❌ Cancelar");
    GtkWidget *save_btn = gtk_button_new_with_label("💾 Salvar");

    gtk_box_pack_start(GTK_BOX(btn_box), cancel_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(btn_box), save_btn, FALSE, FALSE, 0);

    g_signal_connect_swapped(cancel_btn, "clicked", G_CALLBACK(gtk_widget_destroy), dialog);

    EditClassData *data = g_malloc(sizeof(EditClassData));
    data->class = c;
    data->list = list;
    data->name_entry = name_e;
    data->school_combo = school_combo_e;
    data->dialog = dialog;

    g_signal_connect(save_btn, "clicked", G_CALLBACK(on_save_edit_class), data);

    gtk_widget_show_all(dialog);
}

static void on_confirm_delete(GtkButton *button, gpointer user_data) {
    DeleteClassData *data = (DeleteClassData *)user_data;

    if (delete_class(data->class->class_id)) {
        refresh_class_list(data->list);
        show_message(GTK_WINDOW(data->dialog), "Turma removida.");
    } else {
        show_message(GTK_WINDOW(data->dialog), "Turma não encontrada.");
    }

    gtk_widget_destroy(data->dialog);
    g_free(data);
}


static void on_delete_clicked(GtkButton *button, gpointer user_data) {
    Class *c = g_object_get_data(G_OBJECT(button), "class");
    GtkListBox *list = GTK_LIST_BOX(user_data);

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Remover Turma",
        GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
        GTK_DIALOG_MODAL,
        "Fechar", GTK_RESPONSE_CLOSE,
        NULL);

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_margin_top(box, 24);
    gtk_widget_set_margin_bottom(box, 24);
    gtk_widget_set_margin_start(box, 24);
    gtk_widget_set_margin_end(box, 24);
    gtk_container_add(GTK_CONTAINER(content), box);

    char question[128];
    snprintf(question, sizeof(question), "Remover turma '%s'?", c->name);
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

    DeleteClassData *data = g_malloc(sizeof(DeleteClassData));
    data->dialog = dialog;
    data->list = list;
    data->class = c;

    g_signal_connect(confirm_btn, "clicked", G_CALLBACK(on_confirm_delete), data);

    gtk_widget_show_all(dialog);
}

static void update_pagination_controls(int total_items) {
    int total_pages = (total_items + items_per_page - 1) / items_per_page;
    char page_text[32];
    snprintf(page_text, sizeof(page_text), "Página %d de %d", current_page, total_pages);
    gtk_label_set_text(GTK_LABEL(page_label), page_text);

    gtk_widget_set_sensitive(prev_button, current_page > 1);
    gtk_widget_set_sensitive(next_button, current_page < total_pages);
}


void classes_page_refresh(void) {
    gtk_entry_set_text(GTK_ENTRY(school_search_entry), "");
    gtk_entry_set_text(GTK_ENTRY(filter_search_entry), "");
    reload_school_data();
    refresh_school_combo(GTK_COMBO_BOX_TEXT(school_combo), NULL, FALSE);
    refresh_school_combo(GTK_COMBO_BOX_TEXT(filter_combo), NULL, TRUE);
    filter_school_id = -1;
    if (gtk_tree_model_iter_n_children(gtk_combo_box_get_model(GTK_COMBO_BOX(filter_combo)), NULL) > 0)
        gtk_combo_box_set_active(GTK_COMBO_BOX(filter_combo), 0);
    refresh_class_list(GTK_LIST_BOX(class_list_box));
}

static void on_prev_page(GtkButton *button, gpointer user_data) {
    if (current_page > 1) {
        current_page--;
        refresh_class_list(GTK_LIST_BOX(class_list_box));
    }
}

static void on_next_page(GtkButton *button, gpointer user_data) {
    Class *classes = NULL;
    int total_count = load_classes(&classes);
    free(classes);

    int total_pages = (total_count + items_per_page - 1) / items_per_page;
    if (current_page < total_pages) {
        current_page++;
        refresh_class_list(GTK_LIST_BOX(class_list_box));
    }
}
static void refresh_class_list(GtkListBox *list) {
    if (!GTK_IS_WIDGET(list)) {
        return;
    }

    GList *children = gtk_container_get_children(GTK_CONTAINER(list));
    for (GList *c = children; c; c = c->next) {
        gtk_widget_destroy(GTK_WIDGET(c->data));
    }
    g_list_free(children);

    reload_school_data();
    Class *classes = NULL;
    int total_count = load_classes(&classes);

    int start_index = (current_page - 1) * items_per_page;
    int end_index = start_index + items_per_page;
    if (end_index > total_count) {
        end_index = total_count;
    }

    for (int i = start_index; i < end_index; i++) {
        if (filter_school_id != -1 && classes[i].school_id != filter_school_id) {
            continue;
        }

        GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);

        char idbuf[16];
        snprintf(idbuf, sizeof(idbuf), "%d", classes[i].class_id);
        gtk_box_pack_start(GTK_BOX(row), gtk_label_new(idbuf), FALSE, FALSE, 0);

        const char *sname = "";
        for (int j = 0; j < school_count; j++) {
            if (schools[j].id == classes[i].school_id) {
                sname = schools[j].name;
                break;
            }
        }
        gtk_box_pack_start(GTK_BOX(row), gtk_label_new(sname), FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(row), gtk_label_new(classes[i].name), TRUE, TRUE, 0);

        GtkWidget *students_btn = gtk_button_new_with_label("Alunos");
        GtkWidget *edit_btn = gtk_button_new_with_label("Editar");
        GtkWidget *del_btn = gtk_button_new_with_label("Deletar");

        Class *copy = g_memdup2(&classes[i], sizeof(Class));
        g_object_set_data_full(G_OBJECT(students_btn), "class", copy, g_free);
        g_object_set_data(G_OBJECT(edit_btn), "class", copy);
        g_object_set_data(G_OBJECT(del_btn), "class", copy);

        g_signal_connect(students_btn, "clicked", G_CALLBACK(on_manage_students), NULL);
        g_signal_connect(edit_btn, "clicked", G_CALLBACK(on_edit_clicked), list);
        g_signal_connect(del_btn, "clicked", G_CALLBACK(on_delete_clicked), list);

        gtk_box_pack_start(GTK_BOX(row), students_btn, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(row), edit_btn, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(row), del_btn, FALSE, FALSE, 0);

        GtkWidget *wrap = gtk_list_box_row_new();
        gtk_container_add(GTK_CONTAINER(wrap), row);
        gtk_container_add(GTK_CONTAINER(list), wrap);
    }
    free(classes);

    if (GTK_IS_WIDGET(list)) {
        gtk_widget_show_all(GTK_WIDGET(list));
    }

    update_pagination_controls(total_count);
}

GtkWidget *build_classes_ui() {
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

    GtkWidget *register_btn = gtk_button_new_with_label("📘  Cadastrar turma");
    GtkWidget *list_btn = gtk_button_new_with_label("📋  Listar turmas");

    g_signal_connect(register_btn, "clicked", G_CALLBACK(show_class_page), "form");
    g_signal_connect(list_btn, "clicked", G_CALLBACK(show_class_page), "list");

    gtk_box_pack_start(GTK_BOX(actions), list_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(actions), register_btn, FALSE, FALSE, 0);

    class_stack = gtk_stack_new();

    GtkWidget *form_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(form_grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(form_grid), 8);

    GtkWidget *name_label = gtk_label_new("Nome da turma:");
    name_entry = gtk_entry_new();
    gtk_entry_set_width_chars(GTK_ENTRY(name_entry), 20);

    GtkWidget *sid_label = gtk_label_new("Escola:");
    school_search_entry = gtk_search_entry_new();
    school_combo = gtk_combo_box_text_new();
    gtk_widget_set_hexpand(school_combo, FALSE);
    g_signal_connect(school_search_entry, "search-changed", G_CALLBACK(on_school_search_changed), school_combo);
    reload_school_data();
    refresh_school_combo(GTK_COMBO_BOX_TEXT(school_combo), NULL, FALSE);

    gtk_grid_attach(GTK_GRID(form_grid), name_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(form_grid), name_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(form_grid), sid_label, 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(form_grid), school_search_entry, 3, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(form_grid), school_combo, 4, 0, 1, 1);

    GtkWidget *save_btn = gtk_button_new_with_label("Salvar");
    gtk_grid_attach(GTK_GRID(form_grid), save_btn, 0, 1, 5, 1);
    g_signal_connect(save_btn, "clicked", G_CALLBACK(on_save_class), NULL);

    GtkWidget *form_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_box_pack_start(GTK_BOX(form_container), form_grid, FALSE, FALSE, 0);

    GtkWidget *list_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    GtkWidget *filter_grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(filter_grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(filter_grid), 8);
    GtkWidget *filter_label = gtk_label_new("Escola:");
    filter_search_entry = gtk_search_entry_new();
    filter_combo = gtk_combo_box_text_new();
    g_signal_connect(filter_search_entry, "search-changed", G_CALLBACK(on_filter_search_changed), filter_combo);
    g_signal_connect(filter_combo, "changed", G_CALLBACK(on_filter_changed), NULL);
    refresh_school_combo(GTK_COMBO_BOX_TEXT(filter_combo), NULL, TRUE);
    gtk_grid_attach(GTK_GRID(filter_grid), filter_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(filter_grid), filter_search_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(filter_grid), filter_combo, 2, 0, 1, 1);
    gtk_box_pack_start(GTK_BOX(list_container), filter_grid, FALSE, FALSE, 0);

    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(scroll, -1, 390);

    class_list_box = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(scroll), class_list_box);
    gtk_box_pack_start(GTK_BOX(list_container), scroll, TRUE, TRUE, 0);

    GtkWidget *pagination_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_widget_set_margin_top(pagination_box, 8);
    gtk_widget_set_halign(pagination_box, GTK_ALIGN_CENTER);

    prev_button = gtk_button_new_with_label("⬅️ Anterior");
    next_button = gtk_button_new_with_label("Próxima ➡️");
    page_label = gtk_label_new("");

    gtk_box_pack_start(GTK_BOX(pagination_box), prev_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pagination_box), page_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pagination_box), next_button, FALSE, FALSE, 0);

    g_signal_connect(prev_button, "clicked", G_CALLBACK(on_prev_page), NULL);
    g_signal_connect(next_button, "clicked", G_CALLBACK(on_next_page), NULL);

    gtk_box_pack_start(GTK_BOX(list_container), pagination_box, FALSE, FALSE, 0);

    gtk_stack_add_titled(GTK_STACK(class_stack), list_container, "list", "Listagem");
    gtk_stack_add_titled(GTK_STACK(class_stack), form_container, "form", "Cadastro");

    gtk_box_pack_start(GTK_BOX(card), actions, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(card), class_stack, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), card, TRUE, TRUE, 0);

    gtk_stack_set_visible_child_name(GTK_STACK(class_stack), "list");
    refresh_class_list(GTK_LIST_BOX(class_list_box));

    return box;
}
