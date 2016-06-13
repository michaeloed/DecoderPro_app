#-------------------------------------------------
#
# Project created by QtCreator 2013-10-12T15:05:56
#
#-------------------------------------------------

QT       += gui core widgets xml

TARGET = Tables
TEMPLATE = lib

DEFINES += LIBTABLES_LIBRARY
# BLOCKS_AND_SECTIONS

SOURCES += libtables.cpp \
    turnoutwidget.cpp \
    turnouttablewidget.cpp \
    sensorwidget.cpp \
    sensortablewidget.cpp \
    reporterwidget.cpp \
    memorywidget.cpp \
    blockwidget.cpp \
    idtagswidget.cpp \
    addeditdialog.cpp \
    routewidget.cpp \
    lightwidget.cpp \
    lighttablewidget.cpp \
    addeditlightdialog.cpp \
    addeditlightcontroldialog.cpp \
    slotmonitor.cpp \
    signalheadwidget.cpp \
    beanselectcreatepanel.cpp \
    jmribeancombobox.cpp \
    dialogsignalgroupstablewidget.cpp \
    signalmastwidget.cpp \
    signalgroupswidget.cpp \
    signalmastlogicwidget.cpp \
    routetableaction.cpp \
    logixtableaction.cpp \
    defaultconditionalaction.cpp \
    lroutewidget.cpp \
    logixwidget.cpp \
    pickframe.cpp \
    lroutetableaction.cpp \
    sectiontableaction.cpp \
    sectionwidget.cpp \
    transitwidget.cpp \
    signalheadtableaction.cpp \
    signallingsourcepanel.cpp \
    signallingsourceframe.cpp \
    signallingsourceaction.cpp \
    signallingaction.cpp \
    signallingframe.cpp \
    signallingpanel.cpp \
    signalgrouptableaction.cpp \
    signalgroupsubtableaction.cpp \
    addnewhardwaredevicepanel.cpp \
    sensortableaction.cpp \
    slotmondatamodel.cpp \
    abstracttableaction.cpp \
    turnouttableaction.cpp \
    abstracttabletabaction.cpp \
    turnouttabletabaction.cpp \
    turnouteditaction.cpp \
    beaneditaction.cpp \
    beanitempanel.cpp \
    beanedititem.cpp \
    audiotableaction.cpp \
    audiosourceframe.cpp \
    abstractaudioframe.cpp \
    audiobufferframe.cpp \
    audiolistenerframe.cpp \
    audiotablepanel.cpp \
    audiotableframe.cpp \
    audiowidget.cpp \
    blocktableaction.cpp \
    addnewbeanpanel.cpp \
    tablesframe.cpp \
    trackertableaction.cpp \
    tracker.cpp \
    oblocktableaction.cpp \
    tableframes.cpp \
    oblocktablemodel.cpp \
    blockportaltablemodel.cpp \
    signaltablemodel.cpp \
    jinternalframe.cpp \
    jdesktoppane.cpp \
    jrootpane.cpp \
    portaltablemodel.cpp \
    blockpathtablemodel.cpp \
    internalframeevent.cpp \
    pathturnouttablemodel.cpp \
    blockeditaction.cpp \
    sensordebounceeditaction.cpp

