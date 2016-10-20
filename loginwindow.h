#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QtWidgets/QWidget>
#include <QFile>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>

#include "adminwindow.h"
#include "sellerwindow.h"
#include "guestwindow.h"

class LoginWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = 0);
    
private:
    QFile *m_loginFile;

    QLineEdit *m_loginLineEdit;
    QLineEdit *m_passwordLineEdit;

    QString m_userMode;

    AdminWindow *m_adminWindow;
    SellerWindow *m_sellerWindow;
    GuestWindow *m_guestWindow;

    bool logIn(QString name, QString password);

    bool eventFilter(QObject *obj, QEvent *evt);
signals:
    
public slots:
    void onOKButtonClicked();

    void guestWindowShow();
    
};

#endif // LOGINWINDOW_H
