/****************************************************************************
**
** Trolltech hereby grants a license to use the Qt/Eclipse Integration
** plug-in (the software contained herein), in binary form, solely for the
** purpose of creating code to be used with Trolltech's Qt software.
**
** Qt Designer is licensed under the terms of the GNU General Public
** License versions 2.0 and 3.0 ("GPL License"). Trolltech offers users the
** right to use certain no GPL licensed software under the terms of its GPL
** Exception version 1.2 (http://trolltech.com/products/qt/gplexception).
**
** THIS SOFTWARE IS PROVIDED BY TROLLTECH AND ITS CONTRIBUTORS (IF ANY) "AS
** IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
** TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
** PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
** OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
** EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
** PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
** PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
** LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** Since we now have the GPL exception I think that the "special exception
** is no longer needed. The license text proposed above (other than the
** special exception portion of it) is the BSD license and we have added
** the BSD license as a permissible license under the exception.
**
****************************************************************************/

#include "mainform.h"
#include "setupdialog.h"

#include "config.h"
#include "shared.h"

#include "setupbasepage.h"
#include "setupprojectpage.h"
#include "setupbuildpage.h"
#include "setupbuild2page.h"
#include "setupanalisyspage.h"
#include "setupmessagespage.h"
#include "setuphtmlpage.h"
#include "setuphtmlstylepage.h"
#include "setuplatexpage.h"
#include "setuprtfpage.h"
#include "setupmanpage.h"
#include "setupxmlpage.h"
#include "setupdefpage.h"
#include "setupperlpage.h"
#include "setupdotpage.h"

#include "iisStringListEdit.h"
#include "iisLineEdit.h"
#include "iisEnvironment.h"

const int MessageTimeout = 3000;
const int MaxRecentFiles = 15;

#define BUILD_STRING "1.2"
#define DOX_BUILD_STRING "1.5.7.1"
#define DOX_NEED_VERSION 0x01050701



#if defined(Q_OS_MACX)
#include <CoreFoundation/CFBundle.h>
QString getResourcePath()
{
  // todo: use qApp->applicationDirPath()
  QString result;
  CFURLRef pluginRef  = CFBundleCopyBundleURL(CFBundleGetMainBundle());
  CFStringRef macPath = CFURLCopyFileSystemPath(pluginRef, kCFURLPOSIXPathStyle);
  result = CFStringGetCStringPtr(macPath, CFStringGetSystemEncoding());
  result+="/Contents/Resources/";
  return result;
}

void setDotPath()
{
  Config_getString("DOT_PATH")=getResourcePath();
}

void setMscgenPath()
{
  Config_getString("MSCGEN_PATH")=getResourcePath();
}

#endif

MainForm* MainForm::self = 0;

