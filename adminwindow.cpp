#include "adminwindow.h"

AdminWindow::AdminWindow(QWidget *parent) :
    QWidget(parent)
{
    QTabWidget *mainTabs = new QTabWidget();

    SellersAdminTab *sellersTab = new SellersAdminTab();
    ProvidersAdminTab *providersTab = new ProvidersAdminTab();

    mainTabs->addTab(sellersTab, tr("Sellers"));
    mainTabs->addTab(providersTab, tr("Groups"));

    mainTabs->setBaseSize(sellersTab->size());

    QVBoxLayout *vb = new QVBoxLayout();
    vb->addWidget(mainTabs);
    this->setLayout(vb);

    this->setWindowFlags(Qt::Dialog);

    connect(sellersTab, SIGNAL(closed()), this, SLOT(close()));
    connect(providersTab, SIGNAL(closed()), this, SLOT(close()));
}

void AdminWindow::closeEvent(QCloseEvent *)
{
    emit closed();
}
