#include "htreewidget.h"
#include <QTreeWidgetItem>

HTreeWidget::HTreeWidget(QWidget* parent):QTreeWidget(parent)
{
    initTree();
}

HTreeWidget::~HTreeWidget()
{

}

void HTreeWidget::initTree()
{
    //读取服务器信息
    QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidgetItem*)0,TREE_NET_NULL);
    item->setText(0,QStringLiteral("报文信息"));

    QTreeWidgetItem *itemProtocol = new QTreeWidgetItem((QTreeWidgetItem*)0,TREE_PROTOCOL_ALL);
    itemProtocol->setText(0,QStringLiteral("规约层报文"));
    QTreeWidgetItem *itemProtocolA = new QTreeWidgetItem(itemProtocol,TREE_PROTOCOL_A);
    itemProtocolA->setText(0,QStringLiteral("规约层A网报文"));
    QTreeWidgetItem *itemProtocolB = new QTreeWidgetItem(itemProtocol,TREE_PROTOCOL_B);
    itemProtocolB->setText(0,QStringLiteral("规约层B网报文"));
    addTopLevelItem(itemProtocol);


    QTreeWidgetItem *itemnet = new QTreeWidgetItem((QTreeWidgetItem*)0,TREE_NET_ALL);
    itemnet->setText(0,QStringLiteral("网络层报文"));
    QTreeWidgetItem *itemA = new QTreeWidgetItem(itemnet,TREE_NET_A);
    itemA->setText(0,QStringLiteral("网络层A网报文"));
    QTreeWidgetItem *itemB = new QTreeWidgetItem(itemnet,TREE_NET_B);
    itemB->setText(0,QStringLiteral("网络层B网报文"));
    addTopLevelItem(itemnet);
}

void HTreeWidget::initConnect()
{
    //connect(this,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
}
