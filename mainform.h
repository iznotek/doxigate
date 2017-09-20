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

#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include "ui_mainform.h"

class SetupBasePage;
class SetupDialog;

class MainForm : public QMainWindow
{
    Q_OBJECT

public:
    MainForm(QWidget *parent = 0);
    virtual ~MainForm();

    QString outputDir();
    QString htmlDir();
	QString qhpDir();
    QString xmlDir();
    QString rtfDir();
    QString manDir();
    QString defDir();
    QString perlmodDir();
    QString latexDir();

    QString htmlFile();
    QString chmFile();
	QString qhpFile();
    QString xmlFile();
    QString manFile();
    QString rtfFile();
    QString defFile();

    static MainForm *instance() { return self; }

    static QString stringFromResource(const QString &resName);

public slots:
    void setupPage(int pageID);

    void switchProject();
    void switchConsole();

    void newProject();
    void openProject();
    void openRecentProject(const QString &filename);
    void saveProject();
    void saveProjectAs();
    void saveProjectCopy();

    void setup();
    void checkTools();
    void checkOutputs();

    void on_topWidget_about();

    void on_topWidget_showDocDir();
    void on_topWidget_showHTML();
    void on_topWidget_showCHM();
	void on_topWidget_showQCH();
    void on_topWidget_showXML();
    void on_topWidget_showRTF();
    void on_topWidget_showMAN();
    void on_topWidget_showDEF();
    void on_topWidget_showPerl();
    void on_topWidget_showLatex();

signals:
	void heightShrunk(int);
	void runMessage(const QString &text, int type);
	void clearMessages();

protected:
    void showFirstPage();
    void showSetupPage(SetupBasePage *sb, bool storePrevious = true);
	void handleHeightShrunk(int i);
    void storeCurrentPage();
    bool checkStored();

	void setProjectName(const QString &name);
	bool loadConfigFromFile(const QString &name);
	bool saveConfigToFile(const QString &name);
	void addRecentFile(const QString &name);

	void readSettings();
	void writeSettings();

	void doStyle();

	void setRunning(bool);

    void checkDoxygenVersion();

	virtual void resizeEvent ( QResizeEvent * event );
	virtual void closeEvent ( QCloseEvent * event );

protected slots:
    void runDoxygen();
    void readStdout();
    void readStderr();
    void runComplete(int exitCode, QProcess::ExitStatus exitStatus);
    void runError(QProcess::ProcessError);
    void runOk();
    void abortRun();
    void timeout();

    void doInit();

    void replaceDefaultSettingsTools();
    void replaceDefaultSettingsMsg();

private:
    Ui::MainFormClass ui;

    static MainForm *self;

	SetupBasePage *myCurrentSetupPage;
	int myShrunkState;

	QString myProject;
	QStringList myRecent;

	bool m_running;
	QProcess *m_runProcess;
	QTimer *m_timer;
	int mySec;

	SetupDialog *setupDialog;
};

#endif // MAINFORM_H