HEADERS += libtables.h\
        libtables_global.h \
    turnoutwidget.h \
    turnouttablewidget.h \
    sensorwidget.h \
    sensortablewidget.h \
    reporterwidget.h \
    memorywidget.h \
    blockwidget.h \
    idtagswidget.h \
    addeditdialog.h \
    routewidget.h \
    lightwidget.h \
    lighttablewidget.h \
    addeditlightdialog.h \
    addeditlightcontroldialog.h \
    slotmonitor.h \
    signalheadwidget.h \
    beanselectcreatepanel.h \
    jmribeancombobox.h \
    dialogsignalgroupstablewidget.h \
    signalmastwidget.h \
    signalmastlogicwidget.h \
    signalgroupswidget.h \
    routetableaction.h \
    logixtableaction.h \
    defaultconditionalaction.h \
    lroutewidget.h \
    logixwidget.h \
    pickframe.h \
    lroutetableaction.h \
    sectionwidget.h \
    transitwidget.h \
    signalheadtableaction.h \
    signallingsourcepanel.h \
    signallingsourceframe.h \
    signallingsourceaction.h \
    signallingaction.h \
    signallingframe.h \
    signallingpanel.h \
    signalgrouptableaction.h \
    sectiontableaction.h \
    signalgroupsubtableaction.h \
    addnewhardwaredevicepanel.h \
    sensortableaction.h \
    slotmondatamodel.h \
    abstracttableaction.h \
    turnouttableaction.h \
    abstracttabletabaction.h \
    turnouttabletabaction.h \
    turnouteditaction.h \
    beaneditaction.h \
    beanitempanel.h \
    beanedititem.h \
    audiotableaction.h \
    audiosourceframe.h \
    abstractaudioframe.h \
    audiobufferframe.h \
    audiolistenerframe.h \
    audiotablepanel.h \
    audiotableframe.h \
    audiowidget.h \
    blocktableaction.h \
    addnewbeanpanel.h \
    tablesframe.h \
    trackertableaction.h \
    tracker.h \
    oblocktableaction.h \
    tableframes.h \
    oblocktablemodel.h \
    blockportaltablemodel.h \
    signaltablemodel.h \
    jinternalframe.h \
    jdesktoppane.h \
    jrootpane.h \
    portaltablemodel.h \
    internalframelistener.h \
    blockpathtablemodel.h \
    pathturnouttablemodel.h \
    internalframeevent.h \
    blockeditaction.h \
    sensordebounceeditaction.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

FORMS += \
    turnoutwidget.ui \
    turnouttablewidget.ui \
    sensorwidget.ui \
    sensortablewidget.ui \
    reporterwidget.ui \
    memorywidget.ui \
    blockwidget.ui \
    idtagswidget.ui \
    addeditdialog.ui \
    routewidget.ui \
    lightwidget.ui \
    lighttablewidget.ui \
    addeditlightdialog.ui \
    addeditlightcontroldialog.ui \
    slotmonitor.ui \
    signalheadwidget.ui \
    dialogsignalgroupstablewidget.ui \
    signalmastwidget.ui \
    signalmastlogicwidget.ui \
    signalgroupswidget.ui \
    routetableaction.ui \
    lroutewidget.ui \
    logixwidget.ui \
    sectionwidget.ui \
    transitwidget.ui \
    editconditionalframe.ui \
    addeditlightdialog1.ui \
    audiowidget.ui \
    tablesframe.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../libPr3/release/ -lPr3
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../libPr3/debug/ -lPr3
else:unix:!macx: LIBS += -L$$PWD/../libPr3/ -lPr3

INCLUDEPATH += $$PWD/../libPr3 $$PWD/../libPr3/Roster $$PWD/../libPr3/Signal $$PWD/../LayoutEditor/ $$PWD/../appslib/operations
DEPENDPATH += $$PWD/../libPr3 $$PWD/../libPr3/Roster $$PWD/../libPr3/Signal $$PWD/../LayoutEditor/  $$PWD/../appslib/operations

TRANSLATIONS += \
    languages/libTables_de.ts \
    languages/libTables_en.ts

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../JavaQt/release/ -lJavaQt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../JavaQt/debug/ -lJavaQt
else:unix: LIBS += -L$$PWD/../JavaQt/ -lJavaQt

INCLUDEPATH += $$PWD/../JavaQt
DEPENDPATH += $$PWD/../JavaQt

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../LayoutEditor/release/ -lLayoutEditor
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../LayoutEditor/debug/ -lLayoutEditor

INCLUDEPATH += $$PWD/../LayoutEditor/debug
DEPENDPATH += $$PWD/../LayoutEditor/debug
