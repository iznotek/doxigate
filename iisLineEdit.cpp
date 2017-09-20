/*
 * iisLineEdit.cpp
 *
 *  Created on: 29.07.2008

 */

#include "iisLineEdit.h"

int iisLineEdit::MinHeight = 0;
int iisLineEdit::MaxHeight = 0;

iisLineEdit::iisLineEdit(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *hbl = new QHBoxLayout();
    hbl->setMargin(0);
    hbl->setSpacing(0);
    setLayout(hbl);

    myLineEdit = new QLineEdit(this);
    myLineEdit->installEventFilter(this);
    if (MinHeight)
        myLineEdit->setMinimumHeight(MinHeight);
    if (MaxHeight)
        myLineEdit->setMaximumHeight(MaxHeight);

    myButton = new QToolButton(this);
    myButton->setFocusPolicy(Qt::NoFocus);

    hbl->addWidget(myLineEdit);
    hbl->addWidget(myButton);

    connect(myButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    connect(myLineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(handleTextEdited(const QString &)));
}

iisLineEdit::~iisLineEdit()
{
    // TODO Auto-generated destructor stub
}

bool iisLineEdit::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == myLineEdit) {
        switch (event->type()) {
            case QEvent::FocusOut:
                emit lostFocus(this);
                break;
            case QEvent::FocusIn:
                emit gotFocus(this);
                break;

            case QEvent::Resize: {
                int w = ((QResizeEvent*)event)->size().height();
                myButton->setFixedSize(w,w);
                return true;
            }
            default:;
        }
    }

    return QObject::eventFilter(obj, event);
}

void iisLineEdit::setPickTitle(const QString &title)
{
    myTitlePicker = title;
}

////////////////////////////////////////////////////////////////////////////////

QIcon iisLineEditDir::IconPickDir;

iisLineEditDir::iisLineEditDir(QWidget *parent) : iisLineEdit(parent)
{
    myButton->setIcon(IconPickDir);
}

void iisLineEditDir::buttonClicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
            myTitlePicker, myLineEdit->text());

    if (!dir.isEmpty()) {
        myLineEdit->setText(dir);
        emit buttonClickedOk(this);
    }
}

////////////////////////////////////////////////////////////////////////////////

QIcon iisLineEditFile::IconPickFile;

iisLineEditFile::iisLineEditFile(QWidget *parent) : iisLineEdit(parent)
{
    myButton->setIcon(IconPickFile);
}

void iisLineEditFile::buttonClicked()
{
    QString file = QFileDialog::getOpenFileName(this,
            myTitlePicker, myLineEdit->text());

    if (!file.isEmpty()) {
        myLineEdit->setText(file);
        emit buttonClickedOk(this);
    }
}
