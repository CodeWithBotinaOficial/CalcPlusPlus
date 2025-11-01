#include "CalculatorCore.h"
#include <QtMath>
#include <QRegularExpression>
#include <cmath> // For fmod

CalculatorCore::CalculatorCore(ErrorHandler *errorHandler)
    : m_errorHandler(errorHandler)
{
}

double CalculatorCore::calculate(const QString &expression, bool *ok)
{
    if (ok) *ok = true; // Assume success initially

    // Regex to match a simple binary operation: operand1 operator operand2
    // Supports: +, -, x (multiply), ÷ (divide), * (multiply), / (divide), ^ (power), % (modulo), x^y (power)
    // Numbers can be integers or floating-point, optionally negative.
    // Using a raw string literal (R"(...)") to avoid C++ escape sequence interpretation issues.
    // IMPORTANT: Added '×' (Unicode multiplication sign) to the operator character class.
    QRegularExpression rx(R"((-?\d+(?:\.\d+)?)\s*(x\^y|\+|\-|x|÷|\*|/|\^|%|×)\s*(-?\d+(?:\.\d+)?))");

    if (!rx.isValid()) {
        if (m_errorHandler) {
            m_errorHandler->handleError("Internal error: Invalid regex pattern.", rx.errorString());
        }
        if (ok) *ok = false;
        return qQNaN();
    }

    QRegularExpressionMatch match = rx.match(expression);

    if (match.hasMatch()) {
        double operand1 = match.captured(1).toDouble();
        QString op = match.captured(2);
        double operand2 = match.captured(3).toDouble();

        double result = 0.0;

        if (op == "+") {
            result = operand1 + operand2;
        } else if (op == "-" || op == "−") { // Handle both hyphen and minus sign
            result = operand1 - operand2;
        } else if (op == "x" || op == "*" || op == "×") { // Handle 'x', '*', and '×' (Unicode multiplication sign)
            result = operand1 * operand2;
        } else if (op == "÷" || op == "/") { // Handle both '÷' and '/'
            if (operand2 == 0.0) {
                if (m_errorHandler) {
                    m_errorHandler->handleError("Division by zero is not allowed.");
                }
                if (ok) *ok = false;
                return qQNaN();
            }
            result = operand1 / operand2;
        } else if (op == "^" || op == "x^y") { // Handle both '^' and 'x^y' for power
            result = qPow(operand1, operand2);
        } else if (op == "%") { // Modulo operator
            if (operand2 == 0.0) {
                if (m_errorHandler) {
                    m_errorHandler->handleError("Modulo by zero is not allowed.");
                }
                if (ok) *ok = false;
                return qQNaN();
            }
            result = fmod(operand1, operand2); // fmod for floating point remainder
        }
        else {
            // This case should ideally not be reached if the regex is correct
            if (m_errorHandler) {
                m_errorHandler->handleError("Unknown operator encountered: " + op);
            }
            if (ok) *ok = false;
            return qQNaN();
        }
        return result;
    }

    // If it's not a binary operation, try to convert directly (e.g., single number input)
    bool conversionOk;
    double result = expression.toDouble(&conversionOk);
    if (conversionOk) {
        return result;
    }

    // If no match and not a single number, it's an invalid expression format
    if (m_errorHandler) {
        m_errorHandler->handleError("Invalid expression format: " + expression);
    }
    if (ok) *ok = false;
    return qQNaN();
}
