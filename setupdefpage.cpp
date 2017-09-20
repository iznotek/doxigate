/*
 * setupdefpage.cpp
 *
 *  Created on: 17.07.2008

 */

#include "setupdefpage.h"

SetupDefPage::SetupDefPage(QWidget *parent):
        SetupBasePage(QPixmap(":/gui/icon32_def.png"), tr("AutoGen Definitions generation"), parent)
{
    addSection(tr("General"));

    addGroup();
    addOption(tr("Enable AutoGen generator"), "GENERATE_AUTOGEN_DEF");

    finishPage();
}

SetupDefPage::~SetupDefPage()
{
    // TODO Auto-generated destructor stub
}
