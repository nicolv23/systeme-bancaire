#include <stdio.h>
#include <stdlib.h>

// Solde du compte
float solde = 0.0;

// Nettoyer le buffer d'entrée en cas d'erreur
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

// Lire un montant (float) avec validation
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
    printf("Retrait de %.2f $ effectué.\n", montant);
}

int main() {
    int choix = 0;

    printf("=== Bienvenue dans le système bancaire ===\n");

    do {
        printf("\n--- Menu principal ---\n");
        printf("1. Afficher le solde\n");
        printf("2. Déposer de l'argent\n");
        printf("3. Retirer de l'argent\n");
        printf("4. Quitter\n");
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
                printf("Merci d'avoir utilisé le système bancaire. À bientôt!\n");
                break;
            default:
                printf("Erreur : choix invalide. Veuillez entrer un nombre entre 1 et 4.\n");
        }

    } while (choix != 4);

    return 0;
}