MainForm::MainForm(QWidget *parent)
    : QMainWindow(parent),
	myCurrentSetupPage(0),
	myShrunkState(0),
	m_running(false)
{
	ui.setupUi(this);
	self = this;

	centralWidget()->layout()->setMargin(0);
	ui.stackedWidget->widget(0)->layout()->setMargin(1);
	//ui.stackedWidget->widget(1)->layout()->setMargin(1);

    QString version = tr("DoxyGate build <b>%1</b> <i>(Doxygen <b>%2</b> supported)</i>")
        .arg(BUILD_STRING, DOX_BUILD_STRING);
    ui.outputWidget->showMessage(version, MS_TEXT);
    QLabel *statusLabel = new QLabel(version, this);
    statusLabel->setFrameStyle(QFrame::NoFrame);
    statusBar()->addPermanentWidget(statusLabel);

	doStyle();

    // initialize config settings
    Config::instance()->init();
    Config::instance()->check();

    Config_getBool("HAVE_DOT")=true;
    #if defined(Q_OS_MACX)
      setDotPath();
      setMscgenPath();
    #endif

    connect(ui.menuWidget, SIGNAL(setup(int)), this, SLOT(setupPage(int)));

    connect(ui.topWidget, SIGNAL(newProject()), this, SLOT(newProject()));
    connect(ui.topWidget, SIGNAL(openProject()), this, SLOT(openProject()));
    connect(ui.topWidget, SIGNAL(openRecentProject(const QString &)),
            this, SLOT(openRecentProject(const QString &)));
    connect(ui.topWidget, SIGNAL(saveProject()), this, SLOT(saveProject()));
    connect(ui.topWidget, SIGNAL(saveProjectAs()), this, SLOT(saveProjectAs()));
    connect(ui.topWidget, SIGNAL(saveProjectCopy()), this, SLOT(saveProjectCopy()));

    connect(ui.topWidget, SIGNAL(switchConsole()), this, SLOT(switchConsole()));
    connect(ui.topWidget, SIGNAL(switchProject()), this, SLOT(switchProject()));

    connect(ui.topWidget, SIGNAL(run()), this, SLOT(runDoxygen()));
    connect(ui.topWidget, SIGNAL(setup()), this, SLOT(setup()));

    connect(ui.topWidget, SIGNAL(outputsUpdate()), this, SLOT(checkOutputs()));

    connect(this, SIGNAL(runMessage(const QString&,int)), ui.outputWidget, SLOT(showMessage(const QString&,int)));
    connect(this, SIGNAL(clearMessages()), ui.outputWidget, SLOT(clearMessages()));

    connect(ui.outputWidget, SIGNAL(abort()), this, SLOT(abortRun()));

    // process runner
    m_runProcess = new QProcess;
    connect(m_runProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(readStdout()));
    connect(m_runProcess,SIGNAL(readyReadStandardError()),this,SLOT(readStderr()));
    connect(m_runProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(runComplete(int,QProcess::ExitStatus)));
    connect(m_runProcess,SIGNAL(error(QProcess::ProcessError)),this,SLOT(runError(QProcess::ProcessError)));
    connect(m_runProcess,SIGNAL(started()), this, SLOT(runOk()));
    m_running = false;
    m_timer = new QTimer;
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeout()));

    // setup dialog
    setupDialog = new SetupDialog(this);
    connect(setupDialog, SIGNAL(applyTools()), this, SLOT(replaceDefaultSettingsTools()));
    connect(setupDialog, SIGNAL(applyMsg()), this, SLOT(replaceDefaultSettingsMsg()));


    QTimer::singleShot(0, this, SLOT(doInit()));
}

MainForm::~MainForm()
{

}

////////////////////////////////////////////////////////////////////////////////

void MainForm::doInit()
{
    // read settings
    readSettings();

    // init
    setProjectName("");

    if (toolSettings.defaultsForNewTools)
        replaceDefaultSettingsTools();
    if (msgSettings.defaultsForNewMsg)
        replaceDefaultSettingsMsg();

    showFirstPage();

    // check tools
    checkTools();
    checkDoxygenVersion();
}

void MainForm::doStyle()
{
    setStyle( QStyleFactory::create("macintosh") );
    setStyleSheet(stringFromResource(":/gui/style.qss"));

    iisStringListEdit::IconPickDir = QIcon(":/gui/icon16_dir.png");
    iisStringListEdit::IconPickFile = QIcon(":/gui/icon16_file.png");
    iisStringListEdit::IconAdd = QIcon(":/gui/icon16_add.png");
    iisStringListEdit::IconDelete = QIcon(":/gui/icon16_delete.png");

    //iisLineEdit::MinHeight = 24;
    iisLineEditDir::IconPickDir = QIcon(":/gui/icon16_dir.png");
    iisLineEditFile::IconPickFile = QIcon(":/gui/icon16_file.png");

    QApplication::setWindowIcon(QIcon(":/gui/icon32.png"));
}

////////////////////////////////////////////////////////////////////////////////

void MainForm::setupPage(int id)
{
    switch (id) {
        case PAGE_PPOJECT_ID:
            showSetupPage(new SetupProjectPage(this));
            break;
        case PAGE_EXTRACT_ID:
            showSetupPage(new SetupAnalisysPage(this));
            break;
        case PAGE_BUILD_ID:
            showSetupPage(new SetupBuildPage(this));
            break;
        case PAGE_BUILD2_ID:
            showSetupPage(new SetupBuild2Page(this));
            break;
        case PAGE_MESSAGES_ID:
            showSetupPage(new SetupMessagesPage(this));
            break;
        case PAGE_HTML_ID:
            showSetupPage(new SetupHTMLPage(this));
            break;
        case PAGE_HTML_STYLES_ID:
            showSetupPage(new SetupHTMLStylePage(this));
            break;
        case PAGE_LATEX_ID:
            showSetupPage(new SetupLatexPage(this));
            break;
        case PAGE_RTF_ID:
            showSetupPage(new SetupRTFPage(this));
            break;
        case PAGE_MAN_ID:
            showSetupPage(new SetupManPage(this));
            break;
        case PAGE_XML_ID:
            showSetupPage(new SetupXMLPage(this));
            break;
        case PAGE_DEF_ID:
            showSetupPage(new SetupDefPage(this));
            break;
        case PAGE_PERL_ID:
            showSetupPage(new SetupPerlPage(this));
            break;
        case PAGE_DOT_ID:
            showSetupPage(new SetupDotPage(this));
            break;
        default:;
    }
}

