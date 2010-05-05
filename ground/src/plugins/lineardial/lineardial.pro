TEMPLATE = lib
TARGET = LineardialGadget
QT += svg
include(../../openpilotgcsplugin.pri)
include(../../plugins/coreplugin/coreplugin.pri)
include(../../libs/qwt/qwt.pri)
HEADERS += lineardialplugin.h
HEADERS += lineardialgadget.h
HEADERS += lineardialgadgetwidget.h
HEADERS += lineardialgadgetfactory.h
HEADERS += lineardialgadgetconfiguration.h
HEADERS += lineardialgadgetoptionspage.h
SOURCES += lineardialplugin.cpp
SOURCES += lineardialgadget.cpp
SOURCES += lineardialgadgetfactory.cpp
SOURCES += lineardialgadgetwidget.cpp
SOURCES += lineardialgadgetconfiguration.cpp
SOURCES += lineardialgadgetoptionspage.cpp
OTHER_FILES += LineardialGadget.pluginspec
FORMS += lineardialgadgetoptionspage.ui
