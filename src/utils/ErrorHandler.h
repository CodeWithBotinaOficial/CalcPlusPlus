#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <QObject>
#include <QString>
#include "CustomAlert.h" // Use CustomAlert instead of QMessageBox

class ErrorHandler : public QObject
{
    Q_OBJECT

public:
    explicit ErrorHandler(QObject *parent = nullptr);

    void handleError(const QString &errorMessage, const QString &details = "");

signals:
    void errorOccurred(const QString &errorMessage);
};

#endif // ERRORHANDLER_H