#include "helpwidget.h"

#include "config.h"

HelpWidget::HelpWidget(QWidget *parent)
	: QFrame(parent)
{
	setFrameStyle(QFrame::StyledPanel);

	QVBoxLayout *vbl = new QVBoxLayout();
	setLayout(vbl);

	myHeader = new QLabel(this);
	vbl->addWidget(myHeader);

	myDoc = new QLabel(this);
	vbl->addWidget(myDoc);
	myDoc->setWordWrap(true);

	vbl->addStretch();
}

HelpWidget::~HelpWidget()
{

}

void HelpWidget::helpOption(ConfigOptionPtr option)
{
	help(option->nameString(), option->docString());
}

void HelpWidget::help(const QString &title, const QString &text)
{
	myHeader->setText(
		QString("<b>%1</b>")
		.arg(title)
		);

	myDoc->setText(text);
}

