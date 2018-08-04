#ifndef HTREEWIDGET_H
#define HTREEWIDGET_H

#include <QTreeWidget>
#include "networkapi.h"
class QTreeWidgetItem;
class HTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    HTreeWidget(QWidget* parent = 0);
    ~HTreeWidget();
public:
    void initTree();
    void initConnect();
public slots:
    void doubleClickPointAt(QTreeWidgetItem* pItem,int columm);
};

#endif // HTREEWIDGET_H
