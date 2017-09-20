#include "setupbuild2page.h"

SetupBuild2Page::SetupBuild2Page(QWidget *parent)
: SetupBasePage(tr("Documentation: Additional parts"), parent)
{
    addSection(tr("Source browser"));

    addGroup();
	addOption(tr("Enable source browser"), "SOURCE_BROWSER");

    addGroup();
	addOption(tr("Insert sources into docs"), "INLINE_SOURCES");
	addOption(tr("Strip doxygen comments"), "STRIP_CODE_COMMENTS");
	addOption(tr("Verbatim headers"), "VERBATIM_HEADERS");

	addGroup(tr("List references"));
	addOption(tr("Show callers"), "REFERENCED_BY_RELATION");
	addOption(tr("Show called"), "REFERENCES_RELATION");
	addOption(tr("Hyperlinks to source"), "REFERENCES_LINK_SOURCE");

	addGroup(tr("HTags"));
	addOption(tr("Use HTags output"), "USE_HTAGS");

	addColumn();
	addColumn2();

    addSection(tr("Alphabetical index"));

    addGroup();
    addOption(tr("Enable index"), "ALPHABETICAL_INDEX");

    addGroup();
    addOption(tr("Columns in index"), "COLS_IN_ALPHA_INDEX");
    addOption(tr("Ignore prefix"), "IGNORE_PREFIX");

    addSection(tr("Tag files"));

    addGroup();
    addOption(tr("Input tag files"), "TAGFILES");
    addOption(tr("Generated tag file"), "GENERATE_TAGFILE");

	finishPage();
}

SetupBuild2Page::~SetupBuild2Page()
{

}
