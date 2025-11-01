#include "HistoryItemWidget.h"

HistoryItemWidget::HistoryItemWidget(const QString &expression, const QString &result, QWidget *parent)
    : QWidget(parent),
      m_expression(expression),
      m_result(result)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(2);

    QLabel *expressionLabel = new QLabel(expression, this);
    expressionLabel->setStyleSheet("font-size: 14px; color: #BBBBBB;");
    expressionLabel->setAlignment(Qt::AlignRight);

    QLabel *resultLabel = new QLabel(result, this);
    resultLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #EEEEEE;");
    resultLabel->setAlignment(Qt::AlignRight);

    layout->addWidget(expressionLabel);
    layout->addWidget(resultLabel);
}
