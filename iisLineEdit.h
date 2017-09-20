/*
 * iisLineEdit.h
 *
 *  Created on: 29.07.2008
 *
 *  Industrial Infosystems (c) 2008
 *  www.ii-system.com
 *
 *  This component is freeware and could be used while building any applications.
 *  Removal/modification of this information not permitted.
 */

#ifndef IISLINEEDIT_H_
#define IISLINEEDIT_H_

#include <QtGui>

/**
 * \class iisLineEdit
 * \brief This class is the base class for line editor with pick button.
 */
class iisLineEdit : public QWidget
{
    Q_OBJECT

public:
    iisLineEdit(QWidget *parent = 0);
    virtual ~iisLineEdit();

    inline QLineEdit* editor() { return myLineEdit; }

    void setPickTitle(const QString &title);

    static int MinHeight, MaxHeight;

signals:
    void buttonClickedOk(QWidget*);
    void textEdited(QWidget*, const QString &);
    void lostFocus(QWidget*);
    void gotFocus(QWidget*);

protected slots:
    virtual void buttonClicked() {};
    void handleTextEdited(const QString &text) { emit textEdited(this, text); }

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);

    QToolButton *myButton;
    QLineEdit *myLineEdit;

    QString myTitlePicker;
};

/**
 * \class iisLineEditDir
 * \brief This class is used to pick a path to existing directory into the line editor.
 */
class iisLineEditDir : public iisLineEdit
{
public:
    iisLineEditDir(QWidget *parent = 0);
    //virtual ~iisLineEditDir();

    static QIcon IconPickDir;

protected:
    virtual void buttonClicked();
};

/**
 * \class iisLineEditFile
 * \brief This class is used to pick a path to existing file into the line editor.
 */
class iisLineEditFile : public iisLineEdit
{
public:
    iisLineEditFile(QWidget *parent = 0);
    //virtual ~iisLineEditDir();

    static QIcon IconPickFile;

protected:
    virtual void buttonClicked();
};

#endif /* IISLINEEDIT_H_ */
