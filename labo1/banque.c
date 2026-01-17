#include <stdio.h>

float solde = 0.0;

void afficherSolde() {
    printf("\nSolde actuel : %.2f $\n", solde);
}

void deposer(float montant) {
    if (montant <= 0) {
        printf("Le montant doit être positif.\n");
        return;
    }
    solde += montant;
    printf("Dépôt de %.2f $ effectué.\n", montant);
}

void retirer(float montant) {
    if (montant <= 0) {
        printf("Le montant doit être positif.\n");
        return;
    }
    if (montant > solde) {
        printf("Fonds insuffisants. Solde actuel : %.2f $\n", solde);
        return;
    }
    solde -= montant;
    printf("Retrait de %.2f $ effectué.\n", montant);
}

int main() {
    int choix = 0;
    float montant = 0.0;

    do {
        printf("\n=== Système bancaire ===\n");
        printf("1. Afficher le solde\n");
        printf("2. Déposer de l'argent\n");
        printf("3. Retirer de l'argent\n");
        printf("4. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                afficherSolde();
                break;
            case 2:
                printf("Montant à déposer : ");
                scanf("%f", &montant);
                deposer(montant);
                break;
            case 3:
                printf("Montant à retirer : ");
                scanf("%f", &montant);
                retirer(montant);
                break;
            case 4:
                printf("Merci d'avoir utilisé le système bancaire!\n");
                break;
            default:
                printf("Choix invalide.\n");
        }

    } while (choix != 4);

    return 0;
}
