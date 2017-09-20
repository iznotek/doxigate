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

#include "setupdialog.h"
#include "shared.h"

#include "iisEnvironment.h"

enum OPTION_DATA { STATE = 0, LABEL, LINEEDIT, CHANGED };

SetupDialog::SetupDialog(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	ui.tabWidget->setCurrentIndex(0);

	// tools

	ui.doxygenDir->setPickTitle(tr("Doxygen Executable"));
    ui.perlDir->setPickTitle(tr("Perl Executable"));
    ui.latexDir->setPickTitle(tr("LaTeX Executable"));
    ui.makeindexDir->setPickTitle(tr("MakeIndex Executable"));
    ui.dotDir->setPickTitle(tr("Directory of Dot Executable"));
    ui.mscgenDir->setPickTitle(tr("Directory of MSCGEN Executable"));
    ui.hhcDir->setPickTitle(tr("HTML Help Compiler Executable"));
	ui.qhpDir->setPickTitle(tr("Qt Help Generator Executable"));

	toolSettings.defaultsForNewTools.connectWidget(ui.cbDefToolsNew);
	toolSettings.defaultsForOpenTools.connectWidget(ui.cbDefToolsOpen);

    toolSettings.doxPath.connectWidget(ui.doxygenDir->editor());
    toolSettings.perlPath.connectWidget(ui.perlDir->editor());
    toolSettings.latexPath.connectWidget(ui.latexDir->editor());
    toolSettings.makeindexPath.connectWidget(ui.makeindexDir->editor());
    toolSettings.dotPath.connectWidget(ui.dotDir->editor());
    toolSettings.mscgenPath.connectWidget(ui.mscgenDir->editor());
    toolSettings.hhcPath.connectWidget(ui.hhcDir->editor());
	toolSettings.qhpPath.connectWidget(ui.qhpDir->editor());

    toolList = toolSettings.options(ToolOptionID);
    foreach (Option* op, toolList) {
        iisLineEdit *le = (iisLineEdit*) op->widget->parentWidget();
        op->data[LINEEDIT] = (quintptr)le;
        connect(le, SIGNAL(buttonClickedOk(QWidget*)), this, SLOT(toolChanged(QWidget*)));
        connect(le, SIGNAL(textEdited(QWidget*, const QString &)), this, SLOT(toolChanged(QWidget*, const QString &)));
        //connect(le, SIGNAL(lostFocus(QWidget*)), this, SLOT(toolLeft(QWidget*)));

        QLabel *label = new QLabel(le);
        label->setFixedWidth(20);
        label->setAlignment(Qt::AlignRight | Qt::AlignCenter);
        le->layout()->addWidget(label);
        op->data[LABEL] = (quintptr)label;

        op->data[STATE] = TOS_UNKNOWN;
        //op->data[CHANGED] = false;
    }

    // messages
    msgSettings.defaultsForNewMsg.connectWidget(ui.cbDefMsgNew);
    msgSettings.defaultsForOpenMsg.connectWidget(ui.cbDefMsgOpen);

    msgSettings.quiet.connectWidget(ui.cbQuietMode);
    msgSettings.warnings.connectWidget(ui.cbWarnings);
    msgSettings.warnError.connectWidget(ui.cbWarnErrors);
    msgSettings.warnUndocMember.connectWidget(ui.cbWarnUndocMember);
    msgSettings.warnUndocParam.connectWidget(ui.cbWarnUndocParam);
    msgSettings.warnFormat.connectWidget(ui.leWarnFormat);
    msgSettings.warnLogFile.connectWidget(ui.leWarnLogFile->editor());
}

SetupDialog::~SetupDialog()
{

}

int SetupDialog::exec()
{
    toolSettings.storeToWidgets();
    msgSettings.storeToWidgets();
    on_bCheckTools_clicked();

    on_cbWarnings_toggled(ui.cbWarnings->isChecked());
    on_cbQuietMode_toggled(ui.cbQuietMode->isChecked());

    int i = QDialog::exec();

    switch (i)
    {
        case QDialogButtonBox::ApplyRole:
        case QDialog::Accepted:
            toolSettings.readFromWidgets();
            msgSettings.readFromWidgets();
            break;

        default:;
    }

    return i;
}

