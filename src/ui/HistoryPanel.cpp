#include "HistoryPanel.h"
#include "HistoryItemWidget.h" // Include the new header for HistoryItemWidget
#include <QListWidgetItem>

HistoryPanel::HistoryPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    setupConnections();
    applyStyles();
}

HistoryPanel::~HistoryPanel()
{
}

void HistoryPanel::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // List Widget for history items
    historyListWidget = new QListWidget(this);
    historyListWidget->setAlternatingRowColors(false);
    historyListWidget->setFrameShape(QFrame::NoFrame);
    historyListWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    mainLayout->addWidget(historyListWidget);

    // Clear History Button
    clearButton = new QPushButton("Clear History", this);
    clearButton->setStyleSheet(
        "QPushButton { background-color: #f44336; color: white; border: none; padding: 10px; font-size: 16px; border-radius: 0px; }"
        "QPushButton:hover { background-color: #da190b; }"
        "QPushButton:pressed { background-color: #b71c1c; }"
    );
    mainLayout->addWidget(clearButton);
}

void HistoryPanel::setupConnections()
{
    connect(historyListWidget, &QListWidget::itemClicked, this, &HistoryPanel::on_historyListWidget_itemClicked);
    connect(clearButton, &QPushButton::clicked, this, &HistoryPanel::clearHistoryRequested);
}

void HistoryPanel::applyStyles()
{
    setStyleSheet(
        "HistoryPanel { background-color: #222222; border-left: 1px solid #444444; }"
        "QListWidget { background-color: #222222; border: none; padding: 8px; }"
        "QListWidget::item { margin-bottom: 5px; }"
        "QListWidget::item:hover { background-color: #444444; border-radius: 8px; }"
        "QListWidget::item:selected { background-color: #555555; border-radius: 8px; }"
        "QListWidget::item QLabel { background-color: transparent; }"
    );
}

void HistoryPanel::addHistoryEntry(const QString &expression, const QString &result)
{
    QListWidgetItem *listItem = new QListWidgetItem(historyListWidget);
    HistoryItemWidget *itemWidget = new HistoryItemWidget(expression, result, historyListWidget);

    listItem->setSizeHint(itemWidget->sizeHint()); // Set size hint based on widget
    historyListWidget->addItem(listItem);
    historyListWidget->setItemWidget(listItem, itemWidget);
    historyListWidget->scrollToBottom();
}

void HistoryPanel::clearHistoryList()
{
    historyListWidget->clear();
}

void HistoryPanel::on_historyListWidget_itemClicked(QListWidgetItem *item)
{
    HistoryItemWidget *itemWidget = qobject_cast<HistoryItemWidget*>(historyListWidget->itemWidget(item));
    if (itemWidget) {
        // Use the public getters to retrieve data
        QString expression = itemWidget->getExpression();
        QString result = itemWidget->getResult();
        emit historyItemSelected(expression, result);
    }
}
