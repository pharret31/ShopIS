#include "sellerwindow.h"

SellerWindow::SellerWindow(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *nameLayout = new QHBoxLayout();

    QLabel *nameLabel = new QLabel(tr("Input product name here: "));
    m_nameLineEdit = new QLineEdit();

    nameLayout->addWidget(nameLabel);
    nameLayout->addStretch();
    nameLayout->addWidget(m_nameLineEdit);

    QHBoxLayout *weightLayout = new QHBoxLayout();

    QLabel *weightLabel = new QLabel(tr("Input current weight here: "));
    m_weightLineEdit = new QLineEdit();

    weightLayout->addWidget(weightLabel);
    weightLayout->addStretch();
    weightLayout->addWidget(m_weightLineEdit);

    QHBoxLayout *provNameLayout = new QHBoxLayout();

    QLabel *provNameLabel = new QLabel(tr("Input provider name here: "));
    m_provNameLineEdit = new QLineEdit();

    provNameLayout->addWidget(provNameLabel);
    provNameLayout->addStretch();
    provNameLayout->addWidget(m_provNameLineEdit);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();

    QPushButton *addButton = new QPushButton("Add");
    QPushButton *removeButton = new QPushButton("Remove");

    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(removeButton);

    m_category = new QComboBox();

    m_categories = new Categories((char*)"base.grs");

    updateCategoriesList();

    QHBoxLayout *infoLayout = new QHBoxLayout();

    QLabel *mwLabel = new QLabel(tr("Max weight: "));
    QLabel *awLabel = new QLabel(tr("Avail. weight: "));
    m_maxWeight = new QLabel("0");
    m_availWeight = new QLabel("0");

    infoLayout->addWidget(mwLabel);
    infoLayout->addWidget(m_maxWeight);
    infoLayout->addStretch();
    infoLayout->addWidget(awLabel);
    infoLayout->addWidget(m_availWeight);

    m_products = new QTableWidget();

    m_products->installEventFilter(this);
    m_products->setColumnCount(3);
    m_products->setSelectionMode(QAbstractItemView::SingleSelection);

    m_products->setHorizontalHeaderLabels(QString(tr("Name;Weight;Provider")).split(";"));

    m_sellerMenu = new QMenu(m_products);

    m_sellerMenu->addAction(tr("Order"));
    m_sellerMenu->addAction(tr("Sell"));

    QPushButton *expectButton = new QPushButton(tr("Show expected goods"));

    QPushButton *okButton = new QPushButton("OK");

    QVBoxLayout *mainLayout = new QVBoxLayout();

    mainLayout->addLayout(nameLayout);
    mainLayout->addLayout(weightLayout);
    mainLayout->addLayout(provNameLayout);
    mainLayout->addWidget(m_category);
    mainLayout->addLayout(infoLayout);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(m_products);
    mainLayout->addWidget(expectButton);
    mainLayout->addWidget(okButton);

    this->setLayout(mainLayout);
    this->setWindowFlags(Qt::Dialog);

    m_soWidget = new SellOrderWidget();

    connect(m_sellerMenu, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuTriggered(QAction*)));
    connect(m_soWidget, SIGNAL(amount(int,bool)), this, SLOT(changeAmount(int,bool)));
    connect(expectButton, SIGNAL(clicked()), this, SLOT(onExpectButtonClick()));
    connect(m_category, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateProductsList(QString)));
    connect(addButton, SIGNAL(clicked()), this, SLOT(onAddButtonClick()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(onRemoveButtonClick()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(close()));
}

void SellerWindow::contextMenuEvent(QContextMenuEvent *cme)
{
    m_sellerMenu->exec(cme->globalPos());
}

int SellerWindow::updateCategoriesList()
{
    m_categories->updateBuffer();

    m_category->clear();
    m_category->insertItem(0, "Choose a category: ");
    m_category->insertSeparator(1);

    deque<categories>::iterator iter;
    for (iter = m_categories->buffer.begin(); iter != m_categories->buffer.end(); iter++)
    {
        categories cat = *iter;

        m_category->insertItem(m_category->count(), cat.categoryName);
    }

    return 0;
}

void SellerWindow::stretchColumns()
{
    m_products->setColumnWidth(0, m_products->width() / 3 - 10);
    m_products->setColumnWidth(1, m_products->width() / 3 - 10);
    m_products->setColumnWidth(2, m_products->width() / 3 - 10);
}

bool SellerWindow::eventFilter(QObject *obj, QEvent *evt)
{
    if (evt->type() == QEvent::Resize)
    {
        stretchColumns();
    }

    return QWidget::eventFilter(obj, evt);
}

int SellerWindow::updateProductsList(QString categoryName)
{
    m_categories->updateBuffer();

    m_products->clearContents();
    m_products->setRowCount(0);

    deque<categories>::iterator iter;
    for (iter = m_categories->buffer.begin(); iter != m_categories->buffer.end(); iter++)
    {
        categories cat = *iter;

        if (cat.categoryName == categoryName)
        {
            int weight = 0;

            for (int i = 0; i < cat.countProducts; i++)
            {
                product prod = cat.products[i];

                QTableWidgetItem *productName = new QTableWidgetItem();
                productName->setText(prod.name);
                QTableWidgetItem *productWeight = new QTableWidgetItem();
                productWeight->setText(QString::number(prod.weight));
                QTableWidgetItem *providerName = new QTableWidgetItem();
                providerName->setText(prod.providerName);

                m_products->setRowCount(m_products->rowCount() + 1);
                m_products->setItem(m_products->rowCount() - 1, 0, productName);
                m_products->setItem(m_products->rowCount() - 1, 1, productWeight);
                m_products->setItem(m_products->rowCount() - 1, 2, providerName);

                weight += cat.products[i].weight;
            }

            m_maxWeight->setText(QString::number(cat.maxWeight));
            m_availWeight->setText(QString::number(cat.maxWeight - weight));

            break;
        }
    }

    return 0;
}

void SellerWindow::contextMenuTriggered(QAction *act)
{
    if (act->text() == "Order")
    {
        m_soWidget->setIsAdding(true);
        m_soWidget->show();
    }
    else if (act->text() == "Sell")
    {
        m_soWidget->setIsAdding(false);
        m_soWidget->show();
    }
}

int SellerWindow::changeAmount(int weight, bool isAdding)
{
    deque<categories>::iterator iter;
    for (iter = m_categories->buffer.begin(); iter != m_categories->buffer.end(); iter++)
    {
        categories cat = *iter;

        if (m_category->currentText() == cat.categoryName)
        {
            int j = isAdding ? 1 : -1;

            QString selectedProduct = m_products->item(m_products->currentItem()->row(), 0)->text();
            for (int i = 0; i < cat.countProducts; i++)
            {
                if (cat.products[i].name == selectedProduct)
                {
                    int availWeight = m_availWeight->text().toInt();

                    if (j == 1 && availWeight < weight * j)
                    {
                        QMessageBox *errorMB = new QMessageBox();
                        errorMB->critical(0, tr("Error!"), tr("It's very much!"));

                        return -4;
                    }
                    else if (j == -1 && weight * j + cat.products[i].weight < 0)
                    {
                        QMessageBox *errorMB = new QMessageBox();
                        errorMB->critical(0, tr("Error!"), tr("Item not enough!"));

                        return -5;
                    }

                    cat.products[i].weight += weight * j;
                }
            }
            m_categories->buffer.erase(iter);

            m_categories->buffer.push_back(cat);

            m_categories->saveBuffer();

            updateProductsList(cat.categoryName);

            break;
        }
    }

    return 0;
}

int SellerWindow::onAddButtonClick()
{
    if (m_nameLineEdit->text().isEmpty() || m_weightLineEdit->text().isEmpty())
    {
        QMessageBox *errorMB = new QMessageBox();
        errorMB->critical(0, tr("Error!"), tr("Something is empty!"));

        return -2;
    }

    if (m_weightLineEdit->text().toInt() < 0)
    {
        QMessageBox *errorMB = new QMessageBox();
        errorMB->critical(0, tr("Error!"), tr("Product is not negative!"));

        return -2;
    }

    product prod;
    strcpy(prod.name, m_nameLineEdit->text().toLatin1().data());
    prod.weight = m_weightLineEdit->text().toInt();
    strcpy(prod.providerName, m_provNameLineEdit->text().toLatin1().data());

    deque<categories>::iterator iter;
    for (iter = m_categories->buffer.begin(); iter != m_categories->buffer.end(); iter++)
    {
        categories cat = *iter;

        if (m_category->currentText() == cat.categoryName)
        {
            if (m_weightLineEdit->text().toInt() > m_availWeight->text().toInt())
            {
                QMessageBox *errorMB = new QMessageBox();
                errorMB->critical(0, tr("Error!"), tr("It's very much!"));

                m_weightLineEdit->clear();

                return -4;
            }

            bool del = false;
            for (int i = 0; i < cat.countProducts; i++)
            {
                if (strcmp(cat.products[i].name, m_nameLineEdit->text().toLatin1().data()) == 0)
                {
                    QMessageBox *errorMB = new QMessageBox();
                    errorMB->setText("Error!");
                    errorMB->setInformativeText("This product already have! Replace it?");
                    errorMB->setIcon(QMessageBox::Critical);
                    errorMB->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                    int ret = errorMB->exec();

                    switch (ret)
                    {
                    case QMessageBox::Ok:

                        m_categories->buffer.erase(iter);

                        cat.products[i].weight = prod.weight;
                        strcpy(cat.products[i].providerName, prod.providerName);

                        m_categories->buffer.push_back(cat);

                        m_categories->saveBuffer();

                        updateProductsList(cat.categoryName);

                        del = true;

                        break;

                    case QMessageBox::Cancel:

                        m_nameLineEdit->clear();
                        m_weightLineEdit->clear();

                        return -3;
                    }

                    if (del)
                    {
                        break;
                    }
                }
            }

            if (del)
            {
                break;
            }

            m_categories->buffer.erase(iter);

            cat.countProducts++;
            cat.products[cat.countProducts - 1] = prod;

            m_categories->buffer.push_back(cat);

            m_categories->saveBuffer();

            updateProductsList(cat.categoryName);

            break;
        }
    }

    m_nameLineEdit->clear();
    m_weightLineEdit->clear();

    return 0;
}

int SellerWindow::onRemoveButtonClick()
{
    if (m_products->rowCount() == 0 || !m_products->currentItem()->isSelected())
    {
        QMessageBox *errorMB = new QMessageBox();
        errorMB->critical(0, tr("Error!"), tr("Nothing is selected!"));

        return -1;
    }

    deque<categories>::iterator iter;
    for (iter = m_categories->buffer.begin(); iter != m_categories->buffer.end(); iter++)
    {
        categories cat = *iter;

        if (m_category->currentText() == cat.categoryName)
        {
            m_categories->buffer.erase(iter);

            QString selectedProduct = m_products->item(m_products->currentItem()->row(), 0)->text();
            for (int i = 0; i < cat.countProducts; i++)
            {
                if (cat.products[i].name == selectedProduct)
                {
                    for (int j = i; j < cat.countProducts - 1; j++)
                    {
                        strcpy(cat.products[j].name, cat.products[j + 1].name);
                        cat.products[j].weight = cat.products[j + 1].weight;
                    }

                    break;
                }
            }
            cat.countProducts--;

            m_categories->buffer.push_back(cat);

            m_categories->saveBuffer();

            updateProductsList(cat.categoryName);

            break;
        }
    }

    return 0;
}

void SellerWindow::onExpectButtonClick()
{
    GuestWindow *gw = new GuestWindow();
    gw->showExpectedGoods();
    gw->show();
}

void SellerWindow::closeEvent(QCloseEvent *)
{
    emit closed();
}
