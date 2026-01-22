#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "auth.h"
#include "users.h"
#include "db.h"

float solde = 0.0;

/* ------------------ UTILITAIRES ------------------ */

void viderBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

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

/* ------------------ FONCTIONS BANCAIRES (SQLite) ------------------ */

void afficherSolde() {
    printf("\nSolde actuel : %.2f $\n", solde);
}

void deposer(const char *email) {
    printf("Montant à déposer : ");
    float montant = lireMontant();

    if (montant <= 0) {
        printf("Erreur : le montant doit être positif.\n");
        return;
    }

    solde += montant;
    sauvegarder_solde(email, solde);
    ajouter_transaction(email, "Dépôt", montant);

    printf("Dépôt de %.2f $ effectué.\n", montant);
}

void retirer(const char *email) {
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
    sauvegarder_solde(email, solde);
    ajouter_transaction(email, "Retrait", montant);

    printf("Retrait de %.2f $ effectué.\n", montant);
}

void afficherHistorique(const char *email) {
    afficher_historique(email);
}

void virement(const char *email_source) {
    char email_dest[100];
    float montant;

    printf("Email du destinataire : ");
    scanf("%99s", email_dest);
    viderBuffer();

    if (strcmp(email_source, email_dest) == 0) {
        printf("Erreur : vous ne pouvez pas vous envoyer un virement à vous-même.\n");
        return;
    }

    if (!user_exists(email_dest)) {
        printf("Erreur : ce destinataire n'existe pas.\n");
        return;
    }

    printf("Montant à transférer : ");
    montant = lireMontant();

    if (montant <= 0) {
        printf("Erreur : le montant doit être positif.\n");
        return;
    }

    float solde_source = solde;

    if (montant > solde_source) {
        printf("Erreur : fonds insuffisants. Solde actuel : %.2f $\n", solde_source);
        return;
    }

    /* --- Débit du compte source --- */
    solde_source -= montant;
    solde = solde_source;
    sauvegarder_solde(email_source, solde_source);
    ajouter_transaction(email_source, "Virement envoyé", montant);

    /* --- Crédit du compte destinataire --- */
    float solde_dest = charger_solde(email_dest);
    solde_dest += montant;
    sauvegarder_solde(email_dest, solde_dest);
    ajouter_transaction(email_dest, "Virement reçu", montant);

    printf("Virement de %.2f $ envoyé à %s.\n", montant, email_dest);
}


/* ------------------ PROGRAMME PRINCIPAL ------------------ */

int main(int argc, char *argv[]) {

    if (!db_init()) {
        printf("Erreur : impossible d'initialiser la base de données.\n");
        return 1;
    }

    /* Mode création d'utilisateur via Makefile */
    if (argc == 4 && strcmp(argv[1], "--add-user") == 0) {
        if (add_user(argv[2], argv[3])) {
            printf("Utilisateur %s créé avec succès.\n", argv[2]);
        } else {
            printf("Erreur : impossible de créer l'utilisateur.\n");
        }
        return 0;
    }

    printf("=== Système bancaire sécurisé ===\n");

    char email[100];

    /* Authentification */
    if (!authenticate_user(email)) {
        printf("Accès refusé.\n");
        return 1;
    }

    printf("Accès autorisé.\n");

    /* Charger le solde depuis SQLite */
    solde = charger_solde(email);

    int choix = 0;

    do {
        printf("\n--- Menu principal ---\n");
        printf("1. Afficher le solde\n");
        printf("2. Déposer de l'argent\n");
        printf("3. Retirer de l'argent\n");
        printf("4. Afficher l'historique\n");
        printf("5. Quitter\n");
        printf("6. Créer un nouvel utilisateur\n");
	printf("7. Faire un virement bancaire\n")
        printf("Votre choix : ");

        choix = lireEntier();

        switch (choix) {

        case 1:
            afficherSolde();
            break;

        case 2:
            deposer(email);
            break;

        case 3:
            retirer(email);
            break;

        case 4:
            afficherHistorique(email);
            break;

        case 5:
            printf("Merci d'avoir utilisé le système bancaire.\n");
            break;

        case 6: {
            char new_email[100];
            char password[50];

            printf("Veuillez ajouter votre email : ");
            scanf("%99s", new_email);
            viderBuffer();

            printf("Veuillez ajouter un mot de passe : ");
            scanf("%49s", password);
            viderBuffer();

            if (add_user(new_email, password)) {
                printf("Utilisateur créé avec succès.\n");
            } else {
                printf("Erreur : impossible de créer cet utilisateur.\n");
            }
            break;
        }

	case 7:
	    virement(email);
	    break;

        default:
            printf("Choix invalide.\n");
        }

    } while (choix != 5);

    return 0;
}
