#include "sellorderwidget.h"

SellOrderWidget::SellOrderWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QLabel *wLabel = new QLabel("What amount?");

    m_weightLineEdit = new QLineEdit();

    QPushButton *okButton = new QPushButton("OK");

    mainLayout->addWidget(wLabel);
    mainLayout->addWidget(m_weightLineEdit);
    mainLayout->addWidget(okButton);

    this->setLayout(mainLayout);
    this->setWindowFlags(Qt::Dialog);

    connect(okButton, SIGNAL(clicked()), this, SLOT(onOKButtonClick()));
}

void SellOrderWidget::setIsAdding(bool _isAdding)
{
    m_isAdding = _isAdding;
}

void SellOrderWidget::showEvent(QShowEvent *)
{
    m_weightLineEdit->clear();
    m_weightLineEdit->setFocus();
}

void SellOrderWidget::onOKButtonClick()
{
    emit amount(m_weightLineEdit->text().toInt(), m_isAdding);
    this->close();
}
