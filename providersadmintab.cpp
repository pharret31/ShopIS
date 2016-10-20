#include "providersadmintab.h"

ProvidersAdminTab::ProvidersAdminTab(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *groupNameLayout = new QHBoxLayout();

    QLabel *groupNameLabel = new QLabel(tr("Input group name here: "));
    m_groupNameLineEdit = new QLineEdit();

    groupNameLayout->addWidget(groupNameLabel);
    groupNameLayout->addStretch();
    groupNameLayout->addWidget(m_groupNameLineEdit);

    QHBoxLayout *maxWeightLayout = new QHBoxLayout();

    QLabel *maxWeightLabel = new QLabel(tr("Input max weight here: "));
    m_maxWeightLineEdit = new QLineEdit();

    maxWeightLayout->addWidget(maxWeightLabel);
    maxWeightLayout->addStretch();
    maxWeightLayout->addWidget(m_maxWeightLineEdit);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();

    QPushButton *addButton = new QPushButton("Add");
    QPushButton *removeButton = new QPushButton("Remove");

    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(removeButton);

    m_groups = new QTableWidget();

    m_groups->installEventFilter(this);
    m_groups->setColumnCount(3);
    m_groups->setSelectionMode(QAbstractItemView::SingleSelection);

    m_groups->setHorizontalHeaderLabels(QString("Name;Max weight;Provider").split(";"));

    m_categories = new Categories((char*)"base.grs");

    updateGroupsList();

    QPushButton *okButton = new QPushButton("OK");

    QVBoxLayout *mainLayout = new QVBoxLayout();

    mainLayout->addLayout(groupNameLayout);
    mainLayout->addLayout(maxWeightLayout);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(m_groups);
    mainLayout->addWidget(okButton);

    this->setLayout(mainLayout);
    this->setWindowFlags(Qt::Dialog);

    connect(addButton, SIGNAL(clicked()), this, SLOT(onAddButtonClick()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(onRemoveButtonClick()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_groups, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onTableDoubleClicked(QModelIndex)));
}

int ProvidersAdminTab::updateGroupsList()
{
    m_categories->updateBuffer();

    m_groups->clearContents();
    m_groups->setRowCount(0);

    deque<categories>::iterator iter;
    for (iter = m_categories->buffer.begin(); iter != m_categories->buffer.end(); iter++)
    {
        categories prod = *iter;
        QTableWidgetItem *categoryName = new QTableWidgetItem();
        categoryName->setText(prod.categoryName);
        QTableWidgetItem *maxWeight = new QTableWidgetItem();
        maxWeight->setText(QString::number(prod.maxWeight));

        m_groups->setRowCount(m_groups->rowCount() + 1);
        m_groups->setItem(m_groups->rowCount() - 1, 0, categoryName);
        m_groups->setItem(m_groups->rowCount() - 1, 1, maxWeight);
    }

    return 0;
}

void ProvidersAdminTab::stretchColumns()
{
    m_groups->setColumnWidth(0, m_groups->width() / 2 - 10);
    m_groups->setColumnWidth(1, m_groups->width() / 2 - 10);
}

void ProvidersAdminTab::closeEvent(QCloseEvent *)
{
    emit closed();
}

int ProvidersAdminTab::onAddButtonClick()
{
    if (m_groupNameLineEdit->text().isEmpty() || m_maxWeightLineEdit->text().isEmpty())
    {
        QMessageBox *errorMB = new QMessageBox();
        errorMB->critical(0, tr("Error!"), tr("Something is empty!"));

        return -2;
    }

    if (m_maxWeightLineEdit->text().toInt() < 0)
    {
        QMessageBox *errorMB = new QMessageBox();
        errorMB->critical(0, tr("Error!"), tr("Category is not negative!"));

        return -2;
    }

    deque<categories>::iterator iter;
    bool del = false;
    for (iter = m_categories->buffer.begin(); iter != m_categories->buffer.end(); iter++)
    {
        categories prod = *iter;
        if (m_groupNameLineEdit->text() == prod.categoryName)
        {
            QMessageBox *errorMB = new QMessageBox();
            errorMB->setText("Error!");
            errorMB->setInformativeText("This group already have! Replace it?");
            errorMB->setIcon(QMessageBox::Critical);
            errorMB->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            int ret = errorMB->exec();

            switch (ret)
            {
            case QMessageBox::Ok:

                del = true;
                break;

            case QMessageBox::Cancel:
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
        m_categories->buffer.erase(iter);

        categories prod1;

        strcpy(prod1.categoryName, m_groupNameLineEdit->text().toLatin1().data());
        prod1.maxWeight = m_maxWeightLineEdit->text().toInt();
        prod1.countProducts = 0;
        m_categories->buffer.push_back(prod1);

        m_categories->saveBuffer();

        m_groupNameLineEdit->clear();
        m_maxWeightLineEdit->clear();

        updateGroupsList();

        return 1;
    }

    categories prod;
    strcpy(prod.categoryName, m_groupNameLineEdit->text().toLatin1().data());
    prod.maxWeight = m_maxWeightLineEdit->text().toInt();
    prod.countProducts = 0;
    m_categories->buffer.push_back(prod);

    m_categories->saveBuffer();

    m_groupNameLineEdit->clear();
    m_maxWeightLineEdit->clear();

    updateGroupsList();

    return 0;
}

int ProvidersAdminTab::onRemoveButtonClick()
{
    if (m_groups->rowCount() == 0 || !m_groups->currentItem()->isSelected())
    {
        QMessageBox *errorMB = new QMessageBox();
        errorMB->critical(0, tr("Error!"), tr("Nothing is selected!"));

        return -1;
    }

    QString selectedCategory = m_groups->item(m_groups->currentItem()->row(), 0)->text();
    deque<categories>::iterator iter;
    for (iter = m_categories->buffer.begin(); iter != m_categories->buffer.end(); iter++)
    {
        categories p = *iter;
        if (p.categoryName == selectedCategory)
        {
            m_categories->buffer.erase(iter);

            break;
        }
    }

    m_categories->saveBuffer();

    updateGroupsList();

    return 0;
}

void ProvidersAdminTab::onTableDoubleClicked(QModelIndex index)
{
    GuestWindow *gw = new GuestWindow();
    gw->showCategory(m_groups->item(index.row(), 0)->text());
    gw->show();
}

bool ProvidersAdminTab::eventFilter(QObject *obj, QEvent *evt)
{
    if (evt->type() == QEvent::Resize)
    {
        stretchColumns();
    }

    return QWidget::eventFilter(obj, evt);
}
