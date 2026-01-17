# Système bancaire en C

Ce projet est une application en ligne de commande permettant de gérer un solde bancaire, d’effectuer des dépôts et retraits, et d’enregistrer un historique des opérations. Les données sont conservées dans des fichiers locaux.

## Fonctionnalités

- Consultation du solde
- Dépôt d’argent
- Retrait avec vérification du solde
- Historique des opérations
- Stockage du solde dans un fichier chiffré

## Structure du projet

- banque_secure.c       # Code source
- solde.enc             # Solde chiffré
- historique.txt        # Historique des opérations
- README.md             # Documentation

## Compilation

- gcc banque_secure.c -o banque

## Exécution

- ./banque

## Améliorations possibles

- Makefile
- Tests unitaires
- Chiffrement plus robuste
- Interface utilisateur améliorée
