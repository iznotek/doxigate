/*
 * topwidget.cpp
 *
 *  Created on: 18.07.2008

 */

#include "topwidget.h"
#include "shared.h"

#include "iisToolButton.h"

TopWidget::TopWidget(QWidget *parent) : QWidget(parent)
{
    iisToolButton *newButton = new iisToolButton(QIcon(":/gui/icon24_new.png"), tr("New"), this, 24);
    newButton->setToolTip(tr("Create new doxygen project"));
    newButton->setStatusTip(newButton->toolTip());
    newButton->setGeometry(200,50,90,30);
    connect(newButton, SIGNAL(clicked()), this, SIGNAL(newProject()));

    openButton = new iisToolButton(QIcon(":/gui/icon24_open.png"), tr("Open"), this, 24);
    openButton->setToolTip(tr("Open doxygen project"));
    openButton->setStatusTip(openButton->toolTip());
    openButton->setGeometry(290,50,90,30);
    connect(openButton, SIGNAL(clicked()), this, SIGNAL(openProject()));
    openMenu = new QMenu(this);
    connect(openMenu, SIGNAL(triggered(QAction*)), this, SLOT(openMenuTriggered(QAction*)));


    iisToolButton *saveButton = new iisToolButton(QIcon(":/gui/icon24_save.png"), tr("Save"), this, 24);
    saveButton->setToolTip(tr("Save current project"));
    saveButton->setStatusTip(saveButton->toolTip());
    saveButton->setGeometry(380,50,90,30);
    QMenu *saveMenu = new QMenu(this);
    saveButton->setMenu(saveMenu);
    saveButton->setPopupMode(QToolButton::MenuButtonPopup);
    connect(saveButton, SIGNAL(clicked()), this, SIGNAL(saveProject()));

    QAction *a;
    a = saveMenu->addAction(QIcon(":/images/saveas.png"), tr("Save As..."), this, SIGNAL(saveProjectAs()));
    a->setStatusTip(tr("Save current project under new name"));
    a = saveMenu->addAction(QIcon(":/images/savecopy.png"), tr("Save Copy..."), this, SIGNAL(saveProjectCopy()));
    a->setStatusTip(tr("Save a copy of the current project under new name"));


    iisToolButton *setupButton = new iisToolButton(QIcon(":/gui/icon24_setup.png"), tr("Setup"), this, 24);
    setupButton->setToolTip(tr("Configure tools and other options"));
    setupButton->setStatusTip(setupButton->toolTip());
    setupButton->setGeometry(470,50,90,30);
    connect(setupButton, SIGNAL(clicked()), this, SIGNAL(setup()));


    projectButton = new iisToolButton(QIcon(":/gui/icon24_project.png"), tr("Project"), this, 24);
    projectButton->setToolTip(tr("Change project settings"));
    projectButton->setStatusTip(projectButton->toolTip());
    projectButton->setGeometry(560,50,90,30);
    projectButton->setCheckable(true);
    projectButton->setAutoExclusive(true);
    projectButton->setProperty("style","style2");
    projectButton->setChecked(true);
    connect(projectButton, SIGNAL(clicked()), this, SIGNAL(switchProject()));


    consoleButton = new iisToolButton(QIcon(":/gui/icon24_output.png"), tr("Output"), this, 24);
    consoleButton->setToolTip(tr("Open output window"));
    consoleButton->setStatusTip(consoleButton->toolTip());
    consoleButton->setGeometry(650,50,100,30);
    consoleButton->setCheckable(true);
    consoleButton->setAutoExclusive(true);
    consoleButton->setProperty("style","style2");
    connect(consoleButton, SIGNAL(clicked()), this, SIGNAL(switchConsole()));

    outputMenu = new QMenu(this);
    consoleButton->setMenu(outputMenu);
    consoleButton->setPopupMode(QToolButton::MenuButtonPopup);
    connect(outputMenu, SIGNAL(aboutToShow()), this, SLOT(outputMenuShow()));
    connect(outputMenu, SIGNAL(hovered(QAction*)), this, SLOT(outputMenuHovered(QAction*)));


    iisToolButton *runButton = new iisToolButton(QIcon(":/gui/icon24_run.png"), tr("Run!"), this, 24);
    runButton->setToolTip(tr("Save current project and start doxygen"));
    runButton->setStatusTip(runButton->toolTip());
    runButton->setGeometry(750,50,90,30);
    runButton->setProperty("style","style1");
    connect(runButton, SIGNAL(clicked()), this, SIGNAL(run()));


    iisToolButton *aboutButton = new iisToolButton(QIcon(":/gui/icon24_about.png"), tr("About"), this, 24);
    aboutButton->setToolTip(tr("Information about DoxyGate"));
    aboutButton->setStatusTip(aboutButton->toolTip());
    aboutButton->setGeometry(830,50,90,30);
    aboutButton->setCheckable(true);
    aboutButton->setAutoExclusive(true);
    aboutButton->setProperty("style","style2");
    connect(aboutButton, SIGNAL(clicked()), this, SIGNAL(about()));
}

TopWidget::~TopWidget()
{
    // TODO Auto-generated destructor stub
}

void TopWidget::switchToProject()
{
    projectButton->click();
}

void TopWidget::switchToConsole()
{
    consoleButton->click();
}

void TopWidget::updateRecentList(const QStringList &recent)
{
    openMenu->clear();

    if (recent.isEmpty()) {
        openButton->setMenu(0);
        return;
    }

    openButton->setMenu(openMenu);
    openButton->setPopupMode(QToolButton::MenuButtonPopup);

    foreach (QString s, recent) {
        openMenu->addAction(s);
    }
}

