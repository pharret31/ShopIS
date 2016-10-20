#ifndef SELLERSADMINTAB_H
#define SELLERSADMINTAB_H

#include <QWidget>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QFile>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QMessageBox>
#include <QVector>
#include <QtWidgets/QTabWidget>
#include <QEvent>
#include <QResizeEvent>

class SellersAdminTab : public QWidget
{
    Q_OBJECT
public:
    explicit SellersAdminTab(QWidget *parent = 0);
    
private:
    QFile *m_loginFile;

    QLineEdit *m_loginLineEdit;
    QLineEdit *m_passwordLineEdit;

    QTableWidget *m_sellers;

    int updateSellersList();
    void stretchColumns();

    bool eventFilter(QObject *obj, QEvent *evt);
    void closeEvent(QCloseEvent *);

signals:
    void closed();

public slots:
    int onAddButtonClick();
    int onRemoveButtonClick();
    
};

#endif // SELLERSADMINTAB_H
