#ifndef HISTORYITEMWIDGET_H
#define HISTORYITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

// Custom QWidget for each history item to allow rich styling
class HistoryItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryItemWidget(const QString &expression, const QString &result, QWidget *parent = nullptr);

    // Public getters for expression and result
    QString getExpression() const { return m_expression; }
    QString getResult() const { return m_result; }

private:
    QString m_expression;
    QString m_result;
};

#endif // HISTORYITEMWIDGET_H