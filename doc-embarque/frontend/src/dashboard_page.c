#include "dashboard_page.h"
#include "../../backend/include/models/school.h"
#include "../../backend/include/models/student.h"
#include "../../backend/include/models/payment.h"
#include "../../backend/include/models/installment.h"
#include "../../backend/include/use_cases/read_school.h"
#include "../../backend/include/use_cases/read_student.h"
#include "../../backend/include/use_cases/read_payment.h"
#include "../../backend/include/use_cases/read_installment.h"
#include "../../backend/include/use_cases/read_travel.h"
#include "../../backend/include/use_cases/read_class.h"
#include <cairo.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[100];
    double due;
    double paid;
} TravelSummary;

typedef struct {
    int total_schools;
    int total_students;
    double total_due;
    double total_paid;
    double pix_paid;
    double card_paid;
    TravelSummary *travels;
    int travel_count;
} DashboardData;

static void dashboard_data_free(gpointer data) {
    DashboardData *d = data;
    g_free(d->travels);
    g_free(d);
}

static void compute_dashboard_data(DashboardData *d) {
    memset(d, 0, sizeof(*d));

    School *schools = NULL;
    d->total_schools = load_schools(&schools);
    free(schools);

    Student *students = NULL;
    d->total_students = load_students(&students);
    free(students);

    Payment *payments = NULL;
    int pcount = load_payments_by_student(-1, &payments);
    for (int i = 0; i < pcount; i++) {
        d->total_due += payments[i].amount;
    }

    Installment *insts = NULL;
    int icount = load_installments_by_payment(-1, &insts);
    for (int i = 0; i < icount; i++) {
        if (insts[i].paid) {
            d->total_paid += insts[i].amount;
            if (g_ascii_strcasecmp(insts[i].method, "PIX") == 0)
                d->pix_paid += insts[i].amount;
            else
                d->card_paid += insts[i].amount;
        }
    }

    Travel *travels = NULL;
    d->travel_count = load_travels(&travels);
    d->travels = g_new0(TravelSummary, d->travel_count);
    for (int i = 0; i < d->travel_count; i++) {
        g_strlcpy(d->travels[i].name, travels[i].name, sizeof(d->travels[i].name));
        Student *tstudents = NULL;
        int scount = load_students_by_class(travels[i].class_id, &tstudents);
        for (int s = 0; s < scount; s++) {
            Payment *spays = NULL;
            int spcount = load_payments_by_student(tstudents[s].id, &spays);
            for (int p = 0; p < spcount; p++) {
                d->travels[i].due += spays[p].amount;
                Installment *sinst = NULL;
                int sicount = load_installments_by_payment(spays[p].id, &sinst);
                for (int k = 0; k < sicount; k++) {
                    if (sinst[k].paid)
                        d->travels[i].paid += sinst[k].amount;
                }
                free(sinst);
            }
            free(spays);
        }
        free(tstudents);
    }

    free(travels);
    free(insts);
    free(payments);
}

static gboolean draw_chart(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    DashboardData *d = user_data;
    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    double max_val = d->card_paid > d->pix_paid ? d->card_paid : d->pix_paid;
    if (max_val < 1.0) max_val = 1.0;

    double bar_width = width / 4.0;
    double spacing = bar_width / 2.0;

    double card_h = (d->card_paid / max_val) * (height - 20);
    double pix_h = (d->pix_paid / max_val) * (height - 20);

    cairo_set_source_rgb(cr, 0.2, 0.5, 0.9);
    cairo_rectangle(cr, spacing, height - card_h, bar_width, card_h);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0.1, 0.8, 0.4);
    cairo_rectangle(cr, spacing * 2 + bar_width, height - pix_h, bar_width, pix_h);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 1, 1, 1);
    char buf[64];
    snprintf(buf, sizeof(buf), "Cartao: %.2lf", d->card_paid);
    cairo_move_to(cr, spacing, 15);
    cairo_show_text(cr, buf);
    snprintf(buf, sizeof(buf), "PIX: %.2lf", d->pix_paid);
    cairo_move_to(cr, spacing * 2 + bar_width, 15);
    cairo_show_text(cr, buf);

    return FALSE;
}

static gboolean draw_travel_chart(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    DashboardData *d = user_data;
    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    int n = d->travel_count;
    if (n == 0)
        return FALSE;

    double max_val = 0.0;
    for (int i = 0; i < n; i++) {
        double pending = d->travels[i].due - d->travels[i].paid;
        if (d->travels[i].paid > max_val) max_val = d->travels[i].paid;
        if (pending > max_val) max_val = pending;
    }
    if (max_val < 1.0) max_val = 1.0;

    double group_w = width / (double)(n * 3);
    double bar_w = group_w;
    double spacing = group_w / 2.0;

    for (int i = 0; i < n; i++) {
        double paid_h = (d->travels[i].paid / max_val) * (height - 40);
        double debt_h = ((d->travels[i].due - d->travels[i].paid) / max_val) * (height - 40);
        double x0 = spacing + i * 3 * bar_w;

        cairo_set_source_rgb(cr, 0.2, 0.5, 0.9);
        cairo_rectangle(cr, x0, height - paid_h - 20, bar_w, paid_h);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 0.8, 0.3, 0.3);
        cairo_rectangle(cr, x0 + bar_w, height - debt_h - 20, bar_w, debt_h);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 1, 1, 1);
        cairo_move_to(cr, x0, height - 5);
        cairo_show_text(cr, d->travels[i].name);
    }

    return FALSE;
}

GtkWidget *build_dashboard_ui() {
    DashboardData *data = g_new0(DashboardData, 1);
    compute_dashboard_data(data);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_margin_top(box, 30);
    gtk_widget_set_margin_bottom(box, 30);
    gtk_widget_set_margin_start(box, 30);
    gtk_widget_set_margin_end(box, 30);

    GtkWidget *title = gtk_label_new("DocEmbarque");
    gtk_style_context_add_class(gtk_widget_get_style_context(title), "title");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(box), title, FALSE, FALSE, 0);

    GtkWidget *summary = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);

    char buf[128];
    snprintf(buf, sizeof(buf), "Escolas: %d", data->total_schools);
    gtk_box_pack_start(GTK_BOX(summary), gtk_label_new(buf), FALSE, FALSE, 0);

    snprintf(buf, sizeof(buf), "Alunos cadastrados: %d", data->total_students);
    gtk_box_pack_start(GTK_BOX(summary), gtk_label_new(buf), FALSE, FALSE, 0);

    snprintf(buf, sizeof(buf), "Valor recebido: %.2lf", data->total_paid);
    gtk_box_pack_start(GTK_BOX(summary), gtk_label_new(buf), FALSE, FALSE, 0);

    double to_receive = data->total_due - data->total_paid;
    snprintf(buf, sizeof(buf), "Valor a receber: %.2lf", to_receive);
    gtk_box_pack_start(GTK_BOX(summary), gtk_label_new(buf), FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(box), summary, FALSE, FALSE, 0);

    GtkWidget *chart = gtk_drawing_area_new();
    gtk_widget_set_size_request(chart, 300, 200);
    g_signal_connect(chart, "draw", G_CALLBACK(draw_chart), data);

    GtkWidget *travel_chart = gtk_drawing_area_new();
    gtk_widget_set_size_request(travel_chart, 400, 250);
    g_signal_connect(travel_chart, "draw", G_CALLBACK(draw_travel_chart), data);

    g_object_set_data_full(G_OBJECT(box), "dashboard-data", data, dashboard_data_free);

    gtk_box_pack_start(GTK_BOX(box), chart, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), travel_chart, FALSE, FALSE, 0);

    return box;
}
