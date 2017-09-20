/*
 * setuphtmlpage.cpp
 *
 *  Created on: 15.07.2008

 */

#include "setuphtmlpage.h"

SetupHTMLPage::SetupHTMLPage(QWidget *parent) :
    SetupBasePage(QPixmap(":/gui/icon32_html.png"), tr("HTML and CHM generation"), parent)
{
    addSection(tr("General"));

    addGroup();
    addOption(tr("Enable HTML generator"), "GENERATE_HTML");

    addGroup(tr("Output"));
    addOption(tr("Path"), "HTML_OUTPUT");
    addOption(tr("Files extension"), "HTML_FILE_EXTENSION");

    addGroup(tr("Surround"));
    addOption(tr("Header"), "HTML_HEADER");
    addOption(tr("Footer"), "HTML_FOOTER");

    addGroup(tr("Decoration"));
    addOption(tr("Stylesheet"), "HTML_STYLESHEET");
    addOption(tr("Table layout"), "HTML_ALIGN_MEMBERS");
    addOption(tr("Group enums"), "ENUM_VALUES_PER_LINE");
    addOption(tr("Formula font size"), "FORMULA_FONTSIZE");

    addGroup(tr("Parts"));
    addOption(tr("Dynamic sections"), "HTML_DYNAMIC_SECTIONS");
    addOption(tr("Expand contents"), "TOC_EXPAND");
    addOption(tr("Remove index"), "DISABLE_INDEX");

    addColumn2();

    addGroup(tr("Tree view"));
    addOption(tr("Type"), "GENERATE_TREEVIEW");
    addOption(tr("Width"), "TREEVIEW_WIDTH");


    addColumn();

    addSection(tr("Additional output"));

    addGroup(tr("CHM"));
    addOption(tr("Enable CHM generator"), "GENERATE_HTMLHELP");
    addOption(tr("CHM file"), "CHM_FILE");
    addOption(tr("Make CHI"), "GENERATE_CHI");
    addOption(tr("Encode content"), "CHM_INDEX_ENCODING");
    addOption(tr("Binary TOC"), "BINARY_TOC");

    addGroup(tr("QCH"));
    addOption(tr("Enable QCH generator"), "GENERATE_QHP");
    addOption(tr("QCH file"), "QCH_FILE");
	addOption(tr("Namespace"), "QHP_NAMESPACE");
	addOption(tr("Virtual folder"), "QHP_VIRTUAL_FOLDER");

    addColumn2();

    addGroup(tr("Tools"));
    addOption(tr("HtmlHelp compiler"), "HHC_LOCATION");
	addOption(tr("Qt Help Generator"), "QHG_LOCATION");

    addGroup(tr("XCode Docset"));
    addOption(tr("Prepare for XCode"), "GENERATE_DOCSET");
    addOption(tr("Name of feed"), "DOCSET_FEEDNAME");
    addOption(tr("Bundle ID"), "DOCSET_BUNDLE_ID");


    addGroup(tr("PHP search engine"));
    addOption(tr("Integrate PHP search"), "SEARCHENGINE");

    finishPage();
}

SetupHTMLPage::~SetupHTMLPage()
{

}
