#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QtGui>

class SetupBasePage;
class ConfigBool;
class iisTreeMenu;

enum PageID {
    PAGE_PPOJECT_ID = 0,
    PAGE_EXTRACT_ID,
    PAGE_BUILD_ID,
    PAGE_BUILD2_ID,

    PAGE_DOT_ID,

    PAGE_MESSAGES_ID,

    PAGE_HTML_ID,
        PAGE_HTML_STYLES_ID,
        PAGE_HTML_TABS_ID,
    PAGE_LATEX_ID,
    PAGE_RTF_ID,
    PAGE_MAN_ID,
    PAGE_XML_ID,
    PAGE_DEF_ID,
    PAGE_PERL_ID
};

class MenuWidget : public QFrame
{
	Q_OBJECT

public:
	MenuWidget(QWidget *parent);
	~MenuWidget();

	void reload();
	void setActivePage(int id);

signals:
    void setup(int id);

    void checked(ConfigBool*, Qt::CheckState check);

public slots:
    void onOptionChecked(ConfigBool*, Qt::CheckState check);

protected slots:
    void onChecked(QTreeWidgetItem *item, Qt::CheckState check);

protected:
    void bindCheckBox(QTreeWidgetItem *item, char *config);

    QMap<QTreeWidgetItem*, ConfigBool*> myCheckMap;

    iisTreeMenu *treeMenu;
};

#endif // MENUWIDGET_H
