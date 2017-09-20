/*
 * topwidget.h
 *
 *  Created on: 18.07.2008

 */

#ifndef TOPWIDGET_H_
#define TOPWIDGET_H_

#include <QtWidgets>

class iisToolButton;

class TopWidget : public QWidget
{
    Q_OBJECT

public:
    TopWidget(QWidget *parent);
    virtual ~TopWidget();

    void switchToProject();
    void switchToConsole();

    void updateRecentList(const QStringList &recent);

signals:
    void newProject();
    void openProject();
    void openRecentProject(const QString &filename);
    void saveProject();
    void saveProjectAs();
    void saveProjectCopy();

    void switchProject();
    void switchConsole();

    void run();
    void setup();

    void about();

    void showDocDir();
    void showHTML();
    void showCHM();
	void showQCH();
    void showRTF();
    void showXML();
    void showLatex();
    void showMAN();
    void showDEF();
    void showPerl();
    void outputsUpdate();

protected slots:
    void openMenuTriggered(QAction *a);

    void outputMenuShow();
    void outputMenuHovered(QAction *a);

protected:
	void paintEvent ( QPaintEvent * event );

	iisToolButton *projectButton, *consoleButton, *openButton;
	QMenu *openMenu, *outputMenu;
};

#endif /* TOPWIDGET_H_ */
