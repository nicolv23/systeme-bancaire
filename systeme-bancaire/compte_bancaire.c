#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASSWORD "test123"
#define SOLDE_FICHIER "solde.txt"
#define HISTORIQUE_FICHIER "historique.txt"

float solde = 0.0;

// Nettoyer le buffer
void viderBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
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

// Charger le solde depuis un fichier
void chargerSolde() {
    FILE *f = fopen(SOLDE_FICHIER, "r");
    if (f == NULL) {
        solde = 0.0;
        return;
    }
    fscanf(f, "%f", &solde);
    fclose(f);
}

// Sauvegarder le solde dans un fichier
void sauvegarderSolde() {
    FILE *f = fopen(SOLDE_FICHIER, "w");
    if (f == NULL) {
        printf("Erreur : impossible de sauvegarder le solde.\n");
        return;
    }
    fprintf(f, "%.2f", solde);
    fclose(f);
}

// Ajouter une transaction à l'historique
void ajouterHistorique(const char *message, float montant) {
    FILE *f = fopen(HISTORIQUE_FICHIER, "a");
    if (f == NULL) {
        printf("Erreur : impossible d'écrire dans l'historique.\n");
        return;
    }
    fprintf(f, "%s : %.2f $\n", message, montant);
    fclose(f);
}

void afficherSolde() {
    printf("\nSolde actuel : %.2f $\n", solde);
}

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
    if (f == NULL) {
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

int demanderMotDePasse() {
    char entree[50];

    printf("Mot de passe : ");
    scanf("%49s", entree);
    viderBuffer();

    if (strcmp(entree, PASSWORD) == 0) {
        return 1;
    }

    return 0;
}

int main() {
    printf("=== Système bancaire sécurisé ===\n");

    // Mot de passe
    int essais = 3;
    while (essais > 0) {
        if (demanderMotDePasse()) {
            printf("Accès autorisé.\n");
            break;
        }
        essais--;
        printf("Mot de passe incorrect. Il reste %d essais.\n", essais);
    }

    if (essais == 0) {
        printf("Accès refusé.\n");
        return 1;
    }

    // Charger le solde
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
            case 1: afficherSolde(); break;
            case 2: deposer(); break;
            case 3: retirer(); break;
            case 4: afficherHistorique(); break;
            case 5: printf("Merci d'avoir utilisé le système bancaire.\n"); break;
            default: printf("Choix invalide.\n");
        }

    } while (choix != 5);

    return 0;
}
