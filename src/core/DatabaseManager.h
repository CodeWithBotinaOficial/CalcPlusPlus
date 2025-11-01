#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    bool openDatabase(const QString &dbPath);
    void closeDatabase();
    bool createHistoryTable();
    bool addHistoryEntry(const QString &expression, const QString &result);
    QList<QPair<QString, QString>> getHistory();
    bool clearHistory();

private:
    QSqlDatabase db;
    QString databasePath;

    void logError(const QString &message, const QSqlError &error);
};

#endif // DATABASEMANAGER_H