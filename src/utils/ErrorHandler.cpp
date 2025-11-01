#include "ErrorHandler.h"
#include <QDebug>

ErrorHandler::ErrorHandler(QObject *parent)
    : QObject(parent)
{
}

void ErrorHandler::handleError(const QString &errorMessage, const QString &details)
{
    // Use CustomAlert instead of QMessageBox
    CustomAlert *alert = new CustomAlert(CustomAlert::Error, "Error", errorMessage + (details.isEmpty() ? "" : "\n" + details), nullptr);
    alert->exec(); // Show as a modal dialog
    emit errorOccurred(errorMessage);
}
