#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sqlite3.h>
#include "../include/db.h"

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
