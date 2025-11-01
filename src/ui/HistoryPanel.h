#ifndef HISTORYPANEL_H
#define HISTORYPANEL_H

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class HistoryPanel : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryPanel(QWidget *parent = nullptr);
    ~HistoryPanel();

    void addHistoryEntry(const QString &expression, const QString &result);
    void clearHistoryList();

signals:
    void historyItemSelected(const QString &expression, const QString &result);
    void clearHistoryRequested();

private slots:
    void on_historyListWidget_itemClicked(QListWidgetItem *item);

private:
    QListWidget *historyListWidget;
    QPushButton *clearButton; // Re-added the clear button

    void setupUi();
    void setupConnections();
    void applyStyles();
};

#endif // HISTORYPANEL_H