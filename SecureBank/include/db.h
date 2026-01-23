#ifndef DB_H
#define DB_H

#include <sqlite3.h>

// Ouvre la base de données (data/securebank.db)
int db_open(sqlite3 **db);

// Initialise la base : crée les tables users, comptes, transactions
int db_init();

int programmer_virement(const char *email_source, const char *email_dest, float montant,
                        const char *date_exec, const char *heure_exec);

void executer_virements_programmes();

void get_heure_actuelle(char *buffer, size_t size);

// Charger le solde courant depuis SQLite
float charger_solde(const char *email);

// Sauvegarder le solde dans SQLite
void sauvegarder_solde(const char *email, float solde);

// Ajouter une transaction dans SQLite
void ajouter_transaction(const char *email, const char *type, float montant);

// Afficher l'historique des transactions depuis SQLite
void afficher_historique(const char *email);

// Limites quotidiennes
void get_date_aujourdhui(char *buffer, size_t size);
void charger_limites(const char *email, const char *date,
                     float *depot, float *retrait, float *virement);
void maj_limites(const char *email, const char *date,
                 float depot, float retrait, float virement);

// Constantes limites
#define LIMITE_DEPOT_MAX 1000
#define LIMITE_RETRAIT_MAX 500
#define LIMITE_VIREMENT_MAX 300


#endif
