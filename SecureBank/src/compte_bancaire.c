#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "auth.h"

#define SOLDE_FICHIER "solde.enc"
#define HISTORIQUE_FICHIER "historique.txt"
#define CLE "CLE123" // clé de chiffrement XOR

float solde = 0.0;

// Désactiver l'affichage du terminal
void disableEcho() {
  struct termios t;
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

// Réactiver l'affichage du terminal
void enableEcho() {
  struct termios t;
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag |= ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

// Nettoyer le buffer
void viderBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
  }
}

// Lire un entier avec validation
int lireEntier() {
  int valeur;
  while (scanf("%d", &valeur) != 1) {
    printf("Entrée invalide. Veuillez entrer un nombre entier.\n");
    viderBuffer();
    printf("Votre choix : ");
  }
  viderBuffer();
  return valeur;
}

// Lire un montant avec validation
float lireMontant() {
  float montant;
  while (scanf("%f", &montant) != 1) {
    printf("Entrée invalide. Veuillez entrer un montant numérique.\n");
    viderBuffer();
    printf("Montant : ");
  }
  viderBuffer();
  return montant;
}

// Chiffrement XOR simple
void xorBuffer(char *data, int len) {
  int keyLen = strlen(CLE);
  for (int i = 0; i < len; i++) {
    data[i] ^= CLE[i % keyLen];
  }
}

// Sauvegarder le solde chiffré
void sauvegarderSolde() {
  FILE *f = fopen(SOLDE_FICHIER, "wb");
  if (!f) {
    printf("Erreur : impossible de sauvegarder le solde.\n");
    return;
  }

  char buffer[50];
  snprintf(buffer, sizeof(buffer), "%.2f", solde);

  xorBuffer(buffer, strlen(buffer));
  fwrite(buffer, 1, strlen(buffer), f);

  fclose(f);
}

// Charger le solde chiffré
void chargerSolde() {
  FILE *f = fopen(SOLDE_FICHIER, "rb");
  if (!f) {
    solde = 0.0;
    return;
  }

  char buffer[50] = {0};
  int len = fread(buffer, 1, sizeof(buffer), f);
  fclose(f);

  xorBuffer(buffer, len);
  solde = atof(buffer);
}

// Ajouter une transaction à l'historique
void ajouterHistorique(const char *message, float montant) {
  FILE *f = fopen(HISTORIQUE_FICHIER, "a");
  if (!f)
    return;
  fprintf(f, "%s : %.2f $\n", message, montant);
  fclose(f);
}

void afficherSolde() { printf("\nSolde actuel : %.2f $\n", solde); }

void deposer() {
  printf("Montant à déposer : ");
  float montant = lireMontant();

  if (montant <= 0) {
    printf("Erreur : le montant doit être positif.\n");
    return;
  }

  solde += montant;
  sauvegarderSolde();
  ajouterHistorique("Dépôt", montant);

  printf("Dépôt de %.2f $ effectué.\n", montant);
}

void retirer() {
  printf("Montant à retirer : ");
  float montant = lireMontant();

  if (montant <= 0) {
    printf("Erreur : le montant doit être positif.\n");
    return;
  }

  if (montant > solde) {
    printf("Erreur : fonds insuffisants. Solde actuel : %.2f $\n", solde);
    return;
  }

  solde -= montant;
  sauvegarderSolde();
  ajouterHistorique("Retrait", montant);

  printf("Retrait de %.2f $ effectué.\n", montant);
}

void afficherHistorique() {
  FILE *f = fopen(HISTORIQUE_FICHIER, "r");
  if (!f) {
    printf("Aucune transaction enregistrée.\n");
    return;
  }

  printf("\n=== Historique des transactions ===\n");

  char ligne[256];
  while (fgets(ligne, sizeof(ligne), f)) {
    printf("%s", ligne);
  }

  fclose(f);
}

int main() {
  printf("=== Système bancaire sécurisé ===\n");

  // Authentification code par email
  if( !authenticate_user()) {
      printf("Accès refusé.\n");
      return 1;
  }
  
  printf("Accès autorisé.\n");

  // Charger le solde chiffré
  chargerSolde();

  int choix = 0;

  do {
    printf("\n--- Menu principal ---\n");
    printf("1. Afficher le solde\n");
    printf("2. Déposer de l'argent\n");
    printf("3. Retirer de l'argent\n");
    printf("4. Afficher l'historique\n");
    printf("5. Quitter\n");
    printf("Votre choix : ");

    choix = lireEntier();

    switch (choix) {
    case 1:
      afficherSolde();
      break;
    case 2:
      deposer();
      break;
    case 3:
      retirer();
      break;
    case 4:
      afficherHistorique();
      break;
    case 5:
      printf("Merci d'avoir utilisé le système bancaire.\n");
      break;
    default:
      printf("Choix invalide.\n");
    }

  } while (choix != 5);

  return 0;
}
