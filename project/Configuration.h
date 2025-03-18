#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QString>
#include "DatabaseHandler.h"

class Configuration {
public:
    Configuration(int id, DatabaseHandler* dbHandler);
    ~Configuration();

    bool load();
    QString getName() const;
    QString getDescription() const;

private:
    int id;
    QString name;
    QString description;
    DatabaseHandler* dbHandler;
};

#endif // CONFIGURATION_H