void MainForm::storeCurrentPage()
{
    if (ui.setupWidget->count()) {
        QWidget *w = ui.setupWidget->currentWidget();
        SetupBasePage *current = dynamic_cast<SetupBasePage*>(w);
        if (current)
            current->storeValues();
    }
}

void MainForm::showSetupPage(SetupBasePage *sb, bool storePrevious)
{
    if (ui.setupWidget->count()) {
        QWidget *w = ui.setupWidget->currentWidget();
        SetupBasePage *current = dynamic_cast<SetupBasePage*>(w);
        if (current && storePrevious)
            current->storeValues();

        ui.setupWidget->removeWidget(w);
        delete w;
    }

    ui.setupWidget->addWidget(sb);

    connect(ui.menuWidget, SIGNAL(checked(ConfigBool*, Qt::CheckState)),
         sb, SLOT(onOptionChecked(ConfigBool*, Qt::CheckState)));

    connect(sb, SIGNAL(optionChecked(ConfigBool*, Qt::CheckState)),
            ui.menuWidget, SLOT(onOptionChecked(ConfigBool*, Qt::CheckState)));

    myCurrentSetupPage = sb;

    myShrunkState = (height() < 800);
    if (myShrunkState)
            handleHeightShrunk(myShrunkState);
}

void MainForm::showFirstPage()
{
    ui.topWidget->switchToProject();
    ui.menuWidget->setActivePage(PAGE_PPOJECT_ID);

    showSetupPage(new SetupProjectPage(this), false);

    ui.menuWidget->reload();
}

void MainForm::handleHeightShrunk(int i)
{
	myShrunkState = i;
	if (myCurrentSetupPage) {
		myCurrentSetupPage->switchLayout(i);
	}
}

void MainForm::switchProject()
{
    ui.stackedWidget->setCurrentIndex(0);
}

void MainForm::switchConsole()
{
    ui.stackedWidget->setCurrentIndex(1);
}

////////////////////////////////////////////////////////////////////////////////

void MainForm::setProjectName(const QString &name)
{
    setWindowTitle(QString("DoxyGate - %1").arg(name.isEmpty() ? tr("New Project") : name));
    myProject = name;

    if (!name.isEmpty())
        addRecentFile(name);
}

bool MainForm::checkStored()
{
    storeCurrentPage();

//    if (myProject.isEmpty())
//        return true;

    //int i = QMessageBox::question(this,
    //        tr("Save Changes Before"),
    //        tr("Would you like to break and save latest changes?"),
    //        QMessageBox::Yes | QMessageBox::No,
    //        QMessageBox::No);

    //if (i == QMessageBox::Yes)
    //    return false;

    return true;
}

////////////////////////////////////////////////////////////////////////////////

void MainForm::newProject()
{
    if (!checkStored())
        return;

    Config::instance()->init();
    setProjectName("");

    if (toolSettings.defaultsForNewTools)
        replaceDefaultSettingsTools();
    if (msgSettings.defaultsForNewMsg)
        replaceDefaultSettingsMsg();

    showFirstPage();

    statusBar()->showMessage( tr("New configuration created"), MessageTimeout);
}

void MainForm::openProject()
{
    if (!checkStored())
        return;

    QString s = QFileDialog::getOpenFileName(this,
            tr("Open Doxygen Project"),
            myProject,
            tr("Doxygen projects (Doxyfile*);;All Files (*.*)")
            );
    if (s.isEmpty())
        return;

    loadConfigFromFile(s);
}

void MainForm::openRecentProject(const QString &filename)
{
    loadConfigFromFile(filename);
}

