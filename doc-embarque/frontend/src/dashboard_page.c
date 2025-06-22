#include "dashboard_page.h"

GtkWidget *build_dashboard_ui() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
    gtk_widget_set_margin_top(box, 30);
    gtk_widget_set_margin_bottom(box, 30);
    gtk_widget_set_margin_start(box, 30);
    gtk_widget_set_margin_end(box, 30);

    GtkWidget *title = gtk_label_new("DocEmbarque");
    gtk_style_context_add_class(gtk_widget_get_style_context(title), "title");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_box_pack_start(GTK_BOX(box), title, FALSE, FALSE, 0);

    const char *lines[] = {
        "Resumo do sistema:",
        "Escolas: 4",
        "Turmas: 12",
        "Alunos: 96",
        "Embarques confirmados: 53",
        "Pagamentos pendentes: 21",
        "Seguros pendentes: 5"
    };

    for (int i = 0; i < 7; i++) {
        GtkWidget *label = gtk_label_new(lines[i]);
        gtk_widget_set_halign(label, GTK_ALIGN_START);
        gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    }

    return box;
}
