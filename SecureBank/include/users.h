#ifndef USERS_H
#define USERS_H

// Vérifie si un utilisateur existe (email)
int user_exists(const char *email);

// Vérifie si le mot de passe correspond
int verify_password(const char *email, const char *password);

// Ajoute un utilisateur (email + mot de passe hashé)
int add_user(const char *email, const char *password);

#endif