void MainForm::saveProject()
{
    if (myProject.isEmpty()) {
        saveProjectAs();
        return;
    }

    saveConfigToFile(myProject);
}

void MainForm::saveProjectAs()
{
    QString s = QFileDialog::getSaveFileName(this,
            tr("Save Doxygen Project"),
            myProject,
            tr("Doxygen projects (Doxyfile*);;All Files (*.*)")
            );
    if (s.isEmpty())
        return;

    if (saveConfigToFile(s)) {
        setProjectName(s);
    }
}

void MainForm::saveProjectCopy()
{
    QString s = QFileDialog::getSaveFileName(this,
            tr("Save Doxygen Project Copy"),
            myProject,
            tr("Doxygen projects (Doxyfile*);;All Files (*.*)")
            );
    if (s.isEmpty())
        return;

    saveConfigToFile(s);
}


bool MainForm::loadConfigFromFile(const QString &name)
{
  if (!Config::instance()->parse( CCHARP(name) ))
  {
    QMessageBox::critical(this, tr("Warning"),
        tr("Could not open or read config file %1.").arg(name),
        QMessageBox::Ok);
    return false;
  }
  else
  {
    Config::instance()->convertStrToVal();

#if 0 //defined(Q_OS_MACX)
    if (checkIfDotInstalled() &&
        qstricmp(Config_getString("DOT_PATH"),DOT_PATH)!=0
       )
    {
      Config_getString("DOT_PATH")=DOT_PATH;
      //setConfigSaved(false);
    }
    else
    {
      //setConfigSaved(true);
    }
#else
    //setConfigSaved(true);
#endif
    //addRecentFile(fn);
    //m_workingDir->setText(QFileInfo(fn).dirPath(true));

    setProjectName(name);

    if (toolSettings.defaultsForOpenTools)
        replaceDefaultSettingsTools();
    if (msgSettings.defaultsForOpenMsg)
        replaceDefaultSettingsMsg();

    showFirstPage();

#if defined(Q_OS_MACX)
    if (Config_getString("DOT_PATH").isEmpty())
    {
      setDotPath();
      //setConfigSaved(false);
    }
    if (Config_getString("MSCGEN_PATH").isEmpty())
    {
      setMscgenPath();
      //setConfigSaved(false);
    }
#endif
    statusBar()->showMessage(tr("New configuration loaded"), MessageTimeout);
  }

  return true;
}

bool MainForm::saveConfigToFile(const QString &name)
{
    storeCurrentPage();

    QFile f(name);
    if (f.open(QFile::WriteOnly))
    {
      QTextStream t(&f);
      Config::instance()->writeTemplate(t,true,false);
//      setConfigSaved(true);
//      addRecentFile(fn);
      statusBar()->showMessage(tr("Configuration saved"), MessageTimeout);
      f.close();
      return true;
    }

    QMessageBox::warning(0, tr("Warning"),
          tr("Cannot open file %1 for writing.").arg(name),
          QMessageBox::Ok);

    return false;
}


void MainForm::addRecentFile(const QString &name)
{
    int i = myRecent.indexOf(name);
    if (i == 0)
        return; // added already and at the top

    if (i > 0) {
        myRecent.move(i, 0);
    } else {
        myRecent.prepend(name);
        if (myRecent.count() >= MaxRecentFiles)
            myRecent.takeLast();
    }

    ui.topWidget->updateRecentList(myRecent);
}

////////////////////////////////////////////////////////////////////////////////

void MainForm::runDoxygen()
{
    if ( !m_running )
    {
        if ( myProject.isEmpty() )
        {
            QMessageBox::warning ( this, tr ( "Project not saved" ),
                                   tr ( "Please save the project before run." ),
                                   QMessageBox::Ok );
            return;
        }

        saveProject();
        ui.topWidget->switchToConsole();

        QString doxygenPath, workPath;
        QStringList args;

#if defined(Q_OS_MACX)
        doxygenPath = getResourcePath();
#else
        // TODO: get the right value if not in the search path
#endif

        doxygenPath += ( QString ) toolSettings.doxPath;
        args << "-b" << myProject;

        QFileInfo fi ( myProject );
        workPath = fi.absolutePath();

        emit clearMessages();
        emit runMessage ( QString ( "Starting <i>%1</i> in <i>%2</i>...\n" ).arg ( doxygenPath, workPath ), MS_COMMAND );

        m_runProcess->setWorkingDirectory ( workPath );
        m_runProcess->start ( doxygenPath, args );
    }
    else
    {
        setRunning ( false );
        m_runProcess->kill();
    }
}