void TopWidget::openMenuTriggered(QAction *a)
{
    emit openRecentProject(a->text());
}

void TopWidget::outputMenuShow()
{
    // request update of outputs
    emit outputsUpdate();

    outputMenu->clear();
    QAction *a;

    a = outputMenu->addAction(tr("Show common output directory..."), this, SIGNAL(showDocDir()));
    a->setEnabled(outputs.commonOutput.PATH_DIR_EXISTS);
    a->setToolTip(outputs.commonOutput.PATH_DIR);
    a->setStatusTip(outputs.commonOutput.PATH_DIR);

    outputMenu->addSeparator();

    a = outputMenu->addAction(QIcon(":/gui/icon16_html.png"), tr("Show HTML output..."), this, SIGNAL(showHTML()));
    a->setEnabled(outputs.htmlOutput.PATH_FILE_EXISTS);
    a->setToolTip(outputs.htmlOutput.PATH_FILE);
    a->setStatusTip(outputs.htmlOutput.PATH_FILE);

    a = outputMenu->addAction(QIcon(":/gui/icon16_chm.png"), tr("Show CHM output..."), this, SIGNAL(showCHM()));
    a->setEnabled(outputs.chmOutput.PATH_FILE_EXISTS);
    a->setToolTip(outputs.chmOutput.PATH_FILE);
    a->setStatusTip(outputs.chmOutput.PATH_FILE);

    a = outputMenu->addAction(QIcon(":/gui/icon16_qhp.png"), tr("Show QCH output..."), this, SIGNAL(showQCH()));
    a->setEnabled(outputs.qhpOutput.PATH_DIR_EXISTS);
    a->setToolTip(outputs.qhpOutput.PATH_DIR);
    a->setStatusTip(outputs.qhpOutput.PATH_DIR);

    a = outputMenu->addAction(QIcon(":/gui/icon16_xml.png"), tr("Show XML output..."), this, SIGNAL(showXML()));
    a->setEnabled(outputs.xmlOutput.PATH_FILE_EXISTS);
    a->setToolTip(outputs.xmlOutput.PATH_FILE);
    a->setStatusTip(outputs.xmlOutput.PATH_FILE);

	a = outputMenu->addAction(QIcon(":/gui/icon16_rtf.png"), tr("Show RTF output..."), this, SIGNAL(showRTF()));
    a->setEnabled(outputs.rtfOutput.PATH_FILE_EXISTS);
    a->setToolTip(outputs.rtfOutput.PATH_FILE);
    a->setStatusTip(outputs.rtfOutput.PATH_FILE);

    a = outputMenu->addAction(QIcon(":/gui/icon16_latex.png"), tr("Show LaTeX output..."), this, SIGNAL(showLatex()));
    a->setEnabled(outputs.latexOutput.PATH_DIR_EXISTS);
    a->setToolTip(outputs.latexOutput.PATH_DIR);
    a->setStatusTip(outputs.latexOutput.PATH_DIR);

    a = outputMenu->addAction(QIcon(":/gui/icon16_pl.png"), tr("Show Perl Module output..."), this, SIGNAL(showPerl()));
    a->setEnabled(outputs.perlOutput.PATH_DIR_EXISTS);
    a->setToolTip(outputs.perlOutput.PATH_DIR);
    a->setStatusTip(outputs.perlOutput.PATH_DIR);

    a = outputMenu->addAction(QIcon(":/gui/icon16_man.png"), tr("Show MAN output..."), this, SIGNAL(showMAN()));
    a->setEnabled(outputs.manOutput.PATH_DIR_EXISTS);
    a->setToolTip(outputs.manOutput.PATH_DIR);
    a->setStatusTip(outputs.manOutput.PATH_DIR);

    a = outputMenu->addAction(QIcon(":/gui/icon16_def.png"), tr("Show Autogen DEF output..."), this, SIGNAL(showDEF()));
    a->setEnabled(outputs.defOutput.PATH_FILE_EXISTS);
    a->setToolTip(outputs.defOutput.PATH_FILE);
    a->setStatusTip(outputs.defOutput.PATH_FILE);
}

void TopWidget::outputMenuHovered(QAction *a)
{
    if (!a->toolTip().isEmpty()) {
        QPoint pos = outputMenu->mapToGlobal(outputMenu->rect().bottomLeft());
        QToolTip::showText(pos, tr("<b>Path to open:</b> %1").arg(a->toolTip()));
    }
}

void TopWidget::paintEvent ( QPaintEvent * /*event*/ )
{
	QPainter p(this);
	p.fillRect(rect(), QBrush(qApp->palette().window() ));

	QLinearGradient headerBackgroundGrd(0,0, 0,80);
	//headerBackgroundGrd.setColorAt(0, 0x679993);
	//headerBackgroundGrd.setColorAt(0.3, 0x377a70);
	//headerBackgroundGrd.setColorAt(0.31, 0x0e5e53);
	//headerBackgroundGrd.setColorAt(1, 0x4b8578);
	headerBackgroundGrd.setColorAt(0, 0x6793a9);
	headerBackgroundGrd.setColorAt(0.3, 0x37708a);
	headerBackgroundGrd.setColorAt(0.31, 0x0e536e);
	headerBackgroundGrd.setColorAt(1, 0x4b7895);

	p.fillRect(QRect(0,0,width(),80), QBrush(headerBackgroundGrd));

	p.drawPixmap(0,0,QPixmap(":/gui/header_top_left.png"));
}

