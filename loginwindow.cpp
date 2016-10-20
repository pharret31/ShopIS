#include "loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent)
{
    m_loginFile = new QFile("logoss.dat");

    QVBoxLayout *mainLayout = new QVBoxLayout();

    QLabel *loginLabel = new QLabel(tr("Input your login here: "));
    m_loginLineEdit = new QLineEdit();

    QLabel *passwordLabel = new QLabel(tr("Input your password here: "));
    m_passwordLineEdit = new QLineEdit();
    m_passwordLineEdit->setEchoMode(QLineEdit::Password);

    this->installEventFilter(this);

    QPushButton *okButton = new QPushButton("OK");
    QPushButton *exitButton = new QPushButton("Exit");

    QPushButton *guestLogInButton = new QPushButton("Log in as guest");

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(exitButton);

    mainLayout->addWidget(loginLabel);
    mainLayout->addWidget(m_loginLineEdit);

    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(m_passwordLineEdit);

    mainLayout->addLayout(buttonsLayout);

    mainLayout->addWidget(guestLogInButton);

    this->setLayout(mainLayout);
    this->setMinimumSize(300, 180);
    this->setWindowFlags(Qt::Dialog);

    m_adminWindow = new AdminWindow();
    m_sellerWindow = new SellerWindow();
    m_guestWindow = new GuestWindow();

    connect(okButton, SIGNAL(clicked()), this, SLOT(onOKButtonClicked()));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_adminWindow, SIGNAL(closed()), this, SLOT(close()));
    connect(m_sellerWindow, SIGNAL(closed()), this, SLOT(close()));
    connect(m_guestWindow, SIGNAL(closed()), this, SLOT(close()));
    connect(guestLogInButton, SIGNAL(clicked()), this, SLOT(guestWindowShow()));
}

bool LoginWindow::logIn(QString name, QString password)
{
    if (!m_loginFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_loginFile->open(QIODevice::WriteOnly | QIODevice::Text);
        m_loginFile->write("admin admin admin \n");
        m_loginFile->close();
        m_loginFile->open(QIODevice::ReadOnly | QIODevice::Text);
    }

    m_loginLineEdit->clear();
    m_passwordLineEdit->clear();

    while (!m_loginFile->atEnd())
    {
        QString str = m_loginFile->readLine();
        QStringList pair = str.split(" ");
        if (pair[0] == name && pair[1] == password)
        {
            m_userMode = pair[2];

            this->hide();
            m_loginFile->close();

            // Запуск программы с соответствующими привелегиями

            if (m_userMode == "admin")
            {
                m_adminWindow->show();
            }
            else if (m_userMode == "seller")
            {
                m_sellerWindow->show();
            }

            return true;
        }
    }

    QMessageBox *errorMB = new QMessageBox();
    errorMB->critical(0, tr("Error!"), tr("Login and/or password incorrect!"));

    m_loginLineEdit->setFocus();

    m_loginFile->close();
    return false;
}

bool LoginWindow::eventFilter(QObject *obj, QEvent *evt)
{
    if (evt->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(evt);

        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
        {
            onOKButtonClicked();
        }
        else if (keyEvent->key() == Qt::Key_Escape)
        {
            this->close();
        }
    }

    return QWidget::eventFilter(obj, evt);
}

void LoginWindow::onOKButtonClicked()
{
    logIn(m_loginLineEdit->text(), m_passwordLineEdit->text());
}

void LoginWindow::guestWindowShow()
{
    this->hide();
    m_guestWindow->show();
}
