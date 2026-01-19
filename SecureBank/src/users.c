#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <sqlite3.h>

#include "../include/db.h"
#include "../include/users.h"

// Hash SHA-256
void sha256(const char *input, char *output_hex) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input, strlen(input), hash);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(output_hex + (i * 2), "%02x", hash[i]);

    output_hex[64] = '\0';
}

int user_exists(const char *email) {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (db_open(&db) != SQLITE_OK)
        return 0;

    const char *sql = "SELECT COUNT(*) FROM users WHERE email = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);

    int exists = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        exists = sqlite3_column_int(stmt, 0) > 0;

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return exists;
}

int verify_password(const char *email, const char *password) {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (db_open(&db) != SQLITE_OK)
        return 0;

    const char *sql = "SELECT password_hash FROM users WHERE email = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);

    int ok = 0;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *db_hash = sqlite3_column_text(stmt, 0);

        char input_hash[65];
        sha256(password, input_hash);

        if (strcmp((const char*)db_hash, input_hash) == 0)
            ok = 1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return ok;
}

int add_user(const char *email, const char *password) {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (db_open(&db) != SQLITE_OK)
        return 0;

    char hash[65];
    sha256(password, hash);

    const char *sql = "INSERT INTO users (email, password_hash) VALUES (?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hash, -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return result;
}
