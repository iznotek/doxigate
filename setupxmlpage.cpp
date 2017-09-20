/*
 * SetupXMLPage.cpp
 *
 *  Created on: 17.07.2008

 */

#include "setupxmlpage.h"

SetupXMLPage::SetupXMLPage(QWidget *parent) :
    SetupBasePage(QPixmap(":/gui/icon32_xml.png"), tr("XML generation"), parent)
{
    addSection(tr("General"));

    addGroup();
    addOption(tr("Enable XML generator"), "GENERATE_XML");

    addGroup(tr("Output"));
    addOption(tr("Path"), "XML_OUTPUT");

    addGroup(tr("Parts"));
    addOption(tr("Listing"), "XML_PROGRAMLISTING");

    addGroup(tr("Build"));
    addOption(tr("Schema"), "XML_SCHEMA");
    addOption(tr("DTD"), "XML_DTD");

//    addColumn();
//    addColumn2();

    finishPage();
}

SetupXMLPage::~SetupXMLPage()
{
    // TODO Auto-generated destructor stub
}
