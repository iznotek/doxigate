/*
 * setupperlpage.cpp
 *
 *  Created on: 18.07.2008

 */

#include "setupperlpage.h"

SetupPerlPage::SetupPerlPage(QWidget *parent):
        SetupBasePage(QPixmap(":/gui/icon32_pl.png"), tr("Perl module generation"), parent)
{
    addSection(tr("General"));

    addGroup();
    addOption(tr("Enable Perl generator"), "GENERATE_PERLMOD");

    addGroup(tr("Output"));
    addOption(tr("Format output"), "PERLMOD_PRETTY");

    addGroup(tr("Build"));
    addOption(tr("LaTeX support"), "PERLMOD_LATEX");
    addOption(tr("Variable prefix"), "PERLMOD_MAKEVAR_PREFIX");

    addGroup(tr("Tools"));
    addOption(tr("Path to Perl"), "PERL_PATH");

    finishPage();

}

SetupPerlPage::~SetupPerlPage()
{
    // TODO Auto-generated destructor stub
}
