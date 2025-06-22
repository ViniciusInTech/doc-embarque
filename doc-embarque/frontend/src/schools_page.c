#include "schools_page.h"
#include <gtk/gtk.h>

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

    for (int i = 0; i < 4; i++) {
        GtkWidget *label = gtk_label_new(labels[i]);

        GtkWidget *btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
        gtk_box_pack_start(GTK_BOX(btn_box), label, FALSE, FALSE, 0);

        GtkWidget *btn = gtk_button_new();
        gtk_container_add(GTK_CONTAINER(btn), btn_box);
        gtk_widget_set_name(btn, "action-button");

        gtk_box_pack_start(GTK_BOX(actions), btn, FALSE, FALSE, 0);
    }

    gtk_box_pack_start(GTK_BOX(card), actions, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), card, FALSE, FALSE, 0);

    GtkWidget *label = gtk_label_new("Selecione uma ação para gerenciar escolas.");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_widget_set_margin_top(label, 20);
    gtk_widget_set_margin_start(label, 16);
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);

    return box;
}
