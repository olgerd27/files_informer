QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = informer
TEMPLATE = app

SOURCES += \
    main.cpp \
    main_window.cpp \
    dir_tree.cpp

HEADERS += \
    main_window.h \
    dir_tree.h

FORMS += \
    main_window.ui

RESOURCES +=

RC_FILE += icoset.rc
