#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QWidget>

#include <QTabWidget>
#include <QVBoxLayout>

#include "sellersadmintab.h"
#include "providersadmintab.h"

class AdminWindow : public QWidget
{
    Q_OBJECT
public:
    explicit AdminWindow(QWidget *parent = 0);

private:
    void closeEvent(QCloseEvent *);
    
signals:
    void closed();
    
};

#endif // ADMINWINDOW_H
