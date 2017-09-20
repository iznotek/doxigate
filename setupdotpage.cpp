/*
 * setupdotpage.cpp
 *
 *  Created on: 18.07.2008

 */

#include "setupdotpage.h"

SetupDotPage::SetupDotPage(QWidget *parent) :
    SetupBasePage(tr("Class Diagrams"), parent)
{
    addSection(tr("Built-in generator"));

    addGroup();
    addOption(tr("Enable diagram generator"), "CLASS_DIAGRAMS");

    addGroup(tr("Output"));
    addOption(tr("Hide undocumented relations"), "HIDE_UNDOC_RELATIONS");


    addSection(tr("DOT generator"));

    addGroup();
    addOption(tr("Enable DOT generator"), "HAVE_DOT");

    addGroup(tr("Output"));
    addOption(tr("DOT file directories"), "DOTFILE_DIRS");

    addGroup(tr("Fonts"));
    addOption(tr("Font name"), "DOT_FONTNAME");
    addOption(tr("Font path"), "DOT_FONTPATH");
	addOption(tr("Font size"), "DOT_FONTSIZE");


    addColumn2();

    addGroup(tr("Graphs"));
    addOption(tr("Class graphs"), "CLASS_GRAPH");
    addOption(tr("Collaboration graphs"), "COLLABORATION_GRAPH");
    addOption(tr("Group graphs"), "GROUP_GRAPHS");
    addOption(tr("Template relations"), "TEMPLATE_RELATIONS");
    addOption(tr("\"Include\" graphs"), "INCLUDE_GRAPH");
    addOption(tr("\"Included by\" graphs"), "INCLUDED_BY_GRAPH");
    addOption(tr("Call graphs"), "CALL_GRAPH");
    addOption(tr("Caller graphs"), "CALLER_GRAPH");
    addOption(tr("Directory graphs"), "DIRECTORY_GRAPH");

    addColumn();

    addGroup(tr("Decoration"));
    addOption(tr("UML-style graphs"), "UML_LOOK");
    addOption(tr("Graphical hierarchy"), "GRAPHICAL_HIERARCHY");
    addOption(tr("Max nodes"), "DOT_GRAPH_MAX_NODES");
    addOption(tr("Max depths"), "MAX_DOT_GRAPH_DEPTH");
    addOption(tr("Legend"), "GENERATE_LEGEND");

    addColumn2();

    addGroup(tr("Images"));
    addOption(tr("Format"), "DOT_IMAGE_FORMAT");
    addOption(tr("Transparent background"), "DOT_TRANSPARENT");

    addGroup(tr("Build"));
    addOption(tr("Multiple targets"), "DOT_MULTI_TARGETS");
    addOption(tr("Cleanup after run"), "DOT_CLEANUP");


    addSection(tr("Build"));

    addGroup(tr("Tools"));
    addOption(tr("Path to MSCGEN"), "MSCGEN_PATH");
    addOption(tr("Path to DOT"), "DOT_PATH");

    finishPage();
}

SetupDotPage::~SetupDotPage()
{
    // TODO Auto-generated destructor stub
}
