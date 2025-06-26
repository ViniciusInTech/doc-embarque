#include "payments_page.h"
#include "../../backend/include/models/school.h"
#include "../../backend/include/models/class.h"
#include "../../backend/include/models/student.h"
#include "../../backend/include/models/payment.h"
#include "../../backend/include/models/installment.h"
#include "../../backend/include/use_cases/read_school.h"
#include "../../backend/include/use_cases/read_class.h"
#include "../../backend/include/use_cases/read_student.h"
#include "../../backend/include/use_cases/read_payment.h"
#include "../../backend/include/use_cases/create_installment.h"
#include "../../backend/include/use_cases/read_installment.h"
#include "../../backend/include/use_cases/check_student_payment.h"
#include <cairo.h>
#include <cairo-pdf.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

static GtkWidget *school_combo;
static GtkWidget *class_combo;
static GtkWidget *student_combo;
static GtkWidget *list_box;

typedef struct {
    GtkWidget *dialog;
    GtkListBox *list;
    GtkWidget *page_label;
    GtkWidget *prev_btn;
    GtkWidget *next_btn;
    Student *students;
    int count;
    int current_page;
    int items_per_page;
} SummaryDialog;

static void summary_update_page(SummaryDialog *data);
static void export_summary_pdf(SummaryDialog *data);

static void refresh_student_combo(int class_id);
static void refresh_payment_list(int student_id);

static void refresh_school_combo() {
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(school_combo));
    School *schools = NULL; int count = load_schools(&schools);
    for(int i=0;i<count;i++){
        char idbuf[16]; snprintf(idbuf,sizeof(idbuf),"%d",schools[i].id);
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(school_combo), idbuf, schools[i].name);
    }
    if(count>0) gtk_combo_box_set_active(GTK_COMBO_BOX(school_combo),0);
    free(schools);
}

static void refresh_class_combo(int school_id) {
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(class_combo));
    Class *classes=NULL; int count=load_classes(&classes);
    for(int i=0;i<count;i++){
        if(school_id==-1 || classes[i].school_id==school_id){
            char idbuf[16]; snprintf(idbuf,sizeof(idbuf),"%d",classes[i].class_id);
            gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(class_combo), idbuf, classes[i].name);
        }
    }
    if(gtk_tree_model_iter_n_children(gtk_combo_box_get_model(GTK_COMBO_BOX(class_combo)),NULL)>0)
        gtk_combo_box_set_active(GTK_COMBO_BOX(class_combo),0);
    free(classes);
}

static void on_school_changed(GtkComboBox *combo, gpointer data){
    const char *sid=gtk_combo_box_get_active_id(combo);
    int id=sid?atoi(sid):-1;
    refresh_class_combo(id);
}
static void summary_update_page(SummaryDialog *data) {
    if (!GTK_IS_WIDGET(data->list)) return;
    GList *children = gtk_container_get_children(GTK_CONTAINER(data->list));
    for (GList *c=children;c;c=c->next) gtk_widget_destroy(GTK_WIDGET(c->data));
    g_list_free(children);

    int start = (data->current_page - 1) * data->items_per_page;
    int end = start + data->items_per_page;
    if (end > data->count) end = data->count;

    for (int i=start;i<end;i++) {
        GtkWidget *row=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,8);
        char idbuf[16]; snprintf(idbuf,sizeof(idbuf),"%d",data->students[i].id);
        gtk_box_pack_start(GTK_BOX(row),gtk_label_new(idbuf),FALSE,FALSE,0);
        gtk_box_pack_start(GTK_BOX(row),gtk_label_new(data->students[i].name),TRUE,TRUE,0);
        int paid=is_student_paid(data->students[i].id);
        gtk_box_pack_start(GTK_BOX(row),gtk_label_new(paid?"Pago":"Em aberto"),FALSE,FALSE,0);
        GtkWidget *wrap=gtk_list_box_row_new();
        gtk_container_add(GTK_CONTAINER(wrap),row);
        gtk_container_add(GTK_CONTAINER(data->list),wrap);
    }

    char buf[32];
    int total_pages = (data->count + data->items_per_page -1)/data->items_per_page;
    snprintf(buf,sizeof(buf),"P\303\241gina %d de %d",data->current_page,total_pages);
    gtk_label_set_text(GTK_LABEL(data->page_label),buf);
    gtk_widget_set_sensitive(data->prev_btn,data->current_page>1);
    gtk_widget_set_sensitive(data->next_btn,data->current_page<total_pages);

    gtk_widget_show_all(GTK_WIDGET(data->list));
}

