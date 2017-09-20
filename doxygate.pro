TARGET = doxygate
QT += core gui widgets
HEADERS += setupmessagespage.h \
    options.h \
    iisEnvironment.h \
    shared.h \
    setupdialog.h \
    iisLineEdit.h \
    outputwidget.h \
    iisToolButton.h \
    topwidget.h \
    setupdotpage.h \
    setupperlpage.h \
    setupdefpage.h \
    setupxmlpage.h \
    setupmanpage.h \
    setuprtfpage.h \
    setuplatexpage.h \
    setuphtmlpage.h \
    iisTreeMenu.h \
    setupbuild2page.h \
    setupanalisyspage.h \
    setupbuildpage.h \
    iisStringListEdit.h \
    optionwidgets.h \
    setupbasepage.h \
    setupprojectpage.h \
    setupwidget.h \
    debug.h \
    lang_cfg.h \
    config.h \
    mainform.h \
    menuwidget.h \
    helpwidget.h \
    setuphtmlstylepage.h
SOURCES += setupmessagespage.cpp \
    options.cpp \
    iisEnvironment.cpp \
    shared.cpp \
    setupdialog.cpp \
    iisLineEdit.cpp \
    outputwidget.cpp \
    iisToolButton.cpp \
    topwidget.cpp \
    setupdotpage.cpp \
    setupperlpage.cpp \
    setupdefpage.cpp \
    setupxmlpage.cpp \
    setupmanpage.cpp \
    setuprtfpage.cpp \
    setuplatexpage.cpp \
    setuphtmlpage.cpp \
    iisTreeMenu.cpp \
    setupbuild2page.cpp \
    setupanalisyspage.cpp \
    setupbuildpage.cpp \
    iisStringListEdit.cpp \
    optionwidgets.cpp \
    setupbasepage.cpp \
    setupprojectpage.cpp \
    setupwidget.cpp \
    version.cpp \
    debug.cpp \
    config.cpp \
    main.cpp \
    mainform.cpp \
    portable.cpp \
    helpwidget.cpp \
    menuwidget.cpp \
    setuphtmlstylepage.cpp
FORMS += setupdialog.ui \
    mainform.ui
win32 { 
    HEADERS += 3rdparty/icon_config.h
    SOURCES += 3rdparty/localcharset.c \
        3rdparty/relocatable.c \
        3rdParty/iconv.c
    RC_FILE += gui/res.rc
    win32-msvc:CONFIG += MSVC
    win32-msvc.net:CONFIG += MSVC
    win32-msvc2002:CONFIG += MSVC
    win32-msvc2003:CONFIG += MSVC
    win32-msvc2005:CONFIG += MSVC
    win32-msvc2008:CONFIG += MSVC
    MSVC:LIBS += shell32.lib
}
MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
INCLUDEPATH += . \
    3rdParty
LIBPATH += .
LIBS += 
RESOURCES = mainform.qrc
target.path = /usr/local/bin
INSTALL = target
