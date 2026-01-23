#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sqlite3.h>
#include "../include/db.h"
#include <time.h>

#define DB_PATH "SecureBank/data/securebank.db"

/* ------------------ OUVERTURE DB ------------------ */

int db_open(sqlite3 **db) {
    return sqlite3_open(DB_PATH, db);
}

/* ------------------ INITIALISATION DB ------------------ */

int db_init() {
    sqlite3 *db;
    char *err_msg = NULL;

    system("mkdir -p data");

    if (db_open(&db) != SQLITE_OK) {
        fprintf(stderr, "Erreur ouverture DB: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    const char *sql =
        "CREATE TABLE IF NOT EXISTS users ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " email TEXT UNIQUE NOT NULL,"
        " password_hash TEXT NOT NULL"
        ");"
        "CREATE TABLE IF NOT EXISTS comptes ("
        " email TEXT PRIMARY KEY,"
        " solde REAL DEFAULT 0"
        ");"
        "CREATE TABLE IF NOT EXISTS transactions ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " email TEXT,"
        " type TEXT,"
        " montant REAL,"
        " date TEXT DEFAULT CURRENT_TIMESTAMP"
        ");"
	"CREATE TABLE IF NOT EXISTS limites_journalieres ("
   	" email TEXT,"
    	" date TEXT,"
    	" depot REAL DEFAULT 0,"
    	" retrait REAL DEFAULT 0,"
    	" virement REAL DEFAULT 0,"
    	" PRIMARY KEY (email, date)"
	");"
	"CREATE TABLE IF NOT EXISTS virements_programmes ("
    	" id INTEGER PRIMARY KEY AUTOINCREMENT,"
    	" email_source TEXT NOT NULL,"
    	" email_dest TEXT NOT NULL,"
    	" montant REAL NOT NULL,"
    	" date_exec TEXT NOT NULL,"  // Format YYYY-MM-DD
   	" heure_exec TEXT NOT NULL"  // Format HH:MM 
	");";


    if (sqlite3_exec(db, sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Erreur SQL: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_close(db);
    return 1;
}

void get_date_aujourdhui(char *buffer, size_t size) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, size, "%Y-%m-%d", tm_info);
}

void charger_limites(const char *email, const char *date,
                     float *depot, float *retrait, float *virement) {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    *depot = *retrait = *virement = 0;

    if (db_open(&db) != SQLITE_OK)
        return;

    const char *sql =
        "SELECT depot, retrait, virement FROM limites_journalieres "
        "WHERE email = ? AND date = ?;";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, date, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        *depot = sqlite3_column_double(stmt, 0);
        *retrait = sqlite3_column_double(stmt, 1);
        *virement = sqlite3_column_double(stmt, 2);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void maj_limites(const char *email, const char *date,
                 float depot, float retrait, float virement) {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (db_open(&db) != SQLITE_OK)
        return;

    const char *sql =
        "INSERT INTO limites_journalieres (email, date, depot, retrait, virement) "
        "VALUES (?, ?, ?, ?, ?) "
        "ON CONFLICT(email, date) DO UPDATE SET "
        "depot = excluded.depot, "
        "retrait = excluded.retrait, "
        "virement = excluded.virement;";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, date, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, depot);
    sqlite3_bind_double(stmt, 4, retrait);
    sqlite3_bind_double(stmt, 5, virement);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


/* ------------------ SOLDE ------------------ */

float charger_solde(const char *email) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    float solde = 0.0;

    if (db_open(&db) != SQLITE_OK)
        return 0.0;

    const char *sql = "SELECT solde FROM comptes WHERE email = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        solde = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return solde;
}

void sauvegarder_solde(const char *email, float solde) {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (db_open(&db) != SQLITE_OK)
        return;

    const char *sql =
        "INSERT INTO comptes (email, solde) VALUES (?, ?) "
        "ON CONFLICT(email) DO UPDATE SET solde = excluded.solde;";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, solde);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

/* ------------------ TRANSACTIONS ------------------ */

void ajouter_transaction(const char *email, const char *type, float montant) {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (db_open(&db) != SQLITE_OK)
        return;

    const char *sql =
        "INSERT INTO transactions (email, type, montant) VALUES (?, ?, ?);";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, type, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, montant);

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void afficher_historique(const char *email) {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (db_open(&db) != SQLITE_OK) {
        printf("Erreur : impossible d'accéder à la base.\n");
        return;
    }

    const char *sql =
        "SELECT type, montant, date FROM transactions "
        "WHERE email = ? ORDER BY date DESC;";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);

    printf("\n=== Historique des transactions ===\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *type = (const char*)sqlite3_column_text(stmt, 0);
        float montant = sqlite3_column_double(stmt, 1);
        const char *date = (const char*)sqlite3_column_text(stmt, 2);

        printf("%s : %.2f $ (%s)\n", type, montant, date);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

int programmer_virement(const char *email_source, const char *email_dest, float montant,
                        const char *date_exec, const char *heure_exec) 
{
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO virements_programmes (email_source, email_dest, montant, date_exec, heure_exec) VALUES (?, ?, ?, ?, ?)";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
        return 0;

    sqlite3_bind_text(stmt, 1, email_source, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email_dest, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, montant);
    sqlite3_bind_text(stmt, 4, date_exec, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, heure_exec, -1, SQLITE_STATIC);

    int ok = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return ok;
}

void executer_virements_programmes() {
    char date[20], heure[6];
    get_date_aujourdhui(date, sizeof(date));
    get_heure_actuelle(heure, sizeof(heure));

    sqlite3_stmt *stmt;
    const char *sql = "SELECT id, email_source, email_dest, montant FROM virements_programmes WHERE date_exec = ? AND heure_exec <= ?";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, date, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, heure, -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *src = (const char*)sqlite3_column_text(stmt, 1);
        const char *dst = (const char*)sqlite3_column_text(stmt, 2);
        float montant = sqlite3_column_double(stmt, 3);

        // Exécuter le virement réel
        float solde_src = charger_solde(src);
        float solde_dst = charger_solde(dst);

        if (solde_src >= montant) {
            sauvegarder_solde(src, solde_src - montant);
            sauvegarder_solde(dst, solde_dst + montant);
            ajouter_transaction(src, "Virement programmé envoyé", montant);
            ajouter_transaction(dst, "Virement programmé reçu", montant);
        }

        // Supprimer le virement programmé
        sqlite3_exec(db, "DELETE FROM virements_programmes WHERE id = ?", NULL, NULL, NULL);
    }

    sqlite3_finalize(stmt);
}
