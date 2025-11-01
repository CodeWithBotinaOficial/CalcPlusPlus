#include "MainWindow.h"
#include <cmath>
#include <QMessageBox>
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      errorHandler(new ErrorHandler(this)), // Initialize errorHandler first
      calculatorCore(new CalculatorCore(errorHandler)), // Pass errorHandler to CalculatorCore
      dbManager(new DatabaseManager(this)),
      historyPanel(new HistoryPanel(this)), // Parent historyPanel to MainWindow
      historyDock(new QDockWidget("History", this)), // Parent historyDock to MainWindow
      currentInput("0"), // Initialize currentInput to "0"
      fullExpression(""),
      lastResult(""),
      justCalculated(true), // Start with a fresh calculation state
      waitingForOperand(false),
      lastOperator(None),
      operand1(0.0),
      operand2(0.0)
{
    setWindowTitle("Calc++");
    setFixedSize(350, 500); // Set a fixed size for now, can be made responsive later

    // Connect error handler signal
    connect(errorHandler, &ErrorHandler::errorOccurred, this, &MainWindow::handleCalculationError);

    // Open database
    if (!dbManager->openDatabase("calc_history.db")) {
        errorHandler->handleError("Failed to open database!", "History will not be saved.");
    }

    setupUi();
    setupConnections();
    resetDisplayStyles(); // Apply initial styles

    // Load history from DB on startup
    QList<QPair<QString, QString>> history = dbManager->getHistory();
    for (const auto &entry : history) {
        historyPanel->addHistoryEntry(entry.first, entry.second);
    }
}

MainWindow::~MainWindow()
{
    delete calculatorCore; // Manually delete as it's not parented to QObject
    // historyPanel and historyDock are parented to MainWindow, so they will be deleted automatically.
}

