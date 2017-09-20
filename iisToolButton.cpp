/*
 * iisToolButton.cpp
 *
 *  Created on: 21.07.2008

 */

#include "iisToolButton.h"

iisToolButton::iisToolButton(const QIcon &icon, const QString &label, QWidget *parent, int size) :
    QToolButton(parent),
    myOver(false)
{
    setIcon(icon);
    setText(label);
    setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	if (size)
		setIconSize(QSize(size,size));

    setFocusPolicy(Qt::NoFocus);

//    setStyleSheet(
//            "QPushButton{background-color: transparent; border-width: 0px; color: white;}"
//            ":hover{color: white;}"
//            );

    setProperty("kind","toolbar");
}

iisToolButton::~iisToolButton()
{
}

//void iisToolButton::paintEvent ( QPaintEvent * event )
//{
////    QPainter p(this);
////
////    if (myOver) {
////        //p.setOpacity(0.3);
////        QRadialGradient rg(width()/2,height(),height());
////        rg.setColorAt(0, Qt::cyan);
////        rg.setColorAt(1, Qt::transparent);
////        p.fillRect(rect(), rg);
////    }
////
////   // p.setOpacity(1);
//
//    QToolButton::paintEvent(event);
//}


void iisToolButton::enterEvent ( QEvent * event )
{
    myOver = true;
    QToolButton::enterEvent(event);
}

void iisToolButton::leaveEvent ( QEvent * event )
{
    myOver = false;
    QToolButton::leaveEvent(event);
}
