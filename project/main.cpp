#include "mainwindow.h"

#include <QApplication>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include "DatabaseHandler.h"
#include "Configuration.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseHandler dbHandler("dbname=testdb user=postgres password=sa");
    if (!dbHandler.connect()) {
        QMessageBox::critical(nullptr, "Ошибка", "Отсутствует подключение к БД");
        return -1;
    }

    MainWindow window;

    QVBoxLayout *leftUpLayout = window.findChild<QVBoxLayout*>("left_up");

    QListWidget* configList = new QListWidget(&window);
    PGresult* res = dbHandler.executeQuery("SELECT id, name FROM configurations");
    if (res) {
        for (int i = 0; i < PQntuples(res); i++) {
            QString itemText = QString(PQgetvalue(res, i, 1));
            QListWidgetItem* item = new QListWidgetItem(itemText, configList);
            item->setData(Qt::UserRole, QString(PQgetvalue(res, i, 0)).toInt());
        }
        PQclear(res);
    }

    leftUpLayout->addWidget(configList);

    QPushButton* loadButton = new QPushButton("Загрузить конфигурацию", &window);
    leftUpLayout->addWidget(loadButton);

    QObject::connect(loadButton, &QPushButton::clicked, [&]() {
        QListWidgetItem* selectedItem = configList->currentItem();
        if (!selectedItem) {
            QMessageBox::warning(&window, "Внимание", "Выберите конфигурацию");
            return;
        }
        int configId = selectedItem->data(Qt::UserRole).toInt();
        Configuration config(configId, &dbHandler);
        if (!config.load()) {
            QMessageBox::warning(&window, "Внимание", "Не получилось загрузить конфигурацию");
            return;
        }
        QMessageBox::information(&window, "Конфигурация загружена!\n",
                                 QString("Название: %1\nОписание: %2").arg(config.getName()).arg(config.getDescription()));
    });

    window.show();
    return a.exec();
}
