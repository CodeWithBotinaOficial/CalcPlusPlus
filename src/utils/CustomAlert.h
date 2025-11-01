#ifndef CUSTOMALERT_H
#define CUSTOMALERT_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QShowEvent>
#include <QHideEvent>

class CustomAlert : public QDialog
{
    Q_OBJECT

public:
    enum IconType {
        Info,
        Warning,
        Error
    };

    explicit CustomAlert(IconType type, const QString &title, const QString &message, QWidget *parent = nullptr);
    ~CustomAlert();

private slots:
    void on_okButton_clicked();

private:
    QLabel *iconLabel;
    QLabel *titleLabel;
    QLabel *messageLabel;
    QPushButton *okButton;
    QPropertyAnimation *animation;

    void setupUi(IconType type, const QString &title, const QString &message);
    void applyStyles();
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
};

#endif // CUSTOMALERT_H