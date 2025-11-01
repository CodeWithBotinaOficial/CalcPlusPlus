#ifndef CALCULATORCORE_H
#define CALCULATORCORE_H

#include <QString>
#include <QStack>
#include "../utils/ErrorHandler.h"

class CalculatorCore
{
public:
    CalculatorCore(ErrorHandler *errorHandler = nullptr);

    double calculate(const QString &expression, bool *ok = nullptr);

private:
    ErrorHandler *m_errorHandler;

    bool isOperator(const QString &token) const;
    int getPrecedence(const QString &op) const;
    double applyOperator(double operand1, double operand2, const QString &op, bool *ok);
};

#endif // CALCULATORCORE_H