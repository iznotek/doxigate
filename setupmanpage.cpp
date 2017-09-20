/*
 * setupmanpage.cpp
 *
 *  Created on: 17.07.2008

 */

#include "setupmanpage.h"

SetupManPage::SetupManPage(QWidget *parent) :
    SetupBasePage(QPixmap(":/gui/icon32_man.png"), tr("MAN generation"), parent)
{
    addSection(tr("General"));

    addGroup();
    addOption(tr("Enable MAN generator"), "GENERATE_MAN");

    addGroup(tr("Output"));
    addOption(tr("Path"), "MAN_OUTPUT");
    addOption(tr("Extension"), "MAN_EXTENSION");

    addGroup(tr("Build"));
    addOption(tr("Split to pages"), "MAN_LINKS");

//    addColumn();
//    addColumn2();

    finishPage();
}

SetupManPage::~SetupManPage()
{
    // TODO Auto-generated destructor stub
}
