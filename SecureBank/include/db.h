#ifndef DB_H
#define DB_H

#include <sqlite3.h>

// Ouvre la base de données (data/securebank.db)
int db_open(sqlite3 **db);

// Initialise la base : crée les tables users, comptes, transactions
int db_init();

// Charger le solde courant depuis SQLite
float charger_solde(const char *email);

// Sauvegarder le solde dans SQLite
void sauvegarder_solde(const char *email, float solde);

// Ajouter une transaction dans SQLite
void ajouter_transaction(const char *email, const char *type, float montant);

// Afficher l'historique des transactions depuis SQLite
void afficher_historique(const char *email);

#endif