void MainWindow::setupUi()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(5);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // Expression Label (smaller, for full operation)
    expressionLabel = new QLabel("", this);
    expressionLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    expressionLabel->setTextFormat(Qt::PlainText);
    expressionLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mainLayout->addWidget(expressionLabel);

    // Result Label (larger, for current input or final result)
    resultLabel = new QLabel("0", this);
    resultLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    resultLabel->setTextFormat(Qt::PlainText);
    resultLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    mainLayout->addWidget(resultLabel);

    QGridLayout *buttonLayout = new QGridLayout();
    buttonLayout->setSpacing(5);
    mainLayout->addLayout(buttonLayout);

    // Row 0: Clear, Backspace, %, sqrt
    buttonLayout->addWidget(createButton("C", &MainWindow::clearClicked), 0, 0);
    buttonLayout->addWidget(createButton("←", &MainWindow::backspaceClicked), 0, 1);
    buttonLayout->addWidget(createButton("%", &MainWindow::unaryOperatorClicked), 0, 2);
    buttonLayout->addWidget(createButton("√", &MainWindow::unaryOperatorClicked), 0, 3);

    // Row 1-3: Digits and basic operators
    QStringList digitButtons = {"7", "8", "9", "4", "5", "6", "1", "2", "3"};
    int digitPos = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            QPushButton *button = createButton(digitButtons[digitPos], &MainWindow::digitClicked);
            buttonLayout->addWidget(button, i + 1, j);
            digitPos++;
        }
    }

    buttonLayout->addWidget(createButton("÷", &MainWindow::operatorClicked), 1, 3);
    buttonLayout->addWidget(createButton("×", &MainWindow::operatorClicked), 2, 3);
    buttonLayout->addWidget(createButton("-", &MainWindow::operatorClicked), 3, 3);

    // Row 4: 0, ., =, +
    buttonLayout->addWidget(createButton("0", &MainWindow::digitClicked), 4, 0, 1, 2); // 0 spans two columns
    buttonLayout->addWidget(createButton(".", &MainWindow::decimalClicked), 4, 2);
    buttonLayout->addWidget(createButton("+", &MainWindow::operatorClicked), 4, 3);

    // Row 5: Power, History, Equals
    buttonLayout->addWidget(createButton("x^y", &MainWindow::operatorClicked), 5, 0);
    // Connect the History button to the new toggle slot
    buttonLayout->addWidget(createButton("History", &MainWindow::toggleHistoryPanel), 5, 1, 1, 2); // History spans two columns
    buttonLayout->addWidget(createButton("=", &MainWindow::equalsClicked), 5, 3);

    // Apply styles to buttons
    for (int i = 0; i < buttonLayout->count(); ++i) {
        QWidget *widget = buttonLayout->itemAt(i)->widget();
        if (QPushButton *button = qobject_cast<QPushButton *>(widget)) {
            button->setStyleSheet(
                "QPushButton { background-color: #e0e0e0; border: 1px solid #cccccc; padding: 15px; font-size: 20px; }"
                "QPushButton:hover { background-color: #d0d0d0; }"
                "QPushButton:pressed { background-color: #c0c0c0; }"
            );
            if (button->text() == "=" || button->text() == "+" || button->text() == "-" ||
                button->text() == "×" || button->text() == "÷" || button->text() == "x^y") {
                button->setStyleSheet(
                    "QPushButton { background-color: #4CAF50; color: white; border: 1px solid #4CAF50; padding: 15px; font-size: 20px; }"
                    "QPushButton:hover { background-color: #45a049; }"
                    "QPushButton:pressed { background-color: #3e8e41; }"
                );
            } else if (button->text() == "C" || button->text() == "←") {
                button->setStyleSheet(
                    "QPushButton { background-color: #f44336; color: white; border: 1px solid #f44336; padding: 15px; font-size: 20px; }"
                    "QPushButton:hover { background-color: #da190b; }"
                    "QPushButton:pressed { background-color: #b71c1c; }"
                );
            } else if (button->text() == "History") {
                button->setStyleSheet(
                    "QPushButton { background-color: #2196F3; color: white; border: 1px solid #2196F3; padding: 15px; font-size: 20px; }"
                    "QPushButton:hover { background-color: #1976D2; }"
                    "QPushButton:pressed { background-color: #1565C0; }"
                );
            }
        }
    }

    // Setup QDockWidget for HistoryPanel
    historyDock->setWidget(historyPanel);
    historyDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    historyDock->setAllowedAreas(Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, historyDock);
    historyDock->hide(); // Start hidden

    // Apply main window style for dark theme consistency
    setStyleSheet("QMainWindow { background-color: #2E2E2E; }");
}

QPushButton *MainWindow::createButton(const QString &text, void (MainWindow::*member)())
{
    QPushButton *button = new QPushButton(text);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QFont font = button->font();
    font.setPointSize(18);
    button->setFont(font);
    connect(button, &QPushButton::clicked, this, member);
    return button;
}

void MainWindow::setupConnections()
{
    // Connect signals from HistoryPanel
    connect(historyPanel, &HistoryPanel::historyItemSelected, this, &MainWindow::handleHistoryItemSelected);
    connect(historyPanel, &HistoryPanel::clearHistoryRequested, this, &MainWindow::handleClearHistoryRequested);
}

void MainWindow::resetDisplayStyles()
{
    QFont expressionFont = expressionLabel->font();
    expressionFont.setPointSize(16);
    expressionFont.setBold(false);
    expressionLabel->setFont(expressionFont);
    expressionLabel->setStyleSheet("QLabel { color: #BBBBBB; }"); // Default gray for expression

    QFont resultFont = resultLabel->font();
    resultFont.setPointSize(36);
    resultFont.setBold(true);
    resultLabel->setFont(resultFont);
    resultLabel->setStyleSheet("QLabel { color: #EEEEEE; }"); // Default light color for result
}

void MainWindow::applyResultStyles()
{
    QFont expressionFont = expressionLabel->font();
    expressionFont.setPointSize(16);
    expressionFont.setBold(false);
    expressionLabel->setFont(expressionFont);
    expressionLabel->setStyleSheet("QLabel { color: rgba(187, 187, 187, 0.6); }"); // Semi-transparent gray

    QFont resultFont = resultLabel->font();
    resultFont.setPointSize(48); // Slightly larger for final result
    resultFont.setBold(true);
    resultLabel->setFont(resultFont);
    resultLabel->setStyleSheet("QLabel { color: #EEEEEE; }"); // Bold light color for result
}

