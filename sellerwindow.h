#ifndef SELLERWINDOW_H
#define SELLERWINDOW_H

#include <QWidget>

#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMenu>
#include <QContextMenuEvent>

#include "categories.h"
#include "sellorderwidget.h"
#include "guestwindow.h"

class SellerWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SellerWindow(QWidget *parent = 0);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *cme);

private:
    QLineEdit *m_nameLineEdit;
    QLineEdit *m_weightLineEdit;
    QLineEdit *m_provNameLineEdit;

    QComboBox *m_category;

    QLabel *m_maxWeight;
    QLabel *m_availWeight;

    QMenu *m_sellerMenu;
    QTableWidget *m_products;
    SellOrderWidget *m_soWidget;

    Categories *m_categories;

    int updateCategoriesList();
    void stretchColumns();

    bool eventFilter(QObject *obj, QEvent *evt);
    void closeEvent(QCloseEvent *);
    
signals:
    void closed();
    
public slots:
    int onAddButtonClick();
    int onRemoveButtonClick();

    void onExpectButtonClick();

    int updateProductsList(QString categoryName);

    void contextMenuTriggered(QAction *act);
    int changeAmount(int weight, bool isAdding);
    
};

#endif // SELLERWINDOW_H
