#include "CustomAlert.h"
#include <QGraphicsDropShadowEffect>
#include <QScreen>
#include <QGuiApplication>
#include <QTimer>

CustomAlert::CustomAlert(IconType type, const QString &title, const QString &message, QWidget *parent)
    : QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setModal(true);

    setupUi(type, title, message);
    applyStyles();

    animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);

    connect(okButton, &QPushButton::clicked, this, &CustomAlert::on_okButton_clicked);
}

CustomAlert::~CustomAlert()
{
}

void CustomAlert::setupUi(IconType type, const QString &title, const QString &message)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Title and Icon Layout
    QHBoxLayout *titleIconLayout = new QHBoxLayout();
    titleIconLayout->setSpacing(10);

    iconLabel = new QLabel(this);
    // Set icon based on type
    QString iconPath;
    QString iconColor;
    switch (type) {
        case Info:    iconPath = ":/icons/info.png";    iconColor = "#2196F3"; break; // Blue
        case Warning: iconPath = ":/icons/warning.png"; iconColor = "#FFC107"; break; // Amber
        case Error:   iconPath = ":/icons/error.png";   iconColor = "#F44336"; break; // Red
    }
    // For now, we'll use text icons or simple placeholders as we don't have actual icon files
    // In a real app, you'd load QPixmaps here.
    iconLabel->setText("!"); // Placeholder
    iconLabel->setStyleSheet(QString("font-size: 24px; font-weight: bold; color: %1;").arg(iconColor));
    iconLabel->setFixedSize(30, 30);
    iconLabel->setAlignment(Qt::AlignCenter);
    titleIconLayout->addWidget(iconLabel);

    titleLabel = new QLabel(title, this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #EEEEEE;");
    titleIconLayout->addWidget(titleLabel);
    titleIconLayout->addStretch();

    mainLayout->addLayout(titleIconLayout);

    // Message Label
    messageLabel = new QLabel(message, this);
    messageLabel->setWordWrap(true);
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setStyleSheet("font-size: 16px; color: #BBBBBB;");
    mainLayout->addWidget(messageLabel);

    // OK Button
    okButton = new QPushButton("OK", this);
    okButton->setFixedSize(100, 35);
    okButton->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; border: none; border-radius: 8px; font-size: 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #1976D2; }"
        "QPushButton:pressed { background-color: #1565C0; }"
    );
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void CustomAlert::applyStyles()
{
    setStyleSheet(
        "QDialog { background-color: #2b2b2b; border-radius: 15px; }"
    );

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 150));
    shadow->setOffset(0, 5);
    setGraphicsEffect(shadow);
}

void CustomAlert::on_okButton_clicked()
{
    animation->setDirection(QAbstractAnimation::Backward);
    connect(animation, &QPropertyAnimation::finished, this, &CustomAlert::close);
    animation->start();
}

void CustomAlert::showEvent(QShowEvent *event)
{
    // Center the dialog on the screen
    if (parentWidget())
    {
        QPoint globalPos = parentWidget()->mapToGlobal(QPoint(0,0));
        int x = globalPos.x() + (parentWidget()->width() - width()) / 2;
        int y = globalPos.y() + (parentWidget()->height() - height()) / 2;
        move(x, y);
    }
    else
    {
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        int x = (screenGeometry.width() - width()) / 2;
        int y = (screenGeometry.height() - height()) / 2;
        move(x, y);
    }

    animation->setDirection(QAbstractAnimation::Forward);
    animation->start();
    QDialog::showEvent(event);
}

void CustomAlert::hideEvent(QHideEvent *event)
{
    QDialog::hideEvent(event);
}
