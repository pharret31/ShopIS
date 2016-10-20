#ifndef SELLORDERWIDGET_H
#define SELLORDERWIDGET_H

#include <QWidget>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

class SellOrderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SellOrderWidget(QWidget *parent = 0);

    void setIsAdding(bool _isAdding);

    void showEvent(QShowEvent *);

private:
    QLineEdit *m_weightLineEdit;

    bool m_isAdding;
    
signals:
    void amount(int weight, bool adding);
    
public slots:
    void onOKButtonClick();
    
};

#endif // SELLORDERWIDGET_H
