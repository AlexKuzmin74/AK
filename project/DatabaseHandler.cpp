#include "DatabaseHandler.h"
#include <libpq-fe.h>
#include <QDebug>

DatabaseHandler::DatabaseHandler(const QString& connectionString)
    : connectionString(connectionString), connection(nullptr) {}

DatabaseHandler::~DatabaseHandler() {
    disconnect();
}

bool DatabaseHandler::connect() {
    if (connection) {
        return true;
    }
    connection = PQconnectdb(connectionString.toStdString().c_str());
    if (PQstatus(connection) != CONNECTION_OK) {
        qDebug() << "Ошибка подключения:" << PQerrorMessage(connection);
        PQfinish(connection);
        connection = nullptr;
        return false;
    }
    return true;
}

void DatabaseHandler::disconnect() {
    if (connection) {
        PQfinish(connection);
        connection = nullptr;
    }
}

PGresult* DatabaseHandler::executeQuery(const QString& query) {
    if (!connection) {
        return nullptr;
    }
    PGresult* res = PQexec(connection, query.toStdString().c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK && PQresultStatus(res) != PGRES_COMMAND_OK) {
        qDebug() << "Ошибка выполнения запроса:" << PQerrorMessage(connection);
        PQclear(res);
        return nullptr;
    }
    return res;
}
