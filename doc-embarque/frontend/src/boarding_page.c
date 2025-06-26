#include "boarding_page.h"
#include "../../backend/include/models/student.h"
#include "../../backend/include/models/payment.h"
#include "../../backend/include/models/installment.h"
#include "../../backend/include/models/travel.h"
#include "../../backend/include/use_cases/read_student.h"
#include "../../backend/include/use_cases/read_payment.h"
#include "../../backend/include/use_cases/read_installment.h"
#include "../../backend/include/use_cases/read_travel.h"
#include <gtk/gtk.h>
#include <cairo.h>
#include <cairo-pdf.h>
#include <string.h>
#include <time.h>

static GtkWidget *list_box;
static GtkWidget *travel_combo;
static Travel *travels = NULL;
static int travel_count = 0;

static int date_is_overdue(const char *date_str) {
    if (strcmp(date_str, "pago") == 0)
        return 0;
    struct tm tm = {0};
    if (!strptime(date_str, "%Y-%m-%d", &tm))
        return 0;
    time_t due = mktime(&tm);
    time_t now = time(NULL);
    return difftime(now, due) > 0;
}

static void refresh_boarding_list(GtkComboBox *combo, gpointer user_data) {
    if (!GTK_IS_WIDGET(list_box)) return;
    GList *children = gtk_container_get_children(GTK_CONTAINER(list_box));
    for (GList *c = children; c; c = c->next) gtk_widget_destroy(GTK_WIDGET(c->data));
    g_list_free(children);
    const char *tid = gtk_combo_box_get_active_id(GTK_COMBO_BOX(travel_combo));
    if (!tid) return;
    int travel_id = atoi(tid);
    int class_id = -1;
    for (int i = 0; i < travel_count; i++) {
        if (travels[i].id == travel_id) {
            class_id = travels[i].class_id;
            break;
        }
    }
    if (class_id == -1) return;

    Student *students = NULL;
    int count = load_students_by_class(class_id, &students);
    for (int i = 0; i < count; i++) {
        double due = 0.0, paid = 0.0;
        int overdue = 0;
        Payment *payments = NULL;
        int pcount = load_payments_by_student(students[i].id, &payments);
        for (int p = 0; p < pcount; p++) {
            due += payments[p].amount;
            Installment *inst = NULL;
            int icount = load_installments_by_payment(payments[p].id, &inst);
            for (int j = 0; j < icount; j++) {
                if (inst[j].paid) {
                    paid += inst[j].amount;
                } else if (date_is_overdue(inst[j].due_date)) {
                    overdue = 1;
                }
            }
            free(inst);
        }
        free(payments);
        double debt = due - paid;

        char info[256];
        snprintf(info, sizeof(info), "%s - RG:%s - Devido: %.2lf - Pago: %.2lf - %s",
                 students[i].name, students[i].rg, due, paid,
                 overdue ? "ATRASADO" : (debt > 0.01 ? "DEVENDO" : "OK"));
        GtkWidget *row = gtk_label_new(info);
        GtkWidget *wrap = gtk_list_box_row_new();
        gtk_container_add(GTK_CONTAINER(wrap), row);
        gtk_container_add(GTK_CONTAINER(list_box), wrap);
    }
    free(students);
    gtk_widget_show_all(list_box);
}

