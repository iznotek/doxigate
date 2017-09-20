/*
 * iisStringListEdit.h
 *
 *  Created on: 08.07.2008
 *
 *  Industrial Infosystems (c) 2008
 *  www.ii-system.com
 *
 *  This component is freeware and could be used while building any applications.
 *  Removal/modification of this information not permitted.
 */

#ifndef IISSTRINGLISTEDIT_H_
#define IISSTRINGLISTEDIT_H_

#include <QtWidgets>

////////////////////////////////////////////////////////////////////////////////

enum StringListEditFlags {
    SLF_Custom = 1,
    SLF_Files = 2,
    SLF_Dirs = 4
};

/**
 * \class iisStringListEdit
 * \brief This class is intended to visually edit a QStringList content.
 */
class iisStringListEdit : public QWidget
{
    Q_OBJECT

public:
    iisStringListEdit(QWidget *parent = 0, int flags = SLF_Custom);
    virtual ~iisStringListEdit();

    void setList(const QStringList &list);
    QStringList list() const;

	static QIcon IconPickDir, IconPickFile, IconAdd, IconDelete;

protected slots:
    void add();
    void addFile();
    void addDir();
    void del();

    void onCurrentItemChanged(QListWidgetItem * current, QListWidgetItem * previous);

protected:
	QAction *delAction;

    QListWidgetItem* addItem(const QString &text);
    bool eventFilter(QObject *obj, QEvent *event);

protected:
    QListWidget *myListWidget;
};

////////////////////////////////////////////////////////////////////////////////

/**
 * \class iisStringListEditDialog
 * \brief This class provides a dialog window with iisStringListEdit component inside.
 */
class iisStringListEditDialog : public QDialog
{
public:
    iisStringListEditDialog(const QString &title = QString(),
            int flags = SLF_Custom,
            QWidget *parent = 0,
            Qt::WindowFlags f = 0);
    virtual ~iisStringListEditDialog();

    void setList(const QStringList &list);
    QStringList list() const;

    static bool getList(QStringList &list,
            const QString &title = QString(),
            int flags = SLF_Custom,
			QWidget *parent = 0);

protected:
    iisStringListEdit *myListEdit;
};

////////////////////////////////////////////////////////////////////////////////

#endif /* IISSTRINGLISTEDIT_H_ */
