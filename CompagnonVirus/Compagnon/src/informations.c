#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 256

void displayCPUInfo(GtkWidget *grid, PangoFontDescription *font_desc, GdkRGBA *text_color) {
    // Lecture des informations CPU depuis le fichier
    FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
    char cpuInfoText[LENGTH * 10] = ""; // Stocke toutes les informations CPU
    int count = 0;
    char line[LENGTH];
    while (fgets(line, sizeof(line), cpuinfo) != NULL) {
        if (strstr(line, "model name") != NULL && count == 0) {
            strcat(cpuInfoText, line);
            count++;
        }
    }
    fclose(cpuinfo);
    // Création du label pour afficher les informations CPU
    GtkWidget *label = gtk_label_new(cpuInfoText);
    //gtk_widget_override_background_color(label, GTK_STATE_FLAG_NORMAL, text_color);
    //gtk_widget_override_font(label, font_desc);
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
}

void displayMemoryInfo(GtkWidget *grid, PangoFontDescription *font_desc, GdkRGBA *text_color) {
    // Lecture des informations sur la mémoire depuis le fichier
    FILE *meminfo = fopen("/proc/meminfo", "r");
    char memInfoText[LENGTH * 10] = ""; // Stocke toutes les informations sur la mémoire
    char line[LENGTH];
    while (fgets(line, sizeof(line), meminfo) != NULL) {
        if (strstr(line, "MemTotal") != NULL || strstr(line, "MemFree") != NULL) {
            strcat(memInfoText, line);
        }
    }

    fclose(meminfo);
    // Création du label pour afficher les informations sur la mémoire
    GtkWidget *label = gtk_label_new(memInfoText);
    //gtk_widget_override_background_color(label, GTK_STATE_FLAG_NORMAL, text_color);
    //gtk_widget_override_font(label, font_desc);
    gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
}

int main(int argc, char *argv[]) {
    // Initialisation de GTK
    gtk_init(&argc, &argv);

    // Création de la fenêtre
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Informations système");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Création de la grille
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Chargement de la police et de la couleur
    PangoFontDescription *font_desc = pango_font_description_from_string("Sans Bold 12");
    GdkRGBA text_color = {1.0, 1.0, 1.0, 1.0};

    // Appels des fonctions d'affichage
    displayCPUInfo(grid, font_desc, &text_color);
    displayMemoryInfo(grid, font_desc, &text_color);


    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();
    pango_font_description_free(font_desc);

    return 0;
}