void MainWindow::digitClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    if (!clickedButton) return;

    QString digit = clickedButton->text();

    if (justCalculated || currentInput == "0" || resultLabel->text() == "Error") {
        currentInput = digit;
        justCalculated = false;
    } else {
        currentInput += digit;
    }
    resultLabel->setText(currentInput);
    expressionLabel->setText(fullExpression + currentInput);
    resetDisplayStyles();
}

void MainWindow::decimalClicked()
{
    if (justCalculated || currentInput == "0" || resultLabel->text() == "Error") {
        currentInput = "0.";
        justCalculated = false;
    } else if (!currentInput.contains('.')) {
        currentInput += ".";
    }
    resultLabel->setText(currentInput);
    expressionLabel->setText(fullExpression + currentInput);
    resetDisplayStyles();
}

void MainWindow::operatorClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    if (!clickedButton) return;

    if (justCalculated) {
        // If just calculated, the result becomes the first operand for the next operation
        fullExpression = lastResult;
        currentInput = lastResult;
    } else if (!waitingForOperand) {
        // If an operator was just pressed, perform the previous calculation first
        operand2 = currentInput.toDouble();
        fullExpression += currentInput;
        performCalculation(); // This updates lastResult and operand1
        fullExpression = lastResult; // Start new expression with the result
        currentInput = lastResult; // Update currentInput with the result
    }

    operand1 = currentInput.toDouble();
    waitingForOperand = true;
    justCalculated = false;

    QString opText = clickedButton->text();

    // Ensure the division operator is consistently U+00F7 and multiplication is U+00D7
    if (opText == "÷") {
        opText = QStringLiteral("\u00F7");
    } else if (opText == "×") {
        opText = QStringLiteral("\u00D7");
    }

    if (opText == "+") lastOperator = Add;
    else if (opText == "-") lastOperator = Subtract;
    else if (opText == QStringLiteral("\u00D7")) lastOperator = Multiply;
    else if (opText == QStringLiteral("\u00F7")) lastOperator = Divide;
    else if (opText == "x^y") lastOperator = Power;
    else lastOperator = None; // Should not happen for these buttons

    fullExpression += " " + opText + " ";
    expressionLabel->setText(fullExpression);
    currentInput = "0"; // Clear current input for the next operand
    resultLabel->setText(currentInput);
    resetDisplayStyles();
}

void MainWindow::unaryOperatorClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    if (!clickedButton) return;

    double value = currentInput.toDouble();
    QString opText = clickedButton->text();
    double result = value;
    QString expressionToSave = currentInput;
    bool error = false;

    if (opText == "%") {
        result = value / 100.0;
        expressionToSave += "%";
    } else if (opText == "√") {
        if (value < 0) {
            errorHandler->handleError("Cannot calculate square root of a negative number.");
            error = true;
        } else {
            result = std::sqrt(value);
            expressionToSave = "√(" + expressionToSave + ")";
        }
    }

    if (!error) {
        lastResult = QString::number(result);
        resultLabel->setText(lastResult);
        expressionLabel->setText(expressionToSave + " =");
        applyResultStyles();

        dbManager->addHistoryEntry(expressionToSave, lastResult);
        historyPanel->addHistoryEntry(expressionToSave, lastResult); // Add to history panel

        justCalculated = true;
        waitingForOperand = false;
        lastOperator = None;
        fullExpression = lastResult; // For chaining operations after unary
        currentInput = lastResult; // Update currentInput with the result
        operand1 = result;
        operand2 = 0.0;
    } else {
        handleCalculationError("Error in unary operation."); // Error handler will reset state
    }
}

