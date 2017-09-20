/*
 * iisTreeMenu.h
 *
 *  Created on: 15.07.2008
 *
 *  Industrial Infosystems (c) 2008
 *  www.ii-system.com
 *
 *  This component is freeware and could be used while building any applications.
 *  Removal/modification of this information not permitted.
 */

#ifndef IISTREEMENU_H_
#define IISTREEMENU_H_

#include <QtGui>

#define iisTreeMenuIdRole      1000
#define iisTreeMenuCheckRole   1001
#define iisTreeMenuCheckable   0x1000

/**
 * \class iisTreeMenu
 * \brief This class is intended to build tree-like menus.
 */
class iisTreeMenu : public QTreeWidget
{
    Q_OBJECT

public:
    iisTreeMenu(QWidget *parent = 0);
    virtual ~iisTreeMenu();

    QTreeWidgetItem* addGroup(const QIcon &icon,
            const QString &text,
            QTreeWidgetItem *parent = 0
            );

    QTreeWidgetItem* addItem(const QIcon &icon,
            const QString &text,
            QTreeWidgetItem *parent = 0,
            int checkable = 0,
            const QVariant id = 0,
            int role = iisTreeMenuIdRole,
            int column = 0
            );

    void setCurrent(int id);

signals:
    void selected(int id);
    void selected(QTreeWidgetItem *item);

    void checked(int id, Qt::CheckState check);
    void checked(QTreeWidgetItem *item, Qt::CheckState check);

protected slots:
    void onCurrentItemChanged ( QTreeWidgetItem * current, QTreeWidgetItem * previous );
    void onItemChanged ( QTreeWidgetItem * item, int column);
};

#endif /* IISTREEMENU_H_ */
