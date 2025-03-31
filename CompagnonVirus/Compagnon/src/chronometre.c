#include <gtk/gtk.h>

static int temps = 0;  // Stocker le temps écoulé en secondes
static guint timeout_id = 0; // Identifiant du timeout utilisé pour le rafraîchissement périodique du chronomètre

gboolean mise_a_jour_temps(gpointer user_data) {
    temps++;
    int minutes = temps / 60;
    int secondes = temps % 60;
    char temps_texte[10];
    sprintf(temps_texte, "%02d:%02d", minutes, secondes); // écriture dans temps_texte
    gtk_label_set_text(GTK_LABEL(user_data), temps_texte); // affichage
    return G_SOURCE_CONTINUE;
}

void demarrer_arreter(GtkWidget *widget, gpointer data) {
    static gboolean en_cours = FALSE;
    if (!en_cours) {
        en_cours = TRUE;
        gtk_button_set_label(GTK_BUTTON(widget), "Arrêter");
        timeout_id = g_timeout_add_seconds(1, mise_a_jour_temps, data); // misae a jour du chrono avec appel de la fonction toutes les secondes
    } else {
        en_cours = FALSE;
        gtk_button_set_label(GTK_BUTTON(widget), "Démarrer");
        g_source_remove(timeout_id); // Arrêter le chrono
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    //fenetre principale
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Chronomètre");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    //Affichage et bouton
    GtkWidget *label = gtk_label_new("00:00");
    GtkWidget *bouton_demarrer = gtk_button_new_with_label("Démarrer");
    g_signal_connect(bouton_demarrer, "clicked", G_CALLBACK(demarrer_arreter), label);

    // Initialisation GTK
    GtkWidget *boite = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_box_pack_start(GTK_BOX(boite), label, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(boite), bouton_demarrer, TRUE, TRUE, 0);
    gtk_container_add(GTK_CONTAINER(window), boite);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
