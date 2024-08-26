#include <sqlite3.h>

// 在 handle_client 函数中调用 database_select 和 database_insert 函数
void *handle_client(void *new_socket) {
    int sock = *(int *)new_socket;
    int valread;
    char buffer[1024] = {0};
    sqlite3 *db;

    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);

    int rc = sqlite3_open("test.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        close(sock);
        pthread_exit(NULL);
    }

    database_insert(db, buffer);
    database_select(db);

    sqlite3_close(db);

    send(sock, "Data inserted and selected from server", strlen("Data inserted and selected from server"), 0);
    printf("Response sent to client\n");
    close(sock);
    pthread_exit(NULL);
}
