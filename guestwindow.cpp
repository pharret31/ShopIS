#include "guestwindow.h"

GuestWindow::GuestWindow(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    m_infoTree = new QTreeWidget();

    m_infoTree->setColumnCount(2);
    m_infoTree->setHeaderLabels(QString("Name;Weight").split(";"));

    m_categories = new Categories((char*)"base.grs");
    m_categories->updateBuffer();

    QList<QTreeWidgetItem*> items;

    deque<categories>::iterator iter;
    for (iter = m_categories->buffer.begin(); iter != m_categories->buffer.end(); iter++)
    {
        categories cat = *iter;

        QTreeWidgetItem *root = new QTreeWidgetItem();
        root->setText(0, QString(cat.categoryName));
        root->setText(1, QString::number(cat.maxWeight));

        items.push_back(root);

        QList<QTreeWidgetItem*> subItems;
        for (int i = 0; i < cat.countProducts; i++)
        {
            product prod = cat.products[i];

            QTreeWidgetItem *subItem = new QTreeWidgetItem();
            subItem->setText(0, QString(prod.name));
            subItem->setText(1, QString::number(prod.weight));

            subItems.push_back(subItem);
        }
        items.at(items.count() - 1)->addChildren(subItems);
    }

    m_infoTree->insertTopLevelItems(0, items);

    QPushButton *okButton = new QPushButton("OK");

    mainLayout->addWidget(m_infoTree);
    mainLayout->addWidget(okButton);

    this->setLayout(mainLayout);
    this->setWindowFlags(Qt::Dialog);

    connect(okButton, SIGNAL(clicked()), this, SLOT(close()));
}

int GuestWindow::showCategory(QString categoryName)
{
    m_infoTree->clear();

    QList<QTreeWidgetItem*> items;

    deque<categories>::iterator iter;
    for (iter = m_categories->buffer.begin(); iter != m_categories->buffer.end(); iter++)
    {
        categories cat = *iter;

        if (cat.categoryName == categoryName)
        {
            for (int i = 0; i < cat.countProducts; i++)
            {
                product prod = cat.products[i];

                QTreeWidgetItem *item = new QTreeWidgetItem();
                item->setText(0, QString(prod.name));
                item->setText(1, QString::number(prod.weight));

                items.push_back(item);
            }

            break;
        }
    }

    m_infoTree->insertTopLevelItems(0, items);

    return 0;
}

int GuestWindow::showExpectedGoods()
{
    m_infoTree->clear();

    QList<QTreeWidgetItem*> items;

    deque<categories>::iterator iter;
    for (iter = m_categories->buffer.begin(); iter != m_categories->buffer.end(); iter++)
    {
        categories cat = *iter;

        int k = 0;
        for (int i = 0; i < cat.countProducts; i++)
        {
            if (cat.products[i].weight != 0)
            {
                k++;
            }
        }
        if (k == cat.countProducts)
        {
            continue;
        }

        QTreeWidgetItem *root = new QTreeWidgetItem();
        root->setText(0, QString(cat.categoryName));
        root->setText(1, QString::number(cat.maxWeight));

        items.push_back(root);

        QList<QTreeWidgetItem*> subItems;
        for (int i = 0; i < cat.countProducts; i++)
        {
            product prod = cat.products[i];

            if (prod.weight != 0)
            {
                continue;
            }

            QTreeWidgetItem *subItem = new QTreeWidgetItem();
            subItem->setText(0, QString(prod.name));
            subItem->setText(1, QString::number(prod.weight));

            subItems.push_back(subItem);
        }

        if (subItems.count() == 0)
        {
            continue;
        }

        items.at(items.count() - 1)->addChildren(subItems);
    }

    m_infoTree->insertTopLevelItems(0, items);

    return 0;
}

void GuestWindow::closeEvent(QCloseEvent *)
{
    emit closed();
}