static void on_prev_summary(GtkButton *btn, gpointer user_data){
    SummaryDialog *d=user_data;
    if(d->current_page>1){d->current_page--; summary_update_page(d);}
}

static void on_next_summary(GtkButton *btn, gpointer user_data){
    SummaryDialog *d=user_data;
    int total_pages=(d->count + d->items_per_page -1)/d->items_per_page;
    if(d->current_page<total_pages){d->current_page++; summary_update_page(d);}
}

static void export_summary_pdf(SummaryDialog *data){
    cairo_surface_t *surface=cairo_pdf_surface_create("summary.pdf",595,842);
    cairo_t *cr=cairo_create(surface);
    cairo_select_font_face(cr,"Sans",CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr,12);
    double y=40;
    for(int i=0;i<data->count;i++){
        char line[256];
        int paid=is_student_paid(data->students[i].id);
        snprintf(line,sizeof(line),"%d - %s - %s",data->students[i].id,data->students[i].name,paid?"Pago":"Em aberto");
        cairo_move_to(cr,50,y);
        cairo_show_text(cr,line);
        y+=20;
        if(y>800){
            cairo_show_page(cr);
            y=40;
        }
    }
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

static void on_export_pdf(GtkButton *btn, gpointer user_data){
    SummaryDialog *d=user_data; export_summary_pdf(d);
}

static void show_summary_dialog(GtkButton *btn, gpointer user_data){
    const char *cid=gtk_combo_box_get_active_id(GTK_COMBO_BOX(class_combo));
    if(!cid) return;
    int class_id=atoi(cid);
    SummaryDialog *d=g_malloc0(sizeof(SummaryDialog));
    d->items_per_page=10; d->current_page=1;
    d->count=load_students_by_class(class_id,&d->students);

    GtkWidget *dialog=gtk_dialog_new_with_buttons("Resumo da turma",
            GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(btn))),
            GTK_DIALOG_MODAL,
            "Fechar", GTK_RESPONSE_CLOSE,
            NULL);
    d->dialog=dialog;
    GtkWidget *content=gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *box=gtk_box_new(GTK_ORIENTATION_VERTICAL,8);
    gtk_container_add(GTK_CONTAINER(content),box);

    GtkWidget *list=gtk_list_box_new();
    d->list=GTK_LIST_BOX(list);
    gtk_widget_set_size_request(list,400,300);
    gtk_box_pack_start(GTK_BOX(box),list,TRUE,TRUE,0);

    GtkWidget *controls=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,8);
    d->prev_btn=gtk_button_new_with_label("\342\x9c\x85 Anterior");
    d->next_btn=gtk_button_new_with_label("Pr\303\263xima \342\x9e\xa2");
    d->page_label=gtk_label_new("");
    GtkWidget *export_btn=gtk_button_new_with_label("Exportar PDF");
    gtk_box_pack_start(GTK_BOX(controls),d->prev_btn,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(controls),d->page_label,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(controls),d->next_btn,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(controls),export_btn,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),controls,FALSE,FALSE,0);

    g_signal_connect(d->prev_btn,"clicked",G_CALLBACK(on_prev_summary),d);
    g_signal_connect(d->next_btn,"clicked",G_CALLBACK(on_next_summary),d);
    g_signal_connect(export_btn,"clicked",G_CALLBACK(on_export_pdf),d);
    g_signal_connect(dialog,"response",G_CALLBACK(g_free),d);

    summary_update_page(d);
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static void refresh_student_combo(int class_id){
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(student_combo));
    Student *students=NULL; int count=load_students_by_class(class_id,&students);
    for(int i=0;i<count;i++){
        char idbuf[16]; snprintf(idbuf,sizeof(idbuf),"%d",students[i].id);
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(student_combo), idbuf, students[i].name);
    }
    if(count>0) gtk_combo_box_set_active(GTK_COMBO_BOX(student_combo),0);
    free(students);
}

