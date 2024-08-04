// sqlite3_example.c
#include <stdio.h>
#include <sqlite3.h>

int database_select(sqlite3 *db) {
    char *err_msg = 0;
    sqlite3_stmt *res;

    int rc = sqlite3_prepare_v2(db, "SELECT * FROM test_table", -1, &res, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    int step = sqlite3_step(res);
    while (step == SQLITE_ROW) {
        printf("%s: %s\n", sqlite3_column_text(res, 0), sqlite3_column_text(res, 1));
        step = sqlite3_step(res);
    }

    sqlite3_finalize(res);
    return 0;
}

int database_insert(sqlite3 *db, const char *data) {
    char *err_msg = 0;
    char sql[256];

    sprintf(sql, "INSERT INTO test_table VALUES('%s', 'Some data');", data);

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    return 0;
}

int main(void) {
    sqlite3 *db;
    int rc = sqlite3_open("test.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    database_insert(db, "Hello");
    database_select(db);

    sqlite3_close(db);
    return 0;
}