void MainForm::readStdout()
{
    emit runMessage(m_runProcess->readAllStandardOutput(), MS_TEXT);
}

void MainForm::readStderr()
{
    QStringList tx = QString(m_runProcess->readAllStandardError()).split("\n");

    foreach(QString txt, tx) {
        int dx = MS_TEXT;
        if (txt.contains("Warning:"))
            dx = MS_WARNING;
        else
        if (txt.contains("Error:"))
            dx = MS_ERROR;

        emit runMessage(txt, dx);
    }
}

void MainForm::runComplete(int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/)
{
    if (m_running)
    {
        emit runMessage(QString("Doxygen has finished.\n"), MS_OK);
    }
    else
    {
        emit runMessage(QString("Cancelled by user.\n"), MS_ERROR);
    }

    statusBar()->showMessage(tr("Build finished"), MessageTimeout);
    setRunning(false);
}

void MainForm::runError(QProcess::ProcessError /*err*/)
{
    emit runMessage(tr("Error while running build.\n"), MS_ERROR);
    statusBar()->showMessage(tr("Build error"), MessageTimeout);

    setRunning(false);
}

void MainForm::runOk()
{
    setRunning(true);

    mySec = 0;
    m_timer->start(1000);
    timeout();

    emit runMessage(tr("Doxygen started OK...\n"), MS_OK);
    statusBar()->showMessage(tr("Build started"), MessageTimeout);
}

void MainForm::abortRun()
{
    if (m_running) {
        emit runMessage(tr("Cancel requested.\n"), MS_COMMAND);
        m_runProcess->kill();
    }
}

void MainForm::setRunning(bool b)
{
    m_running = b;
    ui.topWidget->setEnabled(!b);
    ui.outputWidget->setRunning(b);
    if (!b)
        m_timer->stop();
}

void MainForm::timeout()
{
    ui.outputWidget->setTime(mySec);
    mySec += m_timer->interval();
}

////////////////////////////////////////////////////////////////////////////////

void MainForm::resizeEvent ( QResizeEvent * event )
{
    int state = (event->size().height() < 800);
    if (state != myShrunkState) {
        myShrunkState = state;
        handleHeightShrunk(myShrunkState);
    }
}


