# Projet Sécurité - Virus Compagnon & Utilitaires GTK

## Présentation

Ce projet a été réalisé dans le cadre du cours de Sécurité. Il illustre la conception d’un **virus compagnon** en langage C sous Linux, ainsi que plusieurs utilitaires graphiques développés avec **GTK+**.  
Le virus se propage en infectant les exécutables présents dans le répertoire courant, tout en offrant une fonctionnalité légitime pour dissimuler son comportement malveillant.

## Structure du Projet

### Virus

- **virus.c** : Contient le code du virus compagnon qui :
  - Cherche les exécutables ELF dans le répertoire courant.
  - Vérifie qu'ils ne sont pas déjà infectés.
  - Les renomme en `.nomprogramme.old`.
  - Copie son propre code dans le fichier d'origine.

- **mediaplayer.c** : Application légitime servant de **cheval de Troie**.
  - Affiche les images `.jpg` du dossier courant.
  - Contient et déclenche le virus.

---

### Utilitaires GTK

- **chronometre.c** : Chronomètre simple avec démarrage et arrêt.
- **convertisseur.c** : Convertisseur de température (Celsius ↔ Fahrenheit).
- **informations.c** : Affiche les informations CPU et RAM de la machine.
- **chiffrement_vigenere.c** : Application pour chiffrer un fichier texte avec l'algorithme de Vigenère.
- **dechiffrement_vigenere.c** : Déchiffre un fichier texte chiffré via Vigenère.

---

## Fichiers Scripts

- **compile.sh** : Script facilitant la compilation des différents utilitaires et du virus.
- **clean.sh** : Script de nettoyage des fichiers générés.

---

## Compilation

Pour compiler les différents modules, exécuter :
```bash
./compile.sh
