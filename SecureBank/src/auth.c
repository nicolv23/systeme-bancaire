#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "auth.h"

static int generate_code() {
    return rand() % 900000 + 100000; // code entre 100000 et 999999
}

int authenticate_user() {
    char password[50];
    char email[100];
    int code, user_code;

    printf("Mot de passe : ");
    scanf("%49s", password);

    // Vérification du mot de passe (à améliorer plus tard)
    if (strcmp(password, "1234") != 0) {
        printf("Mot de passe incorrect.\n");
        return 0;
    }

    printf("Email : ");
    scanf("%99s", email);

    srand(time(NULL));
    code = generate_code();

    // Simulation d’envoi du code
    printf("Un code de validation a été envoyé à %s\n", email);
    printf("(DEBUG: code = %d)\n", code);

    printf("Entrez le code reçu : ");
    scanf("%d", &user_code);

    if (user_code != code) {
        printf("Code incorrect.\n");
        return 0;
    }

    printf("Authentification réussie.\n");
    return 1;
}
