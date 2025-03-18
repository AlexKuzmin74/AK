#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QString>
#include <libpq-fe.h>

class DatabaseHandler {
public:
    DatabaseHandler(const QString& connectionString);
    ~DatabaseHandler();

    bool connect();
    void disconnect();
    PGresult* executeQuery(const QString& query);

private:
    QString connectionString;
    PGconn* connection;
};

#endif // DATABASEHANDLER_H
