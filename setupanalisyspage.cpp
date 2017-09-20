/*
 * setupanalisyspage.cpp
 *
 *  Created on: 11.07.2008

 */

#include "setupanalisyspage.h"

SetupAnalisysPage::SetupAnalisysPage(QWidget *parent) :
    SetupBasePage(tr("Code Analysis"), parent)
{
    addSection(tr("Extraction"));

    addGroup(tr("What to extract"));
    addOption(tr("All public items"), "EXTRACT_ALL");
    addOption(tr("Private members"), "EXTRACT_PRIVATE");
    addOption(tr("Static members"), "EXTRACT_STATIC");
    addOption(tr("Local classes"), "EXTRACT_LOCAL_CLASSES");
    addOption(tr("Local methods"), "EXTRACT_LOCAL_METHODS");
    addOption(tr("Anonymous namespaces"), "EXTRACT_ANON_NSPACES");

    addGroup(tr("What to skip"));
    addOption(tr("Undocumented members"), "HIDE_UNDOC_MEMBERS");
    addOption(tr("Undocumented classes"), "HIDE_UNDOC_CLASSES");
    addOption(tr("Friend classes"), "HIDE_FRIEND_COMPOUNDS");
    addOption(tr("In-body documentation"), "HIDE_IN_BODY_DOCS");

    addGroup();
    addOption(tr("Sections"), "ENABLED_SECTIONS");
    addOption(tr("Internal documentation"), "INTERNAL_DOCS");
    addOption(tr("Aliases"), "ALIASES");
    addOption(tr("Maximum init lines"), "MAX_INITIALIZER_LINES");

    addColumn();
	addColumn2();
    addSection(tr("Preprocessor"));

    addGroup();
    addOption(tr("Enable preprocessor"), "ENABLE_PREPROCESSING");

    addGroup(tr("General"));
    addOption(tr("Expand macros"), "MACRO_EXPANSION");
    addOption(tr("Skip function macros"), "SKIP_FUNCTION_MACROS");

    addGroup(tr("Predefined"));
    addOption(tr("Only predefined"), "EXPAND_ONLY_PREDEF");
    addOption(tr("Predefined macros"), "PREDEFINED");
    addOption(tr("Macros to expand"), "EXPAND_AS_DEFINED");

    addGroup(tr("Macro includes"));
    addOption(tr("Use additional includes"), "SEARCH_INCLUDES");
    addOption(tr("Directories"), "INCLUDE_PATH");
    addOption(tr("File patterns"), "INCLUDE_FILE_PATTERNS");

    finishPage();
}

SetupAnalisysPage::~SetupAnalisysPage()
{
    // TODO Auto-generated destructor stub
}
