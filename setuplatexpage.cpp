/*
 * setuplatexpage.cpp
 *
 *  Created on: 17.07.2008

 */

#include "setuplatexpage.h"

SetupLatexPage::SetupLatexPage(QWidget *parent) :
    SetupBasePage(QPixmap(":/gui/icon32_latex.png"), tr("LaTeX generation"), parent)
{
    addSection(tr("General"));

    addGroup();
    addOption(tr("Enable LaTeX generator"), "GENERATE_LATEX");

    addGroup(tr("Output"));
    addOption(tr("Path"), "LATEX_OUTPUT");
    addOption(tr("Compact output"), "COMPACT_LATEX");
    addOption(tr("Paper format"), "PAPER_TYPE");

    addGroup(tr("Surround"));
    addOption(tr("Header"), "LATEX_HEADER");

    addGroup(tr("Decoration"));
    addOption(tr("Hyperlinks"), "PDF_HYPERLINKS");
    addOption(tr("Remove index"), "LATEX_HIDE_INDICES");

    addColumn();
    addColumn2();

    addSection(tr("Build"));

    addGroup(tr("Tools"));
    addOption(tr("LaTeX executable"), "LATEX_CMD_NAME");
    addOption(tr("MakeIndex executable"), "MAKEINDEX_CMD_NAME");

    addGroup(tr("Build"));
    addOption(tr("Use PDF latex"), "USE_PDFLATEX");
    addOption(tr("Batch mode"), "LATEX_BATCHMODE");
    addOption(tr("Extra packages"), "EXTRA_PACKAGES");

    finishPage();
}

SetupLatexPage::~SetupLatexPage()
{
    // TODO Auto-generated destructor stub
}
