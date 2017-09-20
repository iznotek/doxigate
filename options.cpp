/*
 * options.cpp
 *
 *  Created on: 13.08.2008
 */

#include "options.h"

#include <QtGui>

void Option::readFromWidget()
{
    if (!widget) return;

    if (dynamic_cast<QAbstractButton*>(widget)) {
        val = ((QAbstractButton*)widget)->isChecked();
        return;
    }

    if (dynamic_cast<QLineEdit*>(widget)) {
        val = ((QLineEdit*)widget)->text();
        return;
    }
}

void Option::storeToWidget(bool _def)
{
    if (!widget) return;

    if (dynamic_cast<QAbstractButton*>(widget)) {
        ((QAbstractButton*)widget)->setChecked(_def ? def.toBool() : val.toBool());
        return;
    }

    if (dynamic_cast<QLineEdit*>(widget)) {
        ((QLineEdit*)widget)->setText(_def ? def.toString() : val.toString());
        return;
    }
}



Options::Options()
{
    // TODO Auto-generated constructor stub

}

Options::~Options()
{
    // TODO Auto-generated destructor stub
}


void Options::setup(Option &op, const QString &_id, const QVariant &_def, int _numId)
{
    op.setup(_id,_def,_numId);
    myOptions << &op;
}

QList<Option*> Options::options(int id)
{
    QList<Option*> list;
    QList<Option*>::const_iterator it, it_end = myOptions.end();
    for (it = myOptions.begin(); it != it_end; it++) {
        if ((*it)->numId == id)
            list << *it;
    }
    return list;
}

void Options::store(QSettings &set)
{
    QList<Option*>::const_iterator it, it_end = myOptions.end();
    for (it = myOptions.begin(); it != it_end; it++) {
        set.setValue((*it)->id, (*it)->val);
    }
}

void Options::restore(QSettings &set)
{
    QList<Option*>::iterator it, it_end = myOptions.end();
    for (it = myOptions.begin(); it != it_end; it++) {
        (*it)->val = set.value((*it)->id, (*it)->def);
    }
}

void Options::readFromWidgets()
{
    QList<Option*>::iterator it, it_end = myOptions.end();
    for (it = myOptions.begin(); it != it_end; it++) {
        (*it)->readFromWidget();
    }
}

void Options::storeToWidgets()
{
    QList<Option*>::iterator it, it_end = myOptions.end();
    for (it = myOptions.begin(); it != it_end; it++) {
        (*it)->storeToWidget();
    }
}

void Options::storeDefaultsToWidgets()
{
    QList<Option*>::iterator it, it_end = myOptions.end();
    for (it = myOptions.begin(); it != it_end; it++) {
        (*it)->storeToWidget(true);
    }
}

