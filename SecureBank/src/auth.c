#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>
#include <termios.h>
#include <unistd.h>
#include "auth.h"
#include "../include/users.h"
#include "../include/db.h"
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

// Génération du code 2FA
static int generate_code() {
    return rand() % 900000 + 100000;
}

/* ---------------- SMTP libcurl ---------------- */

struct upload_status {
    int lines_read;
};

static const char *payload_text[3];

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp) {
    (void)size;
    (void)nmemb;

    struct upload_status *upload_ctx = (struct upload_status *)userp;

    const char *data = payload_text[upload_ctx->lines_read];

    if (data) {
        size_t len = strlen(data);
        memcpy(ptr, data, len);
        upload_ctx->lines_read++;
        return len;
    }

    return 0;
}

void send_email_code(const char *dest_email, int code) {
    CURL *curl;
    CURLcode res = CURLE_OK;

    char subject[128];
    char body[256];
    char to_header[256];

    snprintf(subject, sizeof(subject), "Subject: Code de validation SecureBank\r\n");
    snprintf(body, sizeof(body), "Votre code de validation est : %d\r\n", code);
    snprintf(to_header, sizeof(to_header), "To: <%s>\r\n", dest_email);

    payload_text[0] = subject;
    payload_text[1] = body;
    payload_text[2] = NULL;

    struct upload_status upload_ctx = { 0 };

    char *smtp_user = getenv("SMTP_USER");
    char *smtp_pass = getenv("SMTP_PASS");

    if (!smtp_user || !smtp_pass) {
        printf("Erreur : SMTP_USER ou SMTP_PASS non définies.\n");
        return;
    }

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_USERNAME, smtp_user);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, smtp_pass);

        curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.office365.com:587");
        curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

        char from_header[256];
        snprintf(from_header, sizeof(from_header), "<%s>", smtp_user);
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from_header);

        struct curl_slist *recipients = NULL;
        recipients = curl_slist_append(recipients, to_header);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            printf("Erreur SMTP: %s\n", curl_easy_strerror(res));
        }

        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }
}

/* ---------------- Authentification ---------------- */

int authenticate_user() {
    char password[50];
    char email[100];
    int i = 0;
    char ch;
    int code, user_code;
    int c;

    // --- Étape 1 : Email ---
    printf("Email : ");
    scanf("%99s", email);

    while ((c = getchar()) != '\n' && c != EOF) {}

    // --- Étape 2 : Mot de passe masqué ---
    printf("Mot de passe : ");
    disableEcho();

    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (i < 49) {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';

    enableEcho();
    printf("\n");

    while ((c = getchar()) != '\n' && c != EOF) {}

    // --- Étape 3 : Vérification SQLite ---
    if (!user_exists(email)) {
        printf("Cet utilisateur n'existe pas.\n");
        return 0;
    }

    if (!verify_password(email, password)) {
        printf("Mot de passe incorrect.\n");
        return 0;
    }

    printf("Mot de passe correct.\n");

    // --- Étape 4 : Code 2FA ---
    srand(time(NULL));
    code = generate_code();

    printf("Envoi du code à %s...\n", email);
    send_email_code(email, code);
    printf("Code envoyé.\n");

    printf("Entrez le code reçu : ");
    while (scanf("%d", &user_code) != 1) {
        printf("Entrée invalide. Veuillez entrer un code numérique.\n");
        while ((c = getchar()) != '\n' && c != EOF) {}
        printf("Entrez le code reçu : ");
    }

    while ((c = getchar()) != '\n' && c != EOF) {}

    if (user_code != code) {
        printf("Code incorrect.\n");
        return 0;
    }

    printf("Authentification réussie.\n");
    return 1;
}
