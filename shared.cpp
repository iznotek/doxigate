/*
 * shared.cpp
 *
 *  Created on: 31.07.2008

 */

#include "shared.h"

#include "iisEnvironment.h"

#include <QDebug>

////////////////////////////////////////////////////////////////////////////////

ToolSettings toolSettings;

ToolSettings::ToolSettings() : Options()
{
    // tools
    setup(defaultsForNewTools, "DefaultsForNewTools", true);
    setup(defaultsForOpenTools, "DefaultsForOpenTools", true);

#ifdef Q_OS_WIN32   // windows defaults
    setupPathFind(doxPath, "DoxygenPath", "doxygen*/bin/doxygen*.exe", "doxygen", ToolOptionID);
    setupPathFind(perlPath, "PerlPath", "perl*/bin/perl*.exe", "perl", ToolOptionID);
    setupPathFind(dotPath, "DotPath", "graphviz*/bin/dot*.exe", "dot", ToolOptionID);
    setupPathFind(hhcPath, "HtmlHelpPath", "HTML Help Workshop/hhc*.exe", "hhc", ToolOptionID);
	setupPathFind(qhpPath, "QtHelpPath", "qt*/bin/qhelpgenerator*.exe", "qhelpgenerator", ToolOptionID);
    setupPathFind(mscgenPath, "MscgenPath", "mscgen/mscgen*.exe", "mscgen", ToolOptionID);
    setupPathFind(latexPath, "LatexPath", "latex*/bin/latex*.exe", "latex", ToolOptionID);
    setupPathFind(makeindexPath, "MakeindexPath", "latex*/bin/makeindex*.exe", "makeindex", ToolOptionID);
#else   // linux defaults
    setup(doxPath, "DoxygenPath", "doxygen", ToolOptionID);
    setup(perlPath, "PerlPath", "perl", ToolOptionID);
    setup(latexPath, "LatexPath", "latex", ToolOptionID);
    setup(makeindexPath, "MakeindexPath", "makeindex", ToolOptionID);
    setup(dotPath, "DotPath", "dot", ToolOptionID);
    setup(mscgenPath, "MscgenPath", "mscgen", ToolOptionID);
    setup(hhcPath, "HtmlHelpPath", "hhc", ToolOptionID);
	setup(qhpPath, "QtHelpPath", "qhelpgenerator", ToolOptionID);
#endif
}

void ToolSettings::setupPathFind(Option &op, const QString &_id, const QString &_path, const QString &_def, int _numId)
{
#ifdef Q_OS_WIN32
    QStringList sl = iisEnvironment::matchInPath(iisEnvironment::applicationsPath(), _path);
    if (sl.isEmpty())
        setup(op, _id, _def, _numId);
    else
        setup(op, _id, sl.first(), _numId);
#endif
}

QString ToolSettings::getDoxygenVersion()
{
    QProcess doxProcess;
    QStringList arg; arg << "--version";
    doxProcess.start(doxPath, arg);
    if (doxProcess.waitForStarted()) {
        if (doxProcess.waitForFinished()) {
            QString ver = doxProcess.readAllStandardOutput();
            return ver.trimmed();
        }
    }
    
    return QString();
}

////////////////////////////////////////////////////////////////////////////////

MsgSettings msgSettings;

MsgSettings::MsgSettings() : Options()
{
    // messages
    setup(defaultsForNewMsg, "DefaultsForNewMsg", true);
    setup(defaultsForOpenMsg, "DefaultsForOpenMsg", true);

    setup(quiet, "QuietMode", false);
    setup(warnings, "Warnings", true);
    setup(warnUndocMember, "WarnUndocMember", true);
    setup(warnError, "WarnError", true);
    setup(warnUndocParam, "WarnUndocParam", true);
    setup(warnFormat, "WarnFormat", "File [$file] at line [$line]: $text");
    setup(warnLogFile, "WarnLogFile", "");
}

////////////////////////////////////////////////////////////////////////////////

OutputFormats outputs;

OutputFormats::OutputFormats()
{
    myOptions << &commonOutput;
    myOptions << &htmlOutput << &chmOutput << &latexOutput << &rtfOutput << &defOutput
        << &perlOutput << &manOutput << &xmlOutput << &qhpOutput;
}

void OutputFormats::checkPath()
{
    foreach(OutOption *o, myOptions) {
        o->PATH_DIR_EXISTS = (QDir(o->PATH_DIR).exists());
        o->PATH_FILE_EXISTS = (QFile::exists(o->PATH_FILE));
    }
}

