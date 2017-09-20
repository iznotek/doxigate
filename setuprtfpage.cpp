/*
 * setuprtfpage.cpp
 *
 *  Created on: 17.07.2008

 */

#include "setuprtfpage.h"

SetupRTFPage::SetupRTFPage(QWidget *parent) :
        SetupBasePage(QPixmap(":/gui/icon32_rtf.png"), tr("RTF generation"), parent)
{
    addSection(tr("General"));

    addGroup();
    addOption(tr("Enable RTF generator"), "GENERATE_RTF");

    addGroup(tr("Output"));
    addOption(tr("Path"), "RTF_OUTPUT");
    addOption(tr("Compact output"), "COMPACT_RTF");

    addGroup(tr("Decoration"));
    addOption(tr("Hyperlinks"), "RTF_HYPERLINKS");
    addOption(tr("Stylesheet"), "RTF_STYLESHEET_FILE");

    addGroup(tr("Build"));
    addOption(tr("Additional config"), "RTF_EXTENSIONS_FILE");

//    addColumn();
//    addColumn2();

    finishPage();
}

SetupRTFPage::~SetupRTFPage()
{
    // TODO Auto-generated destructor stub
}
