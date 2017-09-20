/*
 * outputwidget.cpp
 *
 *  Created on: 28.07.2008

 */

#include "outputwidget.h"

OutputWidget::OutputWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *myLayout = new QVBoxLayout();
    myLayout->setMargin(0);
    setLayout(myLayout);

    QGroupBox *gb = new QGroupBox(this);
    QHBoxLayout *hbl = new QHBoxLayout();
    gb->setLayout(hbl);

    myLayout->addWidget(gb);

    myTimeLabel = new QLabel("", gb);
    hbl->addWidget(myTimeLabel);

    hbl->addStretch();

    myStopButton = new QPushButton(tr("Abort"), gb);
    hbl->addWidget(myStopButton);
    connect(myStopButton, SIGNAL(clicked()), this, SIGNAL(abort()));

    myOutput = new QTextBrowser(this);
    myOutput->setOpenExternalLinks(true);
    myLayout->addWidget(myOutput);

    setRunning(false);
}

OutputWidget::~OutputWidget()
{
    // TODO Auto-generated destructor stub
}

void OutputWidget::setTime(int ms)
{
    int s = ms/1000;
    int m = ms/(1000*60);
    int h = ms/(1000*60*60);
    QString time = tr("Running time: <b>%1.%2:%3</b>")
        .arg(h,2,10,QChar('0'))
        .arg(m,2,10,QChar('0'))
        .arg(s,2,10,QChar('0'));
    myTimeLabel->setText(time);
}

void OutputWidget::showMessage(const QString &text, int type)
{
    QString s(text);
    s = s.replace("\n", "<br>");

    switch (type) {
        case MS_COMMAND:
            s = "<font color=#003366><b>" + s + "</b></font>";
            break;
        case MS_ERROR:
            s = "<font color=#660000><b>" + s + "</b></font>";
            break;
        case MS_WARNING:
            s = "<font color=#666600><b>" + s + "</b></font>";
            break;
        case MS_OK:
            s = "<font color=#006633><b>" + s + "</b></font>";
            break;

        default:
            s = "<font color=#000000>" + s + "</font>";
    }

    myOutput->append(s);
}

void OutputWidget::clearMessages()
{
    myOutput->clear();
}

void OutputWidget::setRunning(bool b)
{
    myStopButton->setEnabled(b);
}

void OutputWidget::addLine()
{
    myOutput->append("<hr>");
}


