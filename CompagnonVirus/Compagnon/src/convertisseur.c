#include <gtk/gtk.h>

// Variables globales
GtkWidget *celsius_radio; // pour le choix
GtkWidget *affiche_resultat; // pour l'affichage

// Conversion de Celsius en Fahrenheit
double celsius_to_fahrenheit(double valeur) {
    return (valeur * 9 / 5) + 32;
}

// Conversion de Fahrenheit en Celsius
double fahrenheit_to_celsius(double valeur) {
    return (valeur - 32) * 5 / 9;
}

// Appelée lors du clic sur le bouton "Convertir"
void convert(GtkButton *button, gpointer user_data) {
    GtkWidget *entry = GTK_WIDGET(user_data);
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    double temperature = atof(text);

    // on recupère le choix de l'utilisateur
    gboolean is_celsius = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(celsius_radio));

    //conversion en fonction du choix
    double resultat;
    if (is_celsius) {
        resultat = celsius_to_fahrenheit(temperature);
        gtk_label_set_text(GTK_LABEL(affiche_resultat), g_strdup_printf("%.2f Celsius = %.2f Fahrenheit", temperature, resultat));
    } else {
        resultat = fahrenheit_to_celsius(temperature);
        gtk_label_set_text(GTK_LABEL(affiche_resultat), g_strdup_printf("%.2f Fahrenheit = %.2f Celsius", temperature, resultat));
    }
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget * texte_temperature;
    GtkWidget *bouton_conversion;

    gtk_init(&argc, &argv);

    // Fenêtre principale
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Convertisseur d'unités");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Gille pour organiser les éléments
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // L'entrée du texte pour la temperature
    GtkWidget *temperature_label = gtk_label_new("Température :");
    gtk_grid_attach(GTK_GRID(grid), temperature_label, 0, 0, 1, 1);
     texte_temperature = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid),  texte_temperature, 1, 0, 1, 1);

    // Les boutons radio pour choisir l'unité
    celsius_radio = gtk_radio_button_new_with_label(NULL, "Celsius");
    GtkWidget *fahrenheit_radio = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(celsius_radio), "Fahrenheit");
    gtk_grid_attach(GTK_GRID(grid), celsius_radio, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), fahrenheit_radio, 1, 1, 1, 1);

    // Le bouton convertir
    bouton_conversion = gtk_button_new_with_label("Convertir");
    gtk_grid_attach(GTK_GRID(grid), bouton_conversion, 0, 2, 2, 1);

    affiche_resultat = gtk_label_new(""); // pour le resultat affiche
    gtk_grid_attach(GTK_GRID(grid), affiche_resultat, 0, 3, 2, 1);
    g_signal_connect(bouton_conversion, "clicked", G_CALLBACK(convert),  texte_temperature);
    gtk_widget_show_all(window);

    // Démarrer la boucle principale GTK
    gtk_main();

    return 0;
}
