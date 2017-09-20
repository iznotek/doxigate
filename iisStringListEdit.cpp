/*
 * iisStringListEdit.cpp
 *
 *  Created on: 08.07.2008

 */

#include "iisStringListEdit.h"

////////////////////////////////////////////////////////////////////////////////

QIcon iisStringListEdit::IconPickDir;
QIcon iisStringListEdit::IconPickFile;
QIcon iisStringListEdit::IconAdd;
QIcon iisStringListEdit::IconDelete;

////////////////////////////////////////////////////////////////////////////////

iisStringListEdit::iisStringListEdit(QWidget *parent, int flags) : QWidget(parent)
{
    QVBoxLayout *vbl = new QVBoxLayout();
    setLayout(vbl);
    vbl->setMargin(0);

    QHBoxLayout *hbl = new QHBoxLayout();
    vbl->addLayout(hbl);
    hbl->addStretch();

    QToolBar *toolBar = new QToolBar(this);
    hbl->addWidget(toolBar);
    toolBar->setIconSize(QSize(16,16));

    QAction *a = toolBar->addAction(IconAdd, "", this, SLOT(add()));
    a->setToolTip(tr("Add Item"));

    delAction = toolBar->addAction(IconDelete, "", this, SLOT(del()));
    delAction->setToolTip(tr("Remove Current Item"));
	delAction->setEnabled(false);

    if (flags & SLF_Files) {
        a = toolBar->addAction(IconPickFile, "", this, SLOT(addFile()));
        a->setToolTip(tr("Add File"));
    }

    if (flags & SLF_Dirs) {
        a = toolBar->addAction(IconPickDir, "", this, SLOT(addDir()));
        a->setToolTip(tr("Add Directory"));
    }

    myListWidget = new QListWidget(this);
    vbl->addWidget(myListWidget);

    myListWidget->installEventFilter(this);

    connect(myListWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(onCurrentItemChanged(QListWidgetItem*,QListWidgetItem*)));
}

iisStringListEdit::~iisStringListEdit()
{
}

void iisStringListEdit::setList(const QStringList &list)
{
	for (int i = 0; i < list.count(); i++) {
		QString s = list.at(i);
		if (s.simplified().isEmpty())
			continue;
		QListWidgetItem *wi = new QListWidgetItem(myListWidget);
		wi->setText(s);
		wi->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	}

    //myListWidget->addItems(list);
    //for (int i = 0; i < myListWidget->count(); i++) {
    //    myListWidget->item(i)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
    //}
	delAction->setEnabled(myListWidget->count());
}

QStringList iisStringListEdit::list() const
{
    QStringList sl;
    for (int i = 0; i < myListWidget->count(); i++) {
        QString s = myListWidget->item(i)->text().simplified();
        if (!s.isEmpty())
            sl << s;
    }
    return sl;
}

void iisStringListEdit::add()
{
    QListWidgetItem *wi = addItem("");
    myListWidget->editItem(wi);
	delAction->setEnabled(myListWidget->count());
}

void iisStringListEdit::del()
{
	if (!myListWidget->count())
		return;

    int i = myListWidget->currentRow();

	myListWidget->blockSignals(true);

	QListWidgetItem *wi = myListWidget->takeItem(i);
    delete wi;

	myListWidget->blockSignals(false);

	myListWidget->setCurrentRow(qMin(i,myListWidget->count()-1));

	delAction->setEnabled(myListWidget->count());
}

void iisStringListEdit::onCurrentItemChanged(
        QListWidgetItem * /*current*/, QListWidgetItem * previous )
{
    if (previous && previous->text().trimmed().isEmpty())
        delete previous;

	delAction->setEnabled(myListWidget->count());
}

bool iisStringListEdit::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == myListWidget) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            switch (keyEvent->key()) {
                case Qt::Key_Insert:
                    add();
                    return true;
                case Qt::Key_Delete:
                    del();
                    return true;
                case Qt::Key_Return: {
                    QListWidgetItem *wi = myListWidget->currentItem();
                    if (wi) {
                        myListWidget->editItem(wi);
						delAction->setEnabled(myListWidget->count());
                        return true;
                    }
                    break;
                }
                case Qt::Key_Down:
                    if (myListWidget->currentRow() == myListWidget->count()-1) {
                        add();
                        return true;
                    }
                    break;

                default:;
            }
        }
    }

    return QObject::eventFilter(obj, event);
}

void iisStringListEdit::addDir()
{
    QString dir = QFileDialog::getExistingDirectory(this,
           "", "");

    if (!dir.isEmpty()) {
        addItem(dir);
    }
}

void iisStringListEdit::addFile()
{
    QString file = QFileDialog::getOpenFileName(this,
            "", "");

    if (!file.isEmpty()) {
        addItem(file);
    }
}

QListWidgetItem* iisStringListEdit::addItem(const QString &text)
{
    QListWidgetItem *wi = new QListWidgetItem(text, myListWidget);
    wi->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
    myListWidget->setCurrentItem(wi);
	delAction->setEnabled(myListWidget->count());
    return wi;
}

////////////////////////////////////////////////////////////////////////////////

iisStringListEditDialog::iisStringListEditDialog(const QString &title,
        int flags, QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f)
{
	setObjectName("iisStringListEditDialog");
    setWindowTitle(title);
    resize(400,240);

    QVBoxLayout *vbl = new QVBoxLayout();
    setLayout(vbl);

    myListEdit = new iisStringListEdit(this, flags);
    vbl->addWidget(myListEdit);

    QDialogButtonBox *myBB = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
            Qt::Horizontal, this);
    vbl->addWidget(myBB);
    connect(myBB, SIGNAL(accepted()), this, SLOT(accept()));
    connect(myBB, SIGNAL(rejected()), this, SLOT(reject()));
}

iisStringListEditDialog::~iisStringListEditDialog()
{

}

void iisStringListEditDialog::setList(const QStringList &list)
{
    myListEdit->setList(list);
}

QStringList iisStringListEditDialog::list() const
{
    return myListEdit->list();
}

bool iisStringListEditDialog::getList(QStringList &list,
        const QString &title, int flags, QWidget *parent)
{
    iisStringListEditDialog sld(title, flags, parent);
	//sld.setStyle(qApp->style());
	//sld.setStyleSheet(qApp->styleSheet());
    sld.setList(list);
    if (sld.exec() == QDialog::Accepted) {
        list = sld.list();
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////

