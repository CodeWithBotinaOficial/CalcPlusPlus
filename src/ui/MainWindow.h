#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QDockWidget>

#include "../core/CalculatorCore.h"
#include "../core/DatabaseManager.h"
#include "HistoryPanel.h" // Changed from HistoryWindow.h
#include "../utils/ErrorHandler.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void digitClicked();
    void operatorClicked();
    void equalsClicked();
    void clearClicked();
    void backspaceClicked();
    void decimalClicked();
    void unaryOperatorClicked(); // For sqrt, percentage
    void toggleHistoryPanel(); // New slot to toggle history panel visibility
    void handleHistoryItemSelected(const QString &expression, const QString &result); // New slot for history item click
    void handleCalculationError(const QString &errorMessage);
    void handleClearHistoryRequested(); // New slot for HistoryPanel clear request

private:
    QLabel *expressionLabel;
    QLabel *resultLabel;
    QPushButton *createButton(const QString &text, void (MainWindow::*member)());
    void setupUi();
    void setupConnections();
    void performCalculation();
    void resetDisplayStyles();
    void applyResultStyles();

    CalculatorCore *calculatorCore;
    DatabaseManager *dbManager;
    HistoryPanel *historyPanel; // Changed from HistoryWindow
    QDockWidget *historyDock; // Dock widget for the history panel
    ErrorHandler *errorHandler;

    QString currentInput; // Stores the number currently being typed or the last result
    QString fullExpression; // Stores the entire expression being built
    QString lastResult; // Stores the result of the last calculation
    bool justCalculated; // True if the last action was an equals or unary operation
    bool waitingForOperand; // True if an operator was just pressed, waiting for the next operand

    enum Operator { None, Add, Subtract, Multiply, Divide, Power, Percentage, SquareRoot };
    Operator lastOperator;

    double operand1;
    double operand2;
};

#endif // MAINWINDOW_H