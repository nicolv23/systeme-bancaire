#ifndef DB_H
#define DB_H

#include <sqlite3.h>

// Ouvre la base de données (data/securebank.db)
int db_open(sqlite3 **db);

// Initialise la base : crée la table users si elle n'existe pas
int db_init();

#endif
