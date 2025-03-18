#include "Configuration.h"
#include <libpq-fe.h>

Configuration::Configuration(int id, DatabaseHandler* dbHandler)
    : id(id), dbHandler(dbHandler), name(""), description("") {}

Configuration::~Configuration() {
    // Освобождаем ресурсы, если необходимо
}

bool Configuration::load() {
    if (!dbHandler) {
        return false;
    }
    QString query = QString("SELECT name, description FROM configurations WHERE id = %1").arg(id);
    PGresult* res = dbHandler->executeQuery(query);
    if (!res || PQntuples(res) == 0) {
        return false;
    }
    name = QString(PQgetvalue(res, 0, 0));
    description = QString(PQgetvalue(res, 0, 1));
    PQclear(res);
    return true;
}

QString Configuration::getName() const {
    return name;
}

QString Configuration::getDescription() const {
    return description;
}
