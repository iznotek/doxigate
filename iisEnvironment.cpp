/*
 * iisEnvironment.cpp
 *
 *  Created on: 07.08.2008
 */

#include "iisEnvironment.h"

#ifdef Q_OS_WIN32
#include <windows.h>
#include <shlobj.h>
#endif

#include <QtGui>

iisEnvironment::iisEnvironment()
{
}

iisEnvironment::~iisEnvironment()
{
}


QString iisEnvironment::systemVariable(const QString &varname)
{
    QStringList env = QProcess::systemEnvironment();
    QString pat = varname + "=";
    foreach (QString s, env) {
        if (s.startsWith(pat, Qt::CaseInsensitive))
            return s.right(s.length()-pat.length());
    }
    return QString();
}

QStringList iisEnvironment::systemPath()
{
    QString s = systemVariable("PATH");
#ifdef Q_OS_WIN32
    QStringList sl = s.split(";", QString::SkipEmptyParts);
#else
    QStringList sl = s.split(":", QString::SkipEmptyParts);
#endif
    return sl;
}

QStringList iisEnvironment::availableInPath(const QString &filename, const QStringList &pathlist, bool expandSymLinks)
{
    QStringList files;

    if (filename.isEmpty())
        return files;

    if (QDir::isAbsolutePath(filename)) {
        if (QFile::exists(filename))
            files << filename;
#ifdef Q_OS_WIN32
        else
        if (!filename.endsWith(".exe")) {
            QString tmp = filename + ".exe";
            if (QFile::exists(tmp))
                files << tmp;
        }
#endif
        return files;
    }

    foreach (QString path, pathlist) {
        path = path.replace("\\","/");
        path += "/" + filename;
        if (QFile::exists(path)) {
            if (!expandSymLinks) {
                files << path;
            } else {
                QFileInfo fi(path);
                path = fi.canonicalFilePath();
                if (!files.contains(path))
                    files << path;
            }
        }
#ifdef Q_OS_WIN32
        else
        if (!path.endsWith(".exe")) {
            QString tmp = path + ".exe";
            if (QFile::exists(tmp))
                files << tmp;
        }
#endif
    }

    return files;
}

QStringList iisEnvironment::availableInPath(const QString &filename, bool expandSymLinks)
{
    return availableInPath(filename, systemPath(), expandSymLinks);
}



void iisEnvironment::openFileDir(const QString &name)
{
    QString outDir = QDir::cleanPath(name);

    // Qt can not open URLs with spaces unless file:/// is not specified in front of.

    if (!QDesktopServices::openUrl(QUrl(QString("file:///") + outDir)))
    {
        // last chance on Windows
#ifdef Q_OS_WIN32
        ShellExecuteA(NULL, "open", outDir.toAscii().data(), NULL, NULL, SW_SHOW);
#endif
    }
}

#include <QDebug>

QStringList iisEnvironment::matchInPath(const QString &baseDir, const QString &filter)
{
    QStringList result;
    QStringList filt = filter.split("/", QString::SkipEmptyParts);
    QString dir = QFileInfo(baseDir).canonicalFilePath();

    //qDebug() << dir << filter << filt.count();

    foreach(QString s, filt) {
        QDir progDir(dir);
        QStringList sfilt; sfilt << s;
        result = progDir.entryList(sfilt, QDir::Dirs | QDir::Files);

        //qDebug() << s << result.count();

        if (result.isEmpty())
            return result;
        dir += "/" + result.first();

        //qDebug() << dir;
    }
    result.clear(); result << dir;
    return result;
}

QString iisEnvironment::applicationsPath()
{
#ifdef Q_OS_WIN32
    char path[1024];
    SHGetFolderPathA(0, CSIDL_PROGRAM_FILES, NULL, 0, path);
    return QString(path);
#else
    return "/usr/bin";
#endif
}
