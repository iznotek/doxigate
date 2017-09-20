/*
 * iisEnvironment.h
 *
 *  Created on: 07.08.2008
 *
 *  Industrial Infosystems (c) 2008
 *  www.ii-system.com
 *
 *  This component is freeware and could be used while building any applications.
 *  Removal/modification of this information not permitted.
 */

#ifndef IISENVIRONMENT_H_
#define IISENVIRONMENT_H_

#include <QtCore>

/**
 * \class iisEnvironment
 * \brief This class encapsulates functionality for easier access to the system variables.
 */
class iisEnvironment
{
public:
    iisEnvironment();
    virtual ~iisEnvironment();

    static QString systemVariable(const QString &varname);

    static QStringList systemPath();
    static QStringList availableInPath(const QString &filename, bool expandSymLinks = true);
    static QStringList availableInPath(const QString &filename, const QStringList &pathlist, bool expandSymLinks = true);

    static QString applicationsPath();

    static void openFileDir(const QString &name);
    static QStringList matchInPath(const QString &baseDir, const QString &filter);
};

#endif /* IISENVIRONMENT_H_ */
