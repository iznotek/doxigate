/*
 * shared.h
 *
 *  Created on: 31.07.2008

 */

#ifndef SHARED_H_
#define SHARED_H_

#include "options.h"

////////////////////////////////////////////////////////////////////////////////

#define ToolOptionID 100

enum ToolState
{
    TOS_UNKNOWN,
    TOS_FOUND,
    TOS_NOTFOUND,
    TOS_SEVERAL
};

class ToolSettings : public Options
{
public:
    ToolSettings();

    Option doxPath, perlPath, latexPath, makeindexPath,
            dotPath, mscgenPath, hhcPath, qhpPath;
    Option defaultsForNewTools, defaultsForOpenTools;

    QString getDoxygenVersion();

protected:
    void setupPathFind(Option &op, const QString &_id, const QString &_path, const QString &_def, int _numId);
};

extern ToolSettings toolSettings;

////////////////////////////////////////////////////////////////////////////////

class MsgSettings : public Options
{
public:
    MsgSettings();

    Option defaultsForNewMsg, defaultsForOpenMsg;
    Option quiet, warnings, warnUndocMember, warnError, warnUndocParam,
        warnFormat, warnLogFile;
};

extern MsgSettings msgSettings;

////////////////////////////////////////////////////////////////////////////////

struct OutOption
{
    QString PATH_DIR, PATH_FILE;
    bool PATH_DIR_EXISTS, PATH_FILE_EXISTS;
};

class OutputFormats
{
public:
    OutputFormats();

    void checkPath();

    OutOption commonOutput;
    OutOption htmlOutput, chmOutput, latexOutput, rtfOutput, defOutput, perlOutput,
        manOutput, xmlOutput, qhpOutput;

protected:
    QList<OutOption*> myOptions;
};

extern OutputFormats outputs;

////////////////////////////////////////////////////////////////////////////////

#endif /* SHARED_H_ */