void MainForm::closeEvent ( QCloseEvent * event )
{
    if (checkStored()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainForm::setup()
{
//    SetupDialog sd(this);
//
//    connect(&sd, SIGNAL(applyTools()), this, SLOT(replaceDefaultSettingsTools()));
//    connect(&sd, SIGNAL(applyMsg()), this, SLOT(replaceDefaultSettingsMsg()));
//
//    int i = sd.exec();

    int i = setupDialog->exec();

    if (i == QDialogButtonBox::ApplyRole) {
        replaceDefaultSettingsTools();
        replaceDefaultSettingsMsg();
    }
}

void MainForm::checkTools()
{
    QStringList sl;
    sl << toolSettings.doxPath;
    sl << toolSettings.dotPath;
    sl << toolSettings.perlPath;
    sl << toolSettings.latexPath;
    sl << toolSettings.makeindexPath;
    sl << toolSettings.mscgenPath;
    sl << toolSettings.hhcPath;
	sl << toolSettings.qhpPath;

    emit runMessage(tr("\nChecking tools..."), MS_COMMAND);

    foreach (QString s, sl) {
        QStringList files = iisEnvironment::availableInPath(s);
        if (!files.count()) {
            emit runMessage(tr("Tool not available: %1").arg(s), MS_ERROR);
		} else {
			QString fname = QFileInfo(s).fileName();
            emit runMessage(tr("Tool available: %1 (found at %2)").arg(fname, files.first()), MS_OK);
		}
    }
}


void MainForm::checkDoxygenVersion()
{
    emit runMessage(tr("\nChecking Doxygen version..."), MS_COMMAND);
    
    QString ver = toolSettings.getDoxygenVersion();
    if (ver.isEmpty()) {
        emit runMessage(tr("Impossible to obtain Doxygen version. Make sure that version %1 installed and correct path is set in Setup dialog.").arg(DOX_BUILD_STRING), MS_ERROR);
        return;
    }

    QStringList ver_sl = ver.split(".", QString::SkipEmptyParts);
    int ver_int = 0;
    foreach (QString s, ver_sl) {
        bool ok;
        int i = s.toUInt(&ok);
        if (ok)
            ver_int = (ver_int << 8) + i;
    }
    for(int i=ver_sl.size(); i<4; i++)
    {
        ver_int = (ver_int << 8);
    }

    //qDebug() << ver_int << DOX_NEED_VERSION;

    if (ver_int == DOX_NEED_VERSION) {
        emit runMessage(tr("Correct Doxygen version installed (%1).").arg(DOX_BUILD_STRING), MS_OK);
        return;
    }

    if (ver_int > DOX_NEED_VERSION) {
        emit runMessage(tr("Doxygen version %1 required, but %2 found which is newer than required. There is no warranty that DoxyGate will work properly. Consider upgrading DoxyGate to the most recent version.").arg(DOX_BUILD_STRING).arg(ver), MS_WARNING);
    } else {
        emit runMessage(tr("Doxygen version %1 required, but %2 found which is too old. There is no warranty that DoxyGate will work properly. Consider upgrading Doxygen version to the required one.").arg(DOX_BUILD_STRING).arg(ver), MS_WARNING);
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainForm::readSettings()
{
    //QString path = QApplication::applicationDirPath() + "/settings.ini";
    //QSettings set(path, QSettings::IniFormat);
    QSettings set(QSettings::IniFormat, QSettings::UserScope, "doxygate", "doxygate");

    myRecent = set.value("RecentFiles").toStringList();
    ui.topWidget->updateRecentList(myRecent);

    set.beginGroup("Tools");
    toolSettings.restore(set);
    set.endGroup();

    set.beginGroup("Messages");
    msgSettings.restore(set);
    set.endGroup();

    restoreState(set.value("WindowState").toByteArray());
    //restoreGeometry(set.value("WindowGeometry").toByteArray());
    QPoint pos = set.value("WindowPos", QPoint(0, 0)).toPoint();
    QSize size = set.value("WindowSize", geometry().size()).toSize();
    bool max = set.value("WindowMaximized", true).toBool();
    move(pos);
    if (max)
        showMaximized();
    else
        resize(size);
}

void MainForm::writeSettings()
{
    //QString path = QApplication::applicationDirPath() + "/settings.ini";
    //QSettings set(path, QSettings::IniFormat);
    QSettings set(QSettings::IniFormat, QSettings::UserScope, "doxygate", "doxygate");

    set.setValue("RecentFiles", myRecent);

    set.beginGroup("Tools");
    toolSettings.store(set);
    set.endGroup();

    set.beginGroup("Messages");
    msgSettings.store(set);
    set.endGroup();


    set.setValue("WindowState", saveState());
    //set.setValue("WindowGeometry", saveGeometry());
    set.setValue("WindowPos", pos());
    set.setValue("WindowSize", size());
    set.setValue("WindowMaximized", isMaximized());
}

void MainForm::replaceDefaultSettingsTools()
{
    setStringValue("PERL_PATH", toolSettings.perlPath);
    setStringValue("MSCGEN_PATH", toolSettings.mscgenPath);
    setStringValue("DOT_PATH", toolSettings.dotPath);
    setStringValue("LATEX_CMD_NAME", toolSettings.latexPath);
    setStringValue("MAKEINDEX_CMD_NAME", toolSettings.makeindexPath);
    setStringValue("HHC_LOCATION", toolSettings.hhcPath);
	setStringValue("QHP_LOCATION", toolSettings.qhpPath);

    if (myCurrentSetupPage) {
        myCurrentSetupPage->reloadWidget("PERL_PATH");
        myCurrentSetupPage->reloadWidget("MSCGEN_PATH");
        myCurrentSetupPage->reloadWidget("DOT_PATH");
        myCurrentSetupPage->reloadWidget("LATEX_CMD_NAME");
        myCurrentSetupPage->reloadWidget("MAKEINDEX_CMD_NAME");
        myCurrentSetupPage->reloadWidget("HHC_LOCATION");
		myCurrentSetupPage->reloadWidget("QHP_LOCATION");
    }
}

void MainForm::replaceDefaultSettingsMsg()
{
    setBoolValue("QUIET", msgSettings.quiet);
    setBoolValue("WARNINGS", msgSettings.warnings);
    setBoolValue("WARN_IF_UNDOCUMENTED", msgSettings.warnUndocMember);
    setBoolValue("WARN_IF_DOC_ERROR", msgSettings.warnError);
    setBoolValue("WARN_NO_PARAMDOC", msgSettings.warnUndocParam);
    setStringValue("WARN_FORMAT", msgSettings.warnFormat);
    setStringValue("WARN_LOGFILE", msgSettings.warnLogFile);

    if (myCurrentSetupPage) {
        myCurrentSetupPage->reloadWidget("QUIET");
        myCurrentSetupPage->reloadWidget("WARNINGS");
        myCurrentSetupPage->reloadWidget("WARN_IF_UNDOCUMENTED");
        myCurrentSetupPage->reloadWidget("WARN_IF_DOC_ERROR");
        myCurrentSetupPage->reloadWidget("WARN_NO_PARAMDOC");
        myCurrentSetupPage->reloadWidget("WARN_FORMAT");
        myCurrentSetupPage->reloadWidget("WARN_LOGFILE");
    }
}

////////////////////////////////////////////////////////////////////////////////

void MainForm::on_topWidget_about()
{
    if (ui.tbAbout->toPlainText().isEmpty())
    {
        QString s = stringFromResource(":/gui/about.htm");

        s.replace("%BUILD_STRING", BUILD_STRING);
        s.replace("%DOX_BUILD_STRING", DOX_BUILD_STRING);

        ui.tbAbout->setText(s);
    }

    ui.stackedWidget->setCurrentIndex(2);
}

////////////////////////////////////////////////////////////////////////////////

QString MainForm::stringFromResource(const QString &resName)
{
    QFile file(resName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream ts(&file);
    return ts.readAll();
}

////////////////////////////////////////////////////////////////////////////////

QString MainForm::outputDir()
{
    QString outDir = Config_getString("OUTPUT_DIRECTORY");
    if (!QDir::isAbsolutePath(outDir)) {
        QFileInfo fi(myProject);
        outDir = fi.absolutePath() + "/" + outDir;
    }
    return QDir::cleanPath(outDir);
}

QString MainForm::htmlDir()
{
    QString outDir = Config_getString("HTML_OUTPUT");
    if (outDir.isEmpty())
        outDir = "html";
    if (!QDir::isAbsolutePath(outDir)) {
        outDir = outputDir() + "/" + outDir; // it is absolute
    }
    return QDir::cleanPath(outDir);
}

QString MainForm::qhpDir() 
{
    QString outDir = "qch";
    if (!QDir::isAbsolutePath(outDir)) {
        outDir = outputDir() + "/" + outDir; // it is absolute
    }
    return QDir::cleanPath(outDir);
}

QString MainForm::xmlDir()
{
    QString outDir = Config_getString("XML_OUTPUT");
    if (outDir.isEmpty())
        outDir = "xml";
    if (!QDir::isAbsolutePath(outDir)) {
        outDir = outputDir() + "/" + outDir; // it is absolute
    }
    return QDir::cleanPath(outDir);
}

QString MainForm::rtfDir()
{
    QString outDir = Config_getString("RTF_OUTPUT");
    if (outDir.isEmpty())
        outDir = "rtf";
    if (!QDir::isAbsolutePath(outDir)) {
        outDir = outputDir() + "/" + outDir; // it is absolute
    }
    return QDir::cleanPath(outDir);
}

QString MainForm::defDir()
{
    QString outDir = "def";
    if (!QDir::isAbsolutePath(outDir)) {
        outDir = outputDir() + "/" + outDir; // it is absolute
    }
    return QDir::cleanPath(outDir);
}

QString MainForm::perlmodDir()
{
    QString outDir = "perlmod";
    if (!QDir::isAbsolutePath(outDir)) {
        outDir = outputDir() + "/" + outDir; // it is absolute
    }
    return QDir::cleanPath(outDir);
}

QString MainForm::manDir()
{
    QString outDir = Config_getString("MAN_OUTPUT");
    if (outDir.isEmpty())
        outDir = "man";
    if (!QDir::isAbsolutePath(outDir)) {
        outDir = outputDir() + "/" + outDir; // it is absolute
    }
    return QDir::cleanPath(outDir);
}

QString MainForm::latexDir()
{
    QString outDir = Config_getString("LATEX_OUTPUT");
    if (outDir.isEmpty())
        outDir = "latex";
    if (!QDir::isAbsolutePath(outDir)) {
        outDir = outputDir() + "/" + outDir; // it is absolute
    }
    return QDir::cleanPath(outDir);
}




QString MainForm::htmlFile()
{
    QString outDir = htmlDir();

    QString ext = Config_getString("HTML_FILE_EXTENSION");
    if (ext.isEmpty())
        outDir += "/index.html";
    else
        outDir += "/index" + ext;

    return outDir;
}

QString MainForm::chmFile()
{
    QString outDir = Config_getString("CHM_FILE");
    if (outDir.isEmpty())
        outDir = "index.chm";

    if (!QDir::isAbsolutePath(outDir)) {
        outDir = htmlDir() + "/" + outDir; // it is absolute
    }

    return outDir;
}

QString MainForm::qhpFile()
{
    QString outDir = Config_getString("QCH_FILE");
    if (outDir.isEmpty())
        outDir = "output.qch";

    if (!QDir::isAbsolutePath(outDir)) {
        outDir = qhpDir() + "/" + outDir; // it is absolute
    }

    return outDir;
}

QString MainForm::xmlFile()
{
    return xmlDir() + "/index.xml";
}

QString MainForm::rtfFile()
{
    return rtfDir() + "/refman.rtf";
}

QString MainForm::defFile()
{
    return defDir() + "/doxygen.def";
}

QString MainForm::manFile()
{
    QString outDir = manDir();

    QString ext = Config_getString("MAN_EXTENSION");
    if (ext.isEmpty())
        outDir += "/man3";
    else
        outDir += "/man" + ext.right(ext.length()-1);

    return outDir;
}

////////////////////////////////////////////////////////////////////////////////

void MainForm::on_topWidget_showDocDir()
{
    iisEnvironment::openFileDir(outputDir());
}

void MainForm::on_topWidget_showHTML()
{
    iisEnvironment::openFileDir(htmlFile());
}

void MainForm::on_topWidget_showCHM()
{
    iisEnvironment::openFileDir(chmFile());
}

void MainForm::on_topWidget_showQCH()
{
	iisEnvironment::openFileDir(qhpDir());
}

void MainForm::on_topWidget_showXML()
{
    iisEnvironment::openFileDir(xmlFile());
}

void MainForm::on_topWidget_showRTF()
{
    iisEnvironment::openFileDir(rtfFile());
}

void MainForm::on_topWidget_showDEF()
{
    iisEnvironment::openFileDir(defFile());
}

void MainForm::on_topWidget_showPerl()
{
    QString outDir = perlmodDir();

    iisEnvironment::openFileDir(outDir);
}

void MainForm::on_topWidget_showMAN()
{
    iisEnvironment::openFileDir(manFile());
}

void MainForm::on_topWidget_showLatex()
{
    QString outDir = latexDir();

    iisEnvironment::openFileDir(outDir);
}

void MainForm::checkOutputs()
{
    // check common directory
    outputs.commonOutput.PATH_DIR = outputDir();

    outputs.htmlOutput.PATH_DIR = htmlDir();
    outputs.htmlOutput.PATH_FILE = htmlFile();

    outputs.chmOutput.PATH_DIR = htmlDir();   // sure, it's ok
    outputs.chmOutput.PATH_FILE = chmFile();

    outputs.qhpOutput.PATH_DIR = qhpDir(); 
    outputs.qhpOutput.PATH_FILE = qhpFile();

    outputs.rtfOutput.PATH_DIR = rtfDir();
    outputs.rtfOutput.PATH_FILE = rtfFile();

    outputs.latexOutput.PATH_DIR = latexDir();

    outputs.xmlOutput.PATH_DIR = xmlDir();
    outputs.xmlOutput.PATH_FILE = xmlFile();

    outputs.perlOutput.PATH_DIR = perlmodDir();

    outputs.manOutput.PATH_DIR = manDir();
    //outputs.manOutput.PATH_FILE = manFile();

    outputs.defOutput.PATH_DIR = defDir();
    outputs.defOutput.PATH_FILE = defFile();

    // check accessibility
    outputs.checkPath();
}

////////////////////////////////////////////////////////////////////////////////

