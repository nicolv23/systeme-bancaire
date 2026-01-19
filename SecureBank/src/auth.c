#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include "auth.h"

extern void disableEcho();
extern void enableEcho();

static int generate_code() {
    return rand() % 900000 + 100000; // code entre 100000 et 999999
}

int authenticate_user() {
    char password[50];
    char email[100];
    int i = 0;
    char ch;
    int code, user_code;

    printf("Mot de passe : "); 
    disableEcho(); // masque l'affichage 
    
    while ((ch = getchar()) != '\n' && ch != EOF) { 
	if (i < 49) { 
		password[i++] = ch; 
		printf("*"); // affiche une étoile 
	} 
} 
password[i] = '\0'; 

enableEcho(); // réactive l'affichage 
printf("\n"); 

// Vérification du mot de passe 
if (strcmp(password, "test123") != 0) { 
printf("Mot de passe incorrect.\n"); 
return 0; }

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
