/*
 * outputwidget.h
 *
 *  Created on: 28.07.2008

 */

#ifndef OUTPUTWIDGET_H_
#define OUTPUTWIDGET_H_

#include <QtGui>

enum MessageType
{
    MS_TEXT,
    MS_COMMAND,
    MS_OK,
    MS_ERROR,
    MS_WARNING
};

class OutputWidget : public QWidget
{
    Q_OBJECT

public:
    OutputWidget(QWidget *parent);
    virtual ~OutputWidget();

    void setRunning(bool);
    void setTime(int ms);

public slots:
    void showMessage(const QString &text, int type);
    void addLine();
    void clearMessages();

signals:
    void abort();

protected:
    QTextBrowser *myOutput;

    QLabel *myTimeLabel;
    QPushButton *myStopButton;
};

#endif /* OUTPUTWIDGET_H_ */
