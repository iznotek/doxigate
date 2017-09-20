#ifndef SETUPWIDGET_H
#define SETUPWIDGET_H

#include <QStackedWidget>

class SetupWidget : public QStackedWidget
{
	Q_OBJECT

public:
	SetupWidget(QWidget *parent);
	~SetupWidget();
};

#endif // SETUPWIDGET_H
