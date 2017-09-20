/*
 * setupbuildpage.cpp
 *
 *  Created on: 10.07.2008

 */

#include "setupbuildpage.h"

SetupBuildPage::SetupBuildPage(QWidget *parent) :
    SetupBasePage(tr("Documentation"), parent)
{
    //addSection(tr("Generation"));

    addGroup(tr("Brief descriptions"));
    addOption(tr("Allow briefs"), "BRIEF_MEMBER_DESC");
    addOption(tr("Repeat before detailed"), "REPEAT_BRIEF");
    addOption(tr("Abbreviations"), "ABBREVIATE_BRIEF");
    addOption(tr("Use JavaDoc style"), "JAVADOC_AUTOBRIEF");
    addOption(tr("Use Qt style"), "QT_AUTOBRIEF");
    addOption(tr("Use multiline C++"), "MULTILINE_CPP_IS_BRIEF");

    addGroup(tr("Detailed descriptions"));
    //addOption(tr("Details at the top"), "DETAILS_AT_TOP");	// obsolete
    addOption(tr("Details from brief"), "ALWAYS_DETAILED_SEC");
	addOption(tr("Layout file"), "LAYOUT_FILE");

    addGroup(tr("Member grouping"));
    addOption(tr("Comment per group"), "DISTRIBUTE_GROUP_DOC");
    addOption(tr("Subgroups"), "SUBGROUPING");

	addColumn2();

    addGroup(tr("Pages"));
    addOption(tr("Page per member"), "SEPARATE_MEMBER_PAGES");
    addOption(tr("Namespaces"), "SHOW_NAMESPACES");
    addOption(tr("Files"), "SHOW_FILES");

    addGroup(tr("Lists"));
    addOption(tr("Todo list"), "GENERATE_TODOLIST");
    addOption(tr("Test list"), "GENERATE_TESTLIST");
    addOption(tr("Bug list"), "GENERATE_BUGLIST");
    addOption(tr("Deprecated list"), "GENERATE_DEPRECATEDLIST");

    addColumn();
    //addSection(tr("Generation"));

    addGroup(tr("Sorting"));
    addOption(tr("Details"), "SORT_MEMBER_DOCS");
    addOption(tr("Briefs"), "SORT_BRIEF_DOCS");
    addOption(tr("Groups"), "SORT_GROUP_NAMES");
    addOption(tr("Sort by scope names"), "SORT_BY_SCOPE_NAME");

    addGroup(tr("Externals"));
    addOption(tr("External classes"), "ALLEXTERNALS");
    addOption(tr("External groups"), "EXTERNAL_GROUPS");

    //addColumn();
	addColumn2();

    addGroup(tr("Files"));
    addOption(tr("Use full filenames"), "FULL_PATH_NAMES");
    addOption(tr("Strip path"), "STRIP_FROM_PATH");
    addOption(tr("Strip include path"), "STRIP_FROM_INC_PATH");
    addOption(tr("Show used files"), "SHOW_USED_FILES");
    addOption(tr("Show included files"), "SHOW_INCLUDE_FILES");
    addOption(tr("Show directories"), "SHOW_DIRECTORIES");
    addOption(tr("File version filter"), "FILE_VERSION_FILTER");

    addGroup(tr("Inherited members"));
    addOption(tr("Include inherited"), "INLINE_INHERITED_MEMB");
    addOption(tr("Autodocument inherited"), "INHERIT_DOCS");

    addGroup();
    addOption(tr("Show inline"), "INLINE_INFO");
    addOption(tr("Hide typedef names"), "TYPEDEF_HIDES_STRUCT");
    addOption(tr("Hide scope names"), "HIDE_SCOPE_NAMES");

    addGroup();
    addOption(tr("Tabs size"), "TAB_SIZE");

    finishPage();
}

SetupBuildPage::~SetupBuildPage()
{
    // TODO Auto-generated destructor stub
}
