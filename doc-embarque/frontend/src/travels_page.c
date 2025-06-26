#include "travels_page.h"
#include "../../backend/include/models/travel.h"
#include "../../backend/include/models/class.h"
#include "../../backend/include/models/school.h"
#include "../../backend/include/use_cases/create_travel.h"
#include "../../backend/include/use_cases/read_class.h"
#include "../../backend/include/use_cases/read_school.h"
#include <gtk/gtk.h>
#include <stdlib.h>

static GtkWidget *name_entry;
static GtkWidget *cost_entry;
static GtkWidget *school_combo;
static GtkWidget *class_combo;
static GtkWidget *list_box;
static GtkWidget *travels_stack;

static void refresh_school_combo() {
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(school_combo));
    School *schools = NULL; int count = load_schools(&schools);
    for (int i = 0; i < count; i++) {
        char idbuf[16]; snprintf(idbuf, sizeof(idbuf), "%d", schools[i].id);
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(school_combo), idbuf, schools[i].name);
    }
    if (count > 0) gtk_combo_box_set_active(GTK_COMBO_BOX(school_combo), 0);
    free(schools);
}

static void refresh_class_combo_for_school(int school_id) {
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(class_combo));
    Class *classes = NULL; int count = load_classes(&classes);
    for (int i = 0; i < count; i++) {
        if (school_id == -1 || classes[i].school_id == school_id) {
            char idbuf[16]; snprintf(idbuf, sizeof(idbuf), "%d", classes[i].class_id);
            gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(class_combo), idbuf, classes[i].name);
        }
    }
    if (gtk_tree_model_iter_n_children(gtk_combo_box_get_model(GTK_COMBO_BOX(class_combo)), NULL) > 0)
        gtk_combo_box_set_active(GTK_COMBO_BOX(class_combo), 0);
    free(classes);
}

static void on_school_changed(GtkComboBox *combo, gpointer data) {
    const char *sid = gtk_combo_box_get_active_id(combo);
    int id = sid ? atoi(sid) : -1;
    refresh_class_combo_for_school(id);
}

static void refresh_travel_list() {
    if (!GTK_IS_WIDGET(list_box)) return;
    GList *children = gtk_container_get_children(GTK_CONTAINER(list_box));
    for (GList *c=children;c;c=c->next) gtk_widget_destroy(GTK_WIDGET(c->data));
    g_list_free(children);
    Travel *travels=NULL; int count=load_travels(&travels);
    Class *classes=NULL; int ccount=load_classes(&classes);
    for(int i=0;i<count;i++){
        GtkWidget *row=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,8);
        char idbuf[16]; snprintf(idbuf,sizeof(idbuf),"%d",travels[i].id);
        gtk_box_pack_start(GTK_BOX(row), gtk_label_new(idbuf), FALSE, FALSE, 0);
        const char *cname="";
        for(int j=0;j<ccount;j++){ if(classes[j].class_id==travels[i].class_id){ cname=classes[j].name; break;}}
        gtk_box_pack_start(GTK_BOX(row), gtk_label_new(cname), FALSE,FALSE,0);
        gtk_box_pack_start(GTK_BOX(row), gtk_label_new(travels[i].name), TRUE,TRUE,0);
        char costbuf[32]; snprintf(costbuf,sizeof(costbuf),"%.2lf",travels[i].base_cost);
        gtk_box_pack_start(GTK_BOX(row), gtk_label_new(costbuf), FALSE,FALSE,0);
        GtkWidget *wrap=gtk_list_box_row_new();
        gtk_container_add(GTK_CONTAINER(wrap),row);
        gtk_container_add(GTK_CONTAINER(list_box),wrap);
    }
    free(travels); free(classes);
    gtk_widget_show_all(list_box);
}

static void on_show_page(GtkButton *btn, gpointer data){
    const gchar *name=data; gtk_stack_set_visible_child_name(GTK_STACK(travels_stack),name);
    if(g_strcmp0(name,"list")==0) {
        refresh_travel_list();
    } else if(g_strcmp0(name,"form")==0) {
        refresh_school_combo();
        const char *sid = gtk_combo_box_get_active_id(GTK_COMBO_BOX(school_combo));
        int id = sid ? atoi(sid) : -1;
        refresh_class_combo_for_school(id);
    }
}

