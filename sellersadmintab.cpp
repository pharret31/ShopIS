#include "sellersadmintab.h"

SellersAdminTab::SellersAdminTab(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *loginLayout = new QHBoxLayout();

    QLabel *loginLabel = new QLabel(tr("Input login here: "));
    m_loginLineEdit = new QLineEdit();

    loginLayout->addWidget(loginLabel);
    loginLayout->addStretch();
    loginLayout->addWidget(m_loginLineEdit);

    QHBoxLayout *passwordLayout = new QHBoxLayout();

    QLabel *passwordLabel = new QLabel(tr("Input password here: "));
    m_passwordLineEdit = new QLineEdit();

    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addStretch();
    passwordLayout->addWidget(m_passwordLineEdit);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();

    QPushButton *addButton = new QPushButton("Add");
    QPushButton *removeButton = new QPushButton("Remove");

    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(removeButton);

    m_sellers = new QTableWidget();

    m_sellers->installEventFilter(this);
    m_sellers->setColumnCount(2);
    m_sellers->setSelectionMode(QAbstractItemView::SingleSelection);

    m_sellers->setHorizontalHeaderLabels(QString("Login;Password").split(";"));

    updateSellersList();

    QPushButton *okButton = new QPushButton("OK");

    QVBoxLayout *mainLayout = new QVBoxLayout();

    mainLayout->addLayout(loginLayout);
    mainLayout->addLayout(passwordLayout);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(m_sellers);
    mainLayout->addWidget(okButton);

    this->setLayout(mainLayout);
    this->setWindowFlags(Qt::Dialog);

    connect(addButton, SIGNAL(clicked()), this, SLOT(onAddButtonClick()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(onRemoveButtonClick()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(close()));
}

int SellersAdminTab::updateSellersList()
{
    QFile *loginFile = new QFile("logoss.dat");

    if (!loginFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox *errorMB = new QMessageBox();
        errorMB->critical(0, tr("Error!"), tr("Login's file was not opened!"));

        return -1;
    }

    m_sellers->clearContents();
    m_sellers->setRowCount(0);

    while (!loginFile->atEnd())
    {
        QString str = loginFile->readLine();
        QStringList pair = str.split(" ");
        QString userMode = pair[2];

        if (userMode == "seller")
        {
            QTableWidgetItem *login = new QTableWidgetItem();
            login->setText(pair[0]);
            QTableWidgetItem *password = new QTableWidgetItem();
            password->setText(pair[1]);

            m_sellers->setRowCount(m_sellers->rowCount() + 1);
            m_sellers->setItem(m_sellers->rowCount() - 1, 0, login);
            m_sellers->setItem(m_sellers->rowCount() - 1, 1, password);
        }
    }

    loginFile->close();

    return 0;
}

void SellersAdminTab::stretchColumns()
{
    m_sellers->setColumnWidth(0, m_sellers->width() / 2 - 10);
    m_sellers->setColumnWidth(1, m_sellers->width() / 2 - 10);
}

bool SellersAdminTab::eventFilter(QObject *obj, QEvent *evt)
{
    if (evt->type() == QEvent::Resize)
    {
        stretchColumns();
    }

    return QWidget::eventFilter(obj, evt);
}

void SellersAdminTab::closeEvent(QCloseEvent *)
{
    emit closed();
}

int SellersAdminTab::onAddButtonClick()
{
    if (m_loginLineEdit->text().isEmpty() || m_passwordLineEdit->text().isEmpty())
    {
        QMessageBox *errorMB = new QMessageBox();
        errorMB->critical(0, tr("Error!"), tr("Login and/or password is empty!"));

        return -2;
    }

    QFile *loginFile = new QFile("logoss.dat");

    if (!loginFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox *errorMB = new QMessageBox();
        errorMB->critical(0, tr("Error!"), tr("File was not opened!"));

        return -1;
    }

    while (!loginFile->atEnd())
    {
        QString str = loginFile->readLine();
        QStringList pair = str.split(" ");
        QString login = pair[0];

        if (m_loginLineEdit->text() == login)
        {
            QMessageBox *errorMB = new QMessageBox();
            errorMB->critical(0, tr("Error!"), tr("This login already used!"));

            return -3;
        }
    }

    loginFile->close();

    loginFile->setFileName("logoss.dat");

    if (!loginFile->open(QIODevice::Append | QIODevice::Text))
    {
        QMessageBox *errorMB = new QMessageBox();
        errorMB->critical(0, tr("Error!"), tr("File was not opened!"));

        return -1;
    }

    QString str = m_loginLineEdit->text() + " " + m_passwordLineEdit->text() + " seller \n";
    loginFile->write(str.toUtf8());

    loginFile->close();

    m_loginLineEdit->clear();
    m_passwordLineEdit->clear();

    updateSellersList();

    return 0;
}

int SellersAdminTab::onRemoveButtonClick()
{
    if (m_sellers->rowCount() == 0 || !m_sellers->currentItem()->isSelected())
    {
        QMessageBox *errorMB = new QMessageBox();
        errorMB->critical(0, tr("Error!"), tr("Nothing is selected!"));

        return -1;
    }

    QFile *loginFile = new QFile("logoss.dat");

    if (!loginFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox *errorMB = new QMessageBox();
        errorMB->critical(0, tr("Error!"), tr("File was not opened!"));

        return -1;
    }

    QVector<QString> lines;

    while (!loginFile->atEnd())
    {
        lines.push_back(loginFile->readLine());
    }

    loginFile->remove();

    if (!loginFile->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox *errorMB = new QMessageBox();
        errorMB->critical(0, tr("Error!"), tr("File was not created!"));

        return -1;
    }

    for (int i = 0; i < lines.size(); i++)
    {
        QStringList pair = lines.at(i).split(" ");
        QString name = pair[0];

        QString selectedName = m_sellers->item(m_sellers->currentItem()->row(), 0)->text();

        if (name == selectedName)
        {
            lines.remove(i);

            break;
        }
    }

    for (int i = 0; i < lines.size(); i++)
    {
        loginFile->write(lines.at(i).toUtf8());
    }

    loginFile->close();

    updateSellersList();

    return 0;
}