static void on_class_changed(GtkComboBox *combo, gpointer data){
    const char *cid=gtk_combo_box_get_active_id(combo);
    int id=cid?atoi(cid):-1;
    refresh_student_combo(id);
}

typedef struct {
    GtkWidget *dialog;
    GtkWidget *amount_entry;
    GtkWidget *proof_entry;
    GtkWidget *method_combo;
    int payment_id;
} InstallmentDialog;

static void on_installment_response(GtkDialog *dialog, gint response, gpointer user_data){
    InstallmentDialog *d=user_data;
    if(response==GTK_RESPONSE_OK){
        const char *amount_s=gtk_entry_get_text(GTK_ENTRY(d->amount_entry));
        const char *proof=gtk_entry_get_text(GTK_ENTRY(d->proof_entry));
        const char *method=gtk_combo_box_get_active_id(GTK_COMBO_BOX(d->method_combo));
        if(!method) method="";
        Installment inst={0};
        inst.payment_id=d->payment_id;
        inst.amount=g_ascii_strtod(amount_s,NULL);
        strcpy(inst.due_date,"pago");
        g_strlcpy(inst.proof, proof, sizeof(inst.proof));
        g_strlcpy(inst.method, method, sizeof(inst.method));
        inst.paid=1;
        create_installment(inst);
        const char *sid=gtk_combo_box_get_active_id(GTK_COMBO_BOX(student_combo));
        if(sid) refresh_payment_list(atoi(sid));
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));
    g_free(d);
}

static void add_installment(GtkButton *btn, gpointer data){
    int payment_id=GPOINTER_TO_INT(data);
    GtkWidget *dialog=gtk_dialog_new_with_buttons("Registrar parcela",
            GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(btn))),
            GTK_DIALOG_MODAL,
            "Cancelar", GTK_RESPONSE_CANCEL,
            "Salvar", GTK_RESPONSE_OK,
            NULL);
    GtkWidget *content=gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid=gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid),8);
    gtk_grid_set_column_spacing(GTK_GRID(grid),8);
    gtk_container_add(GTK_CONTAINER(content),grid);

    GtkWidget *amount_e=gtk_entry_new();
    GtkWidget *proof_e=gtk_entry_new();
    GtkWidget *method_c=gtk_combo_box_text_new();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(method_c),"PIX","PIX");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(method_c),"CARTAO","Cartao");
    gtk_combo_box_set_active(GTK_COMBO_BOX(method_c),0);

    gtk_grid_attach(GTK_GRID(grid),gtk_label_new("Valor"),0,0,1,1);
    gtk_grid_attach(GTK_GRID(grid),amount_e,1,0,1,1);
    gtk_grid_attach(GTK_GRID(grid),gtk_label_new("Comprovante"),0,1,1,1);
    gtk_grid_attach(GTK_GRID(grid),proof_e,1,1,1,1);
    gtk_grid_attach(GTK_GRID(grid),gtk_label_new("Metodo"),0,2,1,1);
    gtk_grid_attach(GTK_GRID(grid),method_c,1,2,1,1);

    InstallmentDialog *d=g_malloc(sizeof(InstallmentDialog));
    d->dialog=dialog; d->amount_entry=amount_e; d->proof_entry=proof_e;
    d->method_combo=method_c; d->payment_id=payment_id;

    g_signal_connect(dialog,"response",G_CALLBACK(on_installment_response),d);
    gtk_widget_show_all(dialog);
}