void MainWindow::equalsClicked()
{
    if (justCalculated) return; // No new operation to perform

    operand2 = currentInput.toDouble();
    fullExpression += currentInput;

    performCalculation();
    dbManager->addHistoryEntry(fullExpression, lastResult);
    historyPanel->addHistoryEntry(fullExpression, lastResult); // Add to history panel

    expressionLabel->setText(fullExpression + " =");
    resultLabel->setText(lastResult);

    applyResultStyles();

    justCalculated = true;
    waitingForOperand = false;
    lastOperator = None;
    fullExpression = lastResult; // For chaining operations after equals
    currentInput = lastResult; // Update currentInput with the result
    operand1 = lastResult.toDouble();
    operand2 = 0.0;
}

void MainWindow::performCalculation()
{
    bool calculationOk = false;
    double result = calculatorCore->calculate(fullExpression, &calculationOk);

    if (calculationOk) {
        lastResult = QString::number(result);
        operand1 = result;
    } else {
        lastResult = "Error";
        operand1 = 0.0;
        // ErrorHandler already shows QMessageBox, just update internal state
    }
    waitingForOperand = true; // Ready for next operand or new operation
}

void MainWindow::clearClicked()
{
    currentInput = "0";
    fullExpression = "";
    lastResult = "";
    justCalculated = true;
    waitingForOperand = false;
    lastOperator = None;
    operand1 = 0.0;
    operand2 = 0.0;
    expressionLabel->setText("");
    resultLabel->setText("0");
    resetDisplayStyles();
}

void MainWindow::backspaceClicked()
{
    if (justCalculated) return; // Cannot backspace on a result

    if (currentInput.length() > 1 && currentInput != "Error") {
        currentInput.chop(1);
    } else {
        currentInput = "0";
    }
    resultLabel->setText(currentInput);
    // When backspacing, we need to reconstruct the expressionLabel correctly
    // This is a simplified approach; a more robust solution might involve parsing fullExpression
    // For now, if fullExpression is empty, just show currentInput. Otherwise, append currentInput.
    if (fullExpression.isEmpty()) {
        expressionLabel->setText("");
    } else {
        // Attempt to remove the last operand from fullExpression if it was just added
        // This is a complex task without a proper expression parser. For simplicity,
        // we'll just update the resultLabel and clear expressionLabel if it's a new input.
        // A better approach would be to manage fullExpression as a list of tokens.
        expressionLabel->setText(fullExpression + currentInput);
    }
    resetDisplayStyles();
}

void MainWindow::toggleHistoryPanel()
{
    historyDock->setVisible(!historyDock->isVisible());
}

void MainWindow::handleHistoryItemSelected(const QString &expression, const QString &result)
{
    // Update main calculator display with selected history item
    fullExpression = expression;
    currentInput = result;
    lastResult = result;
    operand1 = result.toDouble();
    operand2 = 0.0;
    lastOperator = None;
    justCalculated = true;
    waitingForOperand = false;

    expressionLabel->setText(fullExpression + " =");
    resultLabel->setText(lastResult);
    applyResultStyles();

    // Optionally hide the history panel after selection
    historyDock->hide();
}

void MainWindow::handleCalculationError(const QString &errorMessage)
{
    // The ErrorHandler already shows a QMessageBox. Here we just reset the calculator state.
    currentInput = "0";
    fullExpression = "";
    lastResult = "Error";
    justCalculated = true;
    waitingForOperand = false;
    lastOperator = None;
    operand1 = 0.0;
    operand2 = 0.0;

    expressionLabel->setText("");
    resultLabel->setText("Error");
    // Apply error styles
    QFont resultFont = resultLabel->font();
    resultFont.setPointSize(48);
    resultFont.setBold(true);
    resultLabel->setFont(resultFont);
    resultLabel->setStyleSheet("QLabel { color: red; }");

    // No resetDisplayStyles() here, as we want the error style to persist until cleared.
}

void MainWindow::handleClearHistoryRequested()
{
    if (dbManager->clearHistory()) {
        historyPanel->clearHistoryList(); // Clear the QListWidget in the panel
        CustomAlert *alert = new CustomAlert(CustomAlert::Info, "History", "Calculation history cleared.", this);
        alert->exec();
    } else {
        errorHandler->handleError("Failed to clear history.");
    }
}
