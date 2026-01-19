#ifndef DB_H
#define DB_H

#include <sqlite3.h>

// Ouvre la base de données (data/securebank.db)
int db_open(sqlite3 **db);

// Initialise la base : crée la table users si elle n'existe pas
int db_init();

// Charger le solde courant
float charger_solde(const char *email);

// Sauvegarder le solde à la fin de la session
void sauvegarder_solde(const char *email, float solde);

// Ajouter les transactions
void ajouter_transaction(const char *email, const char *type, float montant);

// Afficher tout l'historique des transactions
void afficher_historique(const char *email);


#endif