void SetupDialog::on_buttonBox_clicked(QAbstractButton* btn)
{
    switch (ui.buttonBox->buttonRole(btn))
    {
        case QDialogButtonBox::ApplyRole:
            done(QDialogButtonBox::ApplyRole);
            break;

        case QDialogButtonBox::ResetRole:
            toolSettings.storeDefaultsToWidgets();
            msgSettings.storeDefaultsToWidgets();
            break;

        case QDialogButtonBox::DestructiveRole:
            toolSettings.storeToWidgets();
            msgSettings.storeToWidgets();
            break;

        default:;
    }

    on_bCheckTools_clicked();
}

void SetupDialog::toolChanged(QWidget *w, const QString &)
{
    iisLineEdit *le = (iisLineEdit*)w;

    foreach (Option* op, toolList) {
        if (op->data[LINEEDIT].toUInt() == (quintptr)le) {
            QStringList path = iisEnvironment::systemPath();
            checkTool(op, path);
            //op->data[CHANGED] = true;
            break;
        }
    }
}

void SetupDialog::checkTool(Option* op, QStringList &path)
{
    QString s = ((QLineEdit*)op->widget)->text();
    QStringList files = iisEnvironment::availableInPath(s, path);
    QString tip;
    QLabel *label = (QLabel*)(qvariant_cast<quintptr>(op->data[LABEL]));

    switch (files.count()) {
        case 0:
            tip = tr("<b>Tool not found.</b>");
            op->data[STATE] = TOS_NOTFOUND;
            label->setPixmap(QPixmap(":/gui/icon16_off.png"));
            break;
        case 1:
            tip = tr("<b>Tool found at:</b><hr>%1").arg(files.first());
            op->data[STATE] = TOS_FOUND;
            label->setPixmap(QPixmap(":/gui/icon16_ok.png"));
            break;
        default:
            tip = tr("<b>Several tools found at:</b><hr>%1")
                .arg(files.join("<br>"));
            op->data[STATE] = TOS_SEVERAL;
            label->setPixmap(QPixmap(":/gui/icon16_ok_many.png"));
    }

    tip = tip.replace(" ", "&nbsp;");
    label->setToolTip(tip);
    op->widget->parentWidget()->setToolTip(tip);

    op->data[CHANGED] = false;
}

void SetupDialog::on_bCheckTools_clicked()
{
    QStringList path = iisEnvironment::systemPath();

    foreach (Option* op, toolList)
        checkTool(op, path);
}

////////////////////////////////////////////////////////////////////////////////

void SetupDialog::on_bDiscardTools_clicked()
{
    toolSettings.storeToWidgets();
}

void SetupDialog::on_bDiscardMsg_clicked()
{
    msgSettings.storeToWidgets();
}

void SetupDialog::on_bRestoreTools_clicked()
{
    toolSettings.storeDefaultsToWidgets();
}

void SetupDialog::on_bRestoreMsg_clicked()
{
    msgSettings.storeDefaultsToWidgets();
}

void SetupDialog::on_bApplyTools_clicked()
{
    toolSettings.readFromWidgets();
    emit applyTools();
}

void SetupDialog::on_bApplyMsg_clicked()
{
    msgSettings.readFromWidgets();
    emit applyMsg();
}

////////////////////////////////////////////////////////////////////////////////

void SetupDialog::on_cbWarnings_toggled(bool b)
{
    ui.cbWarnUndocMember->setEnabled(b);
    ui.cbWarnUndocParam->setEnabled(b);
    ui.cbWarnErrors->setEnabled(b);
    ui.leWarnFormat->setEnabled(b);
    ui.leWarnLogFile->setEnabled(b);
    ui.lFormat->setEnabled(b);
    ui.lLogFile->setEnabled(b);
}

void SetupDialog::on_cbQuietMode_toggled(bool b)
{
    ui.cbWarnings->setEnabled(!b);
    on_cbWarnings_toggled(!b);
}

