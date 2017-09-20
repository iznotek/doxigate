#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QtWidgets>

class ConfigOption;

class HelpWidget : public QFrame
{
	Q_OBJECT

public:
	HelpWidget(QWidget *parent);
	~HelpWidget();

protected slots:
	void helpOption(ConfigOptionPtr);
	void help(const QString&, const QString&);

private:
	QLabel *myHeader;
	QLabel *myDoc;
};

#endif // HELPWIDGET_H
