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

void send_email_code(const char *email, int code) {
    char command[512];

    snprintf(command, sizeof(command),
        "echo \"Votre code de validation est : %d\" | sendmail %s",
        code, email
    );

    system(command);
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
int c;
while ((c = getchar()) != '\n' && c != EOF) {}

// Vérification du mot de passe 
if (strcmp(password, "test123") != 0) { 
	printf("Mot de passe incorrect.\n");
	
	// vider le buffer pour éviter les lectures parasites
	int c3;
	while ((c3 = getchar()) != '\n' && c3 != EOF) {}

	return 0;
}

// vider le buffer avant de lire l'email
int c2;
while ((c2 = getchar()) != '\n' && c2 != EOF) {}
printf("Email : ");
scanf("%99s", email);

// vider le buffer après scanf
while ((c = getchar()) != '\n' && c != EOF) {}

// Générer le code
srand(time(NULL));
code = generate_code();

// Envoi du code par email
send_email_code(email, code);
    
printf("Un code de validation a été envoyé à %s\n", email);
printf("(DEBUG: code = %d)\n", code);

// Vérification du code
printf("Entrez le code reçu : ");
while (scanf("%d", &user_code) !=1) {
    printf("Entrée invalide. Veuillez entrer un code numérique.\n");
    while ((c = getchar()) != '\n' && c != EOF) {} // vider buffer
    printf("Entrez le code reçu : ");
}

// vider le buffer après lecture
while ((c = getchar()) != '\n' && c != EOF) {} 

if (user_code != code) {
    printf("Code incorrect.\n");
    return 0;
}

printf("Authentification réussie.\n");
return 1;
}