static void refresh_payment_list(int student_id){
    if(!GTK_IS_WIDGET(list_box)) return;
    GList *children=gtk_container_get_children(GTK_CONTAINER(list_box));
    for(GList *c=children;c;c=c->next) gtk_widget_destroy(GTK_WIDGET(c->data));
    g_list_free(children);
    Payment *payments=NULL; int count=load_payments_by_student(student_id,&payments);
    for(int i=0;i<count;i++){
        GtkWidget *row=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,8);
        char idbuf[16]; snprintf(idbuf,sizeof(idbuf),"%d",payments[i].id);
        gtk_box_pack_start(GTK_BOX(row),gtk_label_new(idbuf),FALSE,FALSE,0);
        char valbuf[32]; snprintf(valbuf,sizeof(valbuf),"%.2lf",payments[i].amount);
        gtk_box_pack_start(GTK_BOX(row),gtk_label_new(valbuf),FALSE,FALSE,0);
        Installment *insts=NULL; int icount=load_installments_by_payment(payments[i].id,&insts);
        double paid=0; for(int j=0;j<icount;j++) paid+=insts[j].amount;
        double debt=payments[i].amount - paid;
        char debtbuf[32]; snprintf(debtbuf,sizeof(debtbuf),"%.2lf",debt);
        gtk_box_pack_start(GTK_BOX(row),gtk_label_new(debtbuf),FALSE,FALSE,0);
        GtkWidget *btn=gtk_button_new_with_label("Registrar parcela");
        g_signal_connect(btn,"clicked",G_CALLBACK(add_installment),GINT_TO_POINTER(payments[i].id));
        gtk_box_pack_start(GTK_BOX(row),btn,FALSE,FALSE,0);
        GtkWidget *wrap=gtk_list_box_row_new();
        gtk_container_add(GTK_CONTAINER(wrap),row);
        gtk_container_add(GTK_CONTAINER(list_box),wrap);
        free(insts);
    }
    free(payments);
    gtk_widget_show_all(list_box);
}

static void on_student_changed(GtkComboBox *combo, gpointer data){
    const char *sid=gtk_combo_box_get_active_id(combo);
    int id=sid?atoi(sid):-1;
    refresh_payment_list(id);
}

GtkWidget *build_payments_ui(void){
    GtkWidget *box=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    GtkWidget *controls=gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(controls),8);
    gtk_grid_set_column_spacing(GTK_GRID(controls),8);

    school_combo=gtk_combo_box_text_new();
    class_combo=gtk_combo_box_text_new();
    student_combo=gtk_combo_box_text_new();
    GtkWidget *summary_btn=gtk_button_new_with_label("Resumo da turma");

    g_signal_connect(school_combo,"changed",G_CALLBACK(on_school_changed),NULL);
    g_signal_connect(class_combo,"changed",G_CALLBACK(on_class_changed),NULL);
    g_signal_connect(student_combo,"changed",G_CALLBACK(on_student_changed),NULL);
    g_signal_connect(summary_btn,"clicked",G_CALLBACK(show_summary_dialog),NULL);

    gtk_grid_attach(GTK_GRID(controls),gtk_label_new("Escola"),0,0,1,1);
    gtk_grid_attach(GTK_GRID(controls),school_combo,1,0,1,1);
    gtk_grid_attach(GTK_GRID(controls),gtk_label_new("Turma"),2,0,1,1);
    gtk_grid_attach(GTK_GRID(controls),class_combo,3,0,1,1);
    gtk_grid_attach(GTK_GRID(controls),gtk_label_new("Aluno"),4,0,1,1);
    gtk_grid_attach(GTK_GRID(controls),student_combo,5,0,1,1);
    gtk_grid_attach(GTK_GRID(controls),summary_btn,6,0,1,1);

    list_box=gtk_list_box_new();
    GtkWidget *scroll=gtk_scrolled_window_new(NULL,NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scroll),list_box);

    gtk_box_pack_start(GTK_BOX(box),controls,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(box),scroll,TRUE,TRUE,0);

    refresh_school_combo();
    const char *sid=gtk_combo_box_get_active_id(GTK_COMBO_BOX(school_combo));
    int cid=-1; if(sid) { refresh_class_combo(atoi(sid)); const char *cidstr=gtk_combo_box_get_active_id(GTK_COMBO_BOX(class_combo)); if(cidstr) cid=atoi(cidstr); }
    refresh_student_combo(cid);
    const char *stud=gtk_combo_box_get_active_id(GTK_COMBO_BOX(student_combo));
    if(stud) refresh_payment_list(atoi(stud));

    return box;
}