QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = informer
TEMPLATE = app

SOURCES += \
    main.cpp \
    main_window.cpp \
    dir_tree.cpp \
#    q_sort_filter_proxy_model.cpp
#    q_file_system_model.cpp
#    q_standard_item_model.cpp


HEADERS += \
    main_window.h \
    dir_tree.h

FORMS += \
    main_window.ui

RESOURCES += \
    images.qrc

RC_FILE += icoset.rc
