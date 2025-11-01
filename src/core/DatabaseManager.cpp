#include "DatabaseManager.h"
#include <QSqlError>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

bool DatabaseManager::openDatabase(const QString &dbPath)
{
    databasePath = dbPath;
    db.setDatabaseName(databasePath);

    if (!db.open()) {
        logError("Error opening database", db.lastError());
        return false;
    }
    return createHistoryTable();
}

void DatabaseManager::closeDatabase()
{
    if (db.isOpen()) {
        db.close();
    }
}

bool DatabaseManager::createHistoryTable()
{
    QSqlQuery query(db);
    QString createTableSql = "CREATE TABLE IF NOT EXISTS history ("
                             "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                             "timestamp TEXT NOT NULL,"
                             "expression TEXT NOT NULL,"
                             "result TEXT NOT NULL"
                             ");";

    if (!query.exec(createTableSql)) {
        logError("Error creating history table", query.lastError());
        return false;
    }
    return true;
}

bool DatabaseManager::addHistoryEntry(const QString &expression, const QString &result)
{
    if (!db.isOpen()) {
        // Error handled by ErrorHandler if db fails to open initially
        return false;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO history (timestamp, expression, result) VALUES (:timestamp, :expression, :result)");
    query.bindValue(":timestamp", QDateTime::currentDateTime().toString(Qt::ISODate));
    query.bindValue(":expression", expression);
    query.bindValue(":result", result);

    if (!query.exec()) {
        logError("Error adding history entry", query.lastError());
        return false;
    }
    return true;
}

QList<QPair<QString, QString>> DatabaseManager::getHistory()
{
    QList<QPair<QString, QString>> history;
    if (!db.isOpen()) {
        // Error handled by ErrorHandler if db fails to open initially
        return history;
    }

    QSqlQuery query("SELECT expression, result FROM history ORDER BY timestamp DESC", db);
    if (!query.exec()) {
        logError("Error retrieving history", query.lastError());
        return history;
    }

    while (query.next()) {
        history.append(qMakePair(query.value(0).toString(), query.value(1).toString()));
    }
    return history;
}

bool DatabaseManager::clearHistory()
{
    if (!db.isOpen()) {
        // Error handled by ErrorHandler if db fails to open initially
        return false;
    }

    QSqlQuery query(db);
    if (!query.exec("DELETE FROM history")) {
        logError("Error clearing history", query.lastError());
        return false;
    }
    return true;
}

void DatabaseManager::logError(const QString &message, const QSqlError &error)
{
    // This function is intended for internal logging/error reporting, not direct console output
    // For now, we'll let the ErrorHandler handle user-facing messages.
    // In a real app, this might write to a log file.
}
