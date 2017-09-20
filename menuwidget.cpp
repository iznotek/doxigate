#include "menuwidget.h"

#include "iisTreeMenu.h"

#include "config.h"

MenuWidget::MenuWidget(QWidget *parent)
	: QFrame(parent)
{
    setFrameStyle(QFrame::StyledPanel);

    QVBoxLayout *myLayout = new QVBoxLayout();
    myLayout->setMargin(0);
    setLayout(myLayout);

    treeMenu = new iisTreeMenu(this);
    myLayout->addWidget(treeMenu);

    treeMenu->setIconSize(QSize(32,32));

    QTreeWidgetItem *item = treeMenu->addGroup(QIcon(),
            tr("General"));

    QTreeWidgetItem *subitem = treeMenu->addItem(QIcon(),
            tr("Project"),
            item,
            0,
            PAGE_PPOJECT_ID);

    QTreeWidgetItem *subitem2;

    item = treeMenu->addGroup(QIcon(),
            tr("Build"));

    subitem = treeMenu->addItem(QIcon(),
            tr("Code Analysis"),
            item,
            0,
            PAGE_EXTRACT_ID);

    subitem = treeMenu->addItem(QIcon(),
            tr("Common Output"),
            item,
            false,
            PAGE_BUILD_ID);

    subitem = treeMenu->addItem(QIcon(),
            tr("Additional Output"),
            item,
            0,
            PAGE_BUILD2_ID);

    subitem = treeMenu->addItem(QIcon(),
            tr("Class Diagrams"),
            item,
            0,
            PAGE_DOT_ID);


    item = treeMenu->addGroup(QIcon(),
            tr("Control"));

    subitem = treeMenu->addItem(QIcon(),
            tr("Messages"),
            item,
            0,
            PAGE_MESSAGES_ID);


    item = treeMenu->addGroup(QIcon(),
            tr("Output Formats"));

    subitem = treeMenu->addItem(
			QIcon(":/gui/icon32_html.png"),
            tr("HTML & CHM"),
            item,
            iisTreeMenuCheckable,
            PAGE_HTML_ID);

    bindCheckBox(subitem, "GENERATE_HTML");


    //subitem2 = treeMenu->addItem(
    //                    QIcon(""),
    //        tr("Styles"),
    //        subitem,
    //        0,
    //        PAGE_HTML_STYLES_ID);

    //subitem2 = treeMenu->addItem(
    //                    QIcon(""),
    //        tr("Tabs"),
    //        subitem,
    //        0,
    //        PAGE_HTML_TABS_ID);



    subitem = treeMenu->addItem(
			QIcon(":/gui/icon32_latex.png"),
            tr("LaTeX"),
            item,
            iisTreeMenuCheckable,
            PAGE_LATEX_ID);

    bindCheckBox(subitem, "GENERATE_LATEX");


    subitem = treeMenu->addItem(
			QIcon(":/gui/icon32_rtf.png"),
            tr("RTF"),
            item,
            iisTreeMenuCheckable,
            PAGE_RTF_ID);

    bindCheckBox(subitem, "GENERATE_RTF");


    subitem = treeMenu->addItem(
			QIcon(":/gui/icon32_man.png"),
            tr("MAN"),
            item,
            iisTreeMenuCheckable,
            PAGE_MAN_ID);

    bindCheckBox(subitem, "GENERATE_MAN");


    subitem = treeMenu->addItem(
			QIcon(":/gui/icon32_xml.png"),
            tr("XML"),
            item,
            iisTreeMenuCheckable,
            PAGE_XML_ID);

    bindCheckBox(subitem, "GENERATE_XML");


    subitem = treeMenu->addItem(
			QIcon(":/gui/icon32_def.png"),
            tr("AutoGen"),
            item,
            iisTreeMenuCheckable,
            PAGE_DEF_ID);

    bindCheckBox(subitem, "GENERATE_AUTOGEN_DEF");


    subitem = treeMenu->addItem(
			QIcon(":/gui/icon32_pl.png"),
            tr("Perl module"),
            item,
            iisTreeMenuCheckable,
            PAGE_PERL_ID);

    bindCheckBox(subitem, "GENERATE_PERLMOD");


    connect(treeMenu, SIGNAL(selected(int)),
            this, SIGNAL(setup(int)));

    connect(treeMenu, SIGNAL(checked(QTreeWidgetItem *, Qt::CheckState)),
            this, SLOT(onChecked(QTreeWidgetItem *, Qt::CheckState)));

    reload();
}

MenuWidget::~MenuWidget()
{

}

void MenuWidget::setActivePage(int id)
{
    treeMenu->setCurrent(id);
}

void MenuWidget::bindCheckBox(QTreeWidgetItem *item, char *config)
{
    ConfigBool *cb = (ConfigBool*) Config::instance()->get(config);
    myCheckMap[item] = cb;
}

void MenuWidget::reload()
{
    QMap<QTreeWidgetItem*, ConfigBool*>::iterator it, it_end = myCheckMap.end();
    for (it = myCheckMap.begin(); it != it_end; it++) {
        QTreeWidgetItem *item = it.key();
        ConfigBool *cb = it.value();
        item->setCheckState(0, *(cb->valueRef()) ? Qt::Checked : Qt::Unchecked);
    }
}

void MenuWidget::onChecked(QTreeWidgetItem *item, Qt::CheckState check)
{
    ConfigBool *cb = myCheckMap[item];
    if (!cb)
        return;

    *(cb->valueRef()) = (check == Qt::Checked);

    emit checked(cb, check);
}

void MenuWidget::onOptionChecked(ConfigBool *cb, Qt::CheckState check)
{
    QTreeWidgetItem *item = myCheckMap.key(cb);
    if (item) {
        item->setCheckState(0, check);
    }
}
