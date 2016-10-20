#ifndef GUESTWINDOW_H
#define GUESTWINDOW_H

#include <QWidget>

#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QList>

#include "categories.h"

class GuestWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GuestWindow(QWidget *parent = 0);

    int showCategory(QString categoryName);
    int showExpectedGoods();

private:
    Categories *m_categories;

    QTreeWidget *m_infoTree;

    void closeEvent(QCloseEvent *);
    
signals:
    void closed();
    
public slots:
    
};

#endif // GUESTWINDOW_H
