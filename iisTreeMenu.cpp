/*
 * iisTreeMenu.cpp
 *
 *  Created on: 15.07.2008

 */

#include "iisTreeMenu.h"

#define SectionType QTreeWidgetItem::UserType+1

iisTreeMenu::iisTreeMenu(QWidget *parent) :
    QTreeWidget(parent)
{
    header()->hide();
    setRootIsDecorated(false);
#if QT_VERSION >= 0x040400
    setExpandsOnDoubleClick(false);
#endif

    connect(this, SIGNAL(currentItemChanged ( QTreeWidgetItem *, QTreeWidgetItem * )),
            this, SLOT(onCurrentItemChanged ( QTreeWidgetItem *, QTreeWidgetItem * )));

    connect(this, SIGNAL(itemChanged ( QTreeWidgetItem *, int )),
            this, SLOT(onItemChanged ( QTreeWidgetItem *, int )));
}

iisTreeMenu::~iisTreeMenu()
{

}

QTreeWidgetItem* iisTreeMenu::addGroup(const QIcon &icon,
            const QString &text,
            QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = 0;
    if (parent)
        item = new QTreeWidgetItem(parent, SectionType);
    else
        item = new QTreeWidgetItem(this, SectionType);

    item->setIcon(0, icon);
    item->setText(0, text);
    item->setFlags(Qt::ItemIsEnabled);

    // set invalid data value
    item->setData(0, iisTreeMenuIdRole, -1);

    QFont f;
    f.setBold(true);
    item->setFont(0, f);

    expandAll();
    return item;
}

QTreeWidgetItem* iisTreeMenu::addItem(const QIcon &icon,
            const QString &text,
            QTreeWidgetItem *parent,
            int checkable,
            const QVariant id,
            int role,
            int column)
{
    QTreeWidgetItem *item = 0;
    if (parent)
        item = new QTreeWidgetItem(parent);
    else
        item = new QTreeWidgetItem(this);

    item->setIcon(0, icon);
    item->setText(0, text);

    if (checkable & iisTreeMenuCheckable) {
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        int ch = checkable & ~iisTreeMenuCheckable;
        item->setCheckState(0, (Qt::CheckState)ch);
        item->setData(0, iisTreeMenuCheckRole, ch);
    }

    item->setData(column, role, id);

    expandAll();
    return item;
}

void iisTreeMenu::setCurrent(int id)
{
    QTreeWidgetItemIterator it(this, QTreeWidgetItemIterator::Selectable);
    while (*it) {
        if ((*it)->data(0, iisTreeMenuIdRole).toInt() == id) {
            setCurrentItem(*it);
            return;
        }
        ++it;
    }
}

void iisTreeMenu::onCurrentItemChanged ( QTreeWidgetItem * current, QTreeWidgetItem * /*previous*/ )
{
    if (!current)
        return;

    if (current->type() == SectionType)
        return;

    emit selected(current);
    emit selected(current->data(0, iisTreeMenuIdRole).toInt());
}

void iisTreeMenu::onItemChanged ( QTreeWidgetItem * item, int column)
{
    if (!item || column)
        return;

    if (item->flags() & Qt::ItemIsUserCheckable) {
        Qt::CheckState ch = item->checkState(0);
        if (ch != item->data(0,iisTreeMenuCheckRole).toInt()) {
            emit checked(item, ch);
            emit checked(item->data(0, iisTreeMenuIdRole).toInt(), ch);
            item->setData(0, iisTreeMenuCheckRole, ch);

            //QMessageBox::aboutQt(this, "");
        }
    }
}


