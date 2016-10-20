#ifndef PROVIDERSADMINTAB_H
#define PROVIDERSADMINTAB_H

#include <QWidget>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTableWidget>
#include <QMessageBox>
#include <QDebug>
#include <QEvent>

#include "categories.h"
#include "guestwindow.h"

class ProvidersAdminTab : public QWidget
{
    Q_OBJECT
public:
    explicit ProvidersAdminTab(QWidget *parent = 0);

private:
    QLineEdit *m_groupNameLineEdit;
    QLineEdit *m_maxWeightLineEdit;

    QTableWidget *m_groups;

    Categories *m_categories;

    int updateGroupsList();
    void stretchColumns();

    bool eventFilter(QObject *obj, QEvent *evt);
    void closeEvent(QCloseEvent *);
    
signals:
    void closed();
    
public slots:
    int onAddButtonClick();
    int onRemoveButtonClick();

    void onTableDoubleClicked(QModelIndex index);
    
};

#endif // PROVIDERSADMINTAB_H
