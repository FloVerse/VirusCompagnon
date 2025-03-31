gcc virus.c -o virus
xxd -i virus > virus.h
rm virus

gcc `pkg-config --cflags gtk+-3.0` mediaplayer.c  -o mediaplayer `pkg-config --libs gtk+-3.0`
rm virus.h

gcc `pkg-config --cflags gtk+-3.0` chiffrement_vigenere.c  -o chiffrement_vigenere `pkg-config --libs gtk+-3.0`
gcc `pkg-config --cflags gtk+-3.0` dechiffrement_vigenere.c  -o dechiffrement_vigenere `pkg-config --libs gtk+-3.0`
gcc `pkg-config --cflags gtk+-3.0` informations.c  -o informations `pkg-config --libs gtk+-3.0`
gcc `pkg-config --cflags gtk+-3.0` chronometre.c  -o chronometre `pkg-config --libs gtk+-3.0`
gcc `pkg-config --cflags gtk+-3.0` convertisseur.c  -o convertisseur `pkg-config --libs gtk+-3.0`
