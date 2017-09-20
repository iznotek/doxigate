/*
 * iisToolButton.h
 *
 *  Created on: 21.07.2008
 *
 *  Industrial Infosystems (c) 2008
 *  www.ii-system.com
 *
 *  This component is freeware and could be used while building any applications.
 *  Removal/modification of this information not permitted.
 */

#ifndef IISTOOLBUTTON_H_
#define IISTOOLBUTTON_H_

#include <QtGui>

class iisToolButton : public QToolButton
{
public:
    iisToolButton(const QIcon &icon, const QString &label, QWidget *parent = 0, int size = 0);
    virtual ~iisToolButton();

protected:
//    virtual void paintEvent ( QPaintEvent * event );
    virtual void enterEvent ( QEvent * event );
    virtual void leaveEvent ( QEvent * event );

    bool myOver;
};

#endif /* IISTOOLBUTTON_H_ */
