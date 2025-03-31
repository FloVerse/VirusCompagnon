#include <gtk/gtk.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

GtkWidget *input_entry;
GtkWidget *output_entry;
GtkWidget *key_entry;
GtkWidget *endpath_entry;
GtkWidget *file_chooser_button;

void file_selected(GtkFileChooserButton *filechooserbutton, gpointer user_data);
void save_file(GtkButton *button, gpointer user_data);
char* decrypt_vigenere(const gchar *text, const gchar *key);
int getStringLength(const gchar * text);
void decrypt_file(GtkButton *button, gpointer user_data);

int main(int argc, char *argv[]) {
    // Initialisation de GTK
    gtk_init(&argc, &argv);

    // Création de la fenêtre principale
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Dechiffrement Vigenere de fichier");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 400, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    //Layout au sein de la fenêtre principale
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *hbox2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    
    //Widgets: bouttons, text_entry
    file_chooser_button = gtk_file_chooser_button_new("Sélectionner un fichier", GTK_FILE_CHOOSER_ACTION_OPEN);
    GtkWidget *encrypt_button = gtk_button_new_with_label("Chiffrer");
    GtkWidget *save_button = gtk_button_new_with_label("Sauvegarder");
    key_entry = gtk_entry_new();
    input_entry = gtk_entry_new();
    output_entry = gtk_entry_new();
    endpath_entry = gtk_entry_new();
    GtkWidget *key_label = gtk_label_new("Cle");


    gtk_widget_set_size_request(input_entry, 400, 400);
    gtk_widget_set_size_request(output_entry, 400, 400);

    //Connections des boutons aux fonctions respectives    
    g_signal_connect(file_chooser_button, "file-set", G_CALLBACK(file_selected), NULL);
    g_signal_connect(encrypt_button, "clicked", G_CALLBACK(decrypt_file), NULL);
    g_signal_connect(save_button, "clicked", G_CALLBACK(save_file), NULL);
    
    //Filtrage des fichiers selectionable
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.txt");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file_chooser_button), filter);

    //Ajout des widgets a la fenetre
    gtk_box_pack_start(GTK_BOX(hbox2), file_chooser_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox2), key_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox2), key_entry, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), input_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), output_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), encrypt_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), endpath_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), save_button, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Affichage de la fenêtre principale
    gtk_widget_show_all(window);
    // Démarrage de la boucle principale GTK
    gtk_main();

    return 0;
}

// Fonction de chiffrement
char* decrypt_vigenere(const gchar *text, const gchar *key) {
    int messageLength = getStringLength(text), keyLength = getStringLength(key);
    int i;
    char newKey[messageLength], *decrypted = malloc(messageLength);
    for(i=0; i<messageLength; ++i) {
        newKey[i] = key[i%keyLength];
        if(newKey[i] >= 'a' && newKey[i] <= 'z') { newKey[i] = newKey[i] - ('a' - 'A'); }
    }
    newKey[i] = '\0';
    
    for(i = 0; i < messageLength; ++i) {
    	char temp = text[i];
    	if(temp >= 'a' && temp <= 'z') { temp = temp - ('a' - 'A'); }
    	if(temp < 'A' || temp > 'Z') { continue; }
        decrypted[i] = (temp - newKey[i]) + 'A';
    }
    decrypted[i] = '\0';
    return decrypted;
}

// Lors du clic sur le bouton "Chiffrer"
void decrypt_file(GtkButton *button, gpointer user_data) {
    //Recupere le texte a chiffrer, et la cle
    const gchar* key = gtk_entry_get_text(GTK_ENTRY(key_entry));
    if (key[0] == '\0') { g_print("Aucunes cle d'entree, le dechiffrement est impossible.\n"); return; }
    const gchar* text = gtk_entry_get_text(GTK_ENTRY(input_entry));
    if (text[0] == '\0') { g_print("Aucuns texte d'entree disponible (vous pouvez ouvrir un fichier), le dechiffrement est impossible.\n"); return; }
    
    //Encrypte et affiche le resultat
    char* decrypted_text = decrypt_vigenere(text, key);
    gtk_entry_set_text(GTK_ENTRY(output_entry), decrypted_text);
    free(decrypted_text);

    g_print("Le fichier a été dechiffré\n");
}

// Lorsqu'un fichier est sélectionné
void file_selected(GtkFileChooserButton *filechooserbutton, gpointer user_data) {
    const gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooserbutton));
    FILE* fptr = fopen(filename, "rb");
    if(fptr == NULL) { return; }
    
    fseek(fptr, 0L, SEEK_END);
    long size = ftell(fptr);
    rewind(fptr);

    char *text = malloc(size+1);
    fread(text, size, 1, fptr);
    text[size-1] = '\0';
    gtk_entry_set_text(GTK_ENTRY(input_entry), text);
    free(text);
    fclose(fptr);
    
    int filenameLength = getStringLength(filename);
    char newfilename[filenameLength+10];
    strcpy(newfilename, filename);
    newfilename[filenameLength-4] = '\0';
    strcat(newfilename, "_encrypted.txt");
    GtkEntry *temp = GTK_ENTRY(endpath_entry);
    gtk_entry_set_text(temp, newfilename);
}

// Lors du clic sur le bouton "Sauvegarder"
void save_file(GtkButton *button, gpointer user_data) {
    //Recupere le chemin
    const gchar *filename = gtk_entry_get_text(GTK_ENTRY(endpath_entry));
    if (filename[0] == '\0') { g_print("No path have been set\n"); }
    
    //Creer et sauvegarde le fichier
    FILE *newfile = fopen(filename, "a");
    const gchar* text = gtk_entry_get_text(GTK_ENTRY(output_entry));
    int textLength = getStringLength(text);
    
    fwrite(gtk_entry_get_text(GTK_ENTRY(output_entry)), 1, textLength, newfile);
    fclose(newfile);
    
    g_print("Le fichier a été sauvegarde\n");
}

int getStringLength(const gchar * text) {
    int messageLength = 0, i = 0;
    char c;
    while((c = text[i++]) != '\0') { messageLength++; }
    return messageLength;
}
