#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/db.h"

#define DB_PATH "data/securebank.db"

int db_open(sqlite3 **db) {
    return sqlite3_open(DB_PATH, db);
}

int db_init() {
    sqlite3 *db;
    char *err_msg = NULL;

    // Créer le dossier data/ si nécessaire
    system("mkdir -p data");

    // Ouvrir la base
    if (db_open(&db) != SQLITE_OK) {
        fprintf(stderr, "Erreur ouverture DB: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    // Requêtes SQL concaténées dans UNE SEULE chaîne
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

    // Exécution des requêtes
    if (sqlite3_exec(db, sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "Erreur SQL: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_close(db);
    return 1;
}