static void on_save_travel(GtkButton *btn, gpointer data){
    const char *tname=gtk_entry_get_text(GTK_ENTRY(name_entry));
    const char *costtxt=gtk_entry_get_text(GTK_ENTRY(cost_entry));
    const char *cid=gtk_combo_box_get_active_id(GTK_COMBO_BOX(class_combo));
    if(!*tname||!*costtxt||!cid){ return; }
    Travel t={0}; t.class_id=atoi(cid); g_strlcpy(t.name,tname,sizeof(t.name)); t.base_cost=g_ascii_strtod(costtxt,NULL);
    create_travel(t);
    gtk_entry_set_text(GTK_ENTRY(name_entry),"" );
    gtk_entry_set_text(GTK_ENTRY(cost_entry),"" );
    refresh_travel_list();
    gtk_stack_set_visible_child_name(GTK_STACK(travels_stack),"list");
}

GtkWidget *build_travels_ui(){
    GtkWidget *box=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
    GtkWidget *card=gtk_box_new(GTK_ORIENTATION_VERTICAL,12);
    gtk_widget_set_name(card,"action-card");
    gtk_widget_set_margin_start(card,16);
    gtk_widget_set_margin_end(card,16);

    GtkWidget *actions=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,16);
    gtk_widget_set_margin_top(actions,16);
    GtkWidget *reg_btn=gtk_button_new_with_label("\xF0\x9F\x97\x9C  Nova viagem");
    GtkWidget *list_btn=gtk_button_new_with_label("\xF0\x9F\x93\x8B  Listar viagens");
    g_signal_connect(reg_btn,"clicked",G_CALLBACK(on_show_page),"form");
    g_signal_connect(list_btn,"clicked",G_CALLBACK(on_show_page),"list");
    gtk_box_pack_start(GTK_BOX(actions),list_btn,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(actions),reg_btn,FALSE,FALSE,0);

    travels_stack=gtk_stack_new();

    GtkWidget *form=gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(form),8);
    gtk_grid_set_column_spacing(GTK_GRID(form),8);
    GtkWidget *name_l=gtk_label_new("Descricao:");
    name_entry=gtk_entry_new();
    GtkWidget *cost_l=gtk_label_new("Custo base:");
    cost_entry=gtk_entry_new();
    GtkWidget *school_l=gtk_label_new("Escola:");
    school_combo=gtk_combo_box_text_new();
    g_signal_connect(school_combo,"changed",G_CALLBACK(on_school_changed),NULL);
    GtkWidget *class_l=gtk_label_new("Turma:");
    class_combo=gtk_combo_box_text_new();
    gtk_grid_attach(GTK_GRID(form),name_l,0,0,1,1);
    gtk_grid_attach(GTK_GRID(form),name_entry,1,0,1,1);
    gtk_grid_attach(GTK_GRID(form),cost_l,0,1,1,1);
    gtk_grid_attach(GTK_GRID(form),cost_entry,1,1,1,1);
    gtk_grid_attach(GTK_GRID(form),school_l,0,2,1,1);
    gtk_grid_attach(GTK_GRID(form),school_combo,1,2,1,1);
    gtk_grid_attach(GTK_GRID(form),class_l,0,3,1,1);
    gtk_grid_attach(GTK_GRID(form),class_combo,1,3,1,1);
    GtkWidget *save_btn=gtk_button_new_with_label("Salvar");
    gtk_grid_attach(GTK_GRID(form),save_btn,0,4,2,1);
    g_signal_connect(save_btn,"clicked",G_CALLBACK(on_save_travel),NULL);

    GtkWidget *list_container=gtk_scrolled_window_new(NULL,NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(list_container),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
    list_box=gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(list_container),list_box);

    gtk_stack_add_titled(GTK_STACK(travels_stack),list_container,"list","Listagem");
    gtk_stack_add_titled(GTK_STACK(travels_stack),form,"form","Cadastro");

    gtk_box_pack_start(GTK_BOX(card),actions,FALSE,FALSE,0);
    gtk_box_pack_start(GTK_BOX(card),travels_stack,TRUE,TRUE,0);
    gtk_box_pack_start(GTK_BOX(box),card,TRUE,TRUE,0);

    gtk_stack_set_visible_child_name(GTK_STACK(travels_stack),"list");
    refresh_travel_list();
    return box;
}