static void generate_pdf(GtkButton *btn, gpointer data) {
    const char *tid = gtk_combo_box_get_active_id(GTK_COMBO_BOX(travel_combo));
    if (!tid) {
        GtkWidget *msg = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(btn))),
                                                GTK_DIALOG_MODAL,
                                                GTK_MESSAGE_INFO,
                                                GTK_BUTTONS_OK,
                                                "Selecione a viagem antes de gerar o PDF.");
        gtk_dialog_run(GTK_DIALOG(msg));
        gtk_widget_destroy(msg);
        return;
    }

    int travel_id = atoi(tid);
    Travel *travel = NULL;
    for (int i = 0; i < travel_count; i++) {
        if (travels[i].id == travel_id) { travel = &travels[i]; break; }
    }
    if (!travel) return;

    GtkFileChooserNative *chooser = gtk_file_chooser_native_new("Salvar ficha de embarque",
                                                               GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(btn))),
                                                               GTK_FILE_CHOOSER_ACTION_SAVE,
                                                               "Salvar", "Cancelar");
    char defname[256];
    snprintf(defname, sizeof(defname), "%s.pdf", travel->name);
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(chooser), defname);
    if (gtk_native_dialog_run(GTK_NATIVE_DIALOG(chooser)) != GTK_RESPONSE_ACCEPT) {
        g_object_unref(chooser);
        return;
    }
    char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser));
    g_object_unref(chooser);

    Student *students = NULL;
    int count = load_students_by_class(travel->class_id, &students);
    cairo_surface_t *surface = cairo_pdf_surface_create(filename, 595, 842);
    cairo_t *cr = cairo_create(surface);

    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 18);
    cairo_move_to(cr, 50, 40);
    cairo_show_text(cr, "Lumaster Turismo");
    cairo_set_font_size(cr, 14);
    cairo_move_to(cr, 50, 70);
    cairo_show_text(cr, travel->name);

    cairo_set_font_size(cr, 12);
    double y = 100.0;
    cairo_move_to(cr, 50, y);
    cairo_show_text(cr, "Nome");
    cairo_move_to(cr, 250, y);
    cairo_show_text(cr, "RG");
    cairo_move_to(cr, 360, y);
    cairo_show_text(cr, "Situacao");
    cairo_move_to(cr, 460, y);
    cairo_show_text(cr, "Embarcou");
    y += 20;
    for (int i = 0; i < count; i++) {
        double due = 0.0, paid = 0.0;
        int overdue = 0;
        Payment *payments = NULL;
        int pcount = load_payments_by_student(students[i].id, &payments);
        for (int p = 0; p < pcount; p++) {
            due += payments[p].amount;
            Installment *inst = NULL;
            int icount = load_installments_by_payment(payments[p].id, &inst);
            for (int j = 0; j < icount; j++) {
                if (inst[j].paid) {
                    paid += inst[j].amount;
                } else if (date_is_overdue(inst[j].due_date)) {
                    overdue = 1;
                }
            }
            free(inst);
        }
        free(payments);
        double debt = due - paid;
        const char *status = overdue ? "ATRASADO" : (debt > 0.01 ? "DEVENDO" : "OK");
        cairo_move_to(cr, 50, y);
        cairo_show_text(cr, students[i].name);
        cairo_move_to(cr, 250, y);
        cairo_show_text(cr, students[i].rg);
        cairo_move_to(cr, 360, y);
        cairo_show_text(cr, status);
        cairo_move_to(cr, 460, y);
        cairo_show_text(cr, "");
        y += 20;
        if (y > 820) {
            cairo_show_page(cr);
            y = 100;
        }
    }

    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    free(students);

    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(btn))),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "PDF salvo em %s",
                                               filename);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    g_free(filename);
}

GtkWidget *build_boarding_ui(void) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);

    travel_combo = gtk_combo_box_text_new();
    travels = NULL;
    travel_count = load_travels(&travels);
    for (int i = 0; i < travel_count; i++) {
        char idbuf[16];
        snprintf(idbuf, sizeof(idbuf), "%d", travels[i].id);
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(travel_combo), idbuf, travels[i].name);
    }
    if (travel_count > 0)
        gtk_combo_box_set_active(GTK_COMBO_BOX(travel_combo), 0);
    g_signal_connect(travel_combo, "changed", G_CALLBACK(refresh_boarding_list), NULL);

    GtkWidget *btn = gtk_button_new_with_label("Gerar PDF de Embarque");
    g_signal_connect(btn, "clicked", G_CALLBACK(generate_pdf), NULL);

    list_box = gtk_list_box_new();
    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scroll), list_box);

    gtk_box_pack_start(GTK_BOX(box), travel_combo, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), scroll, TRUE, TRUE, 0);

    refresh_boarding_list(GTK_COMBO_BOX(travel_combo), NULL);

    return box;
}