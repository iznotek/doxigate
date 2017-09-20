/*
 * setupmessagespage.cpp
 *
 *  Created on: 14.08.2008
 *      Author: mas
 */

#include "setupmessagespage.h"

SetupMessagesPage::SetupMessagesPage(QWidget *parent) :
    SetupBasePage(tr("Doxygen messages"), parent)
{
    addGroup();
    addOption(tr("Suppress all messages"), "QUIET");

    addGroup(tr("Warnings"));
    addOption(tr("Enable warnings"), "WARNINGS");
    addOption(tr("Warn on undocumented members"), "WARN_IF_UNDOCUMENTED");
    addOption(tr("Warn on undocumented parameters"), "WARN_NO_PARAMDOC");
    addOption(tr("Warn on errors"), "WARN_IF_DOC_ERROR");
    addOption(tr("Warnings format"), "WARN_FORMAT");
    addOption(tr("Warnings to file"), "WARN_LOGFILE");

    finishPage();
}

SetupMessagesPage::~SetupMessagesPage()
{
    // TODO Auto-generated destructor stub
}
