#DEFINES += TEST_MODEL #for testing: make uncomment this, make clean project and select DEBUG project configuration

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = informer
TEMPLATE = app

SOURCES += \
    main.cpp \
    main_window.cpp \
    dir_tree.cpp \
    file_system_custom_model.cpp \
    lines_counter.cpp \
#    q_sort_filter_proxy_model.cpp \
#    q_file_system_model.cpp \
#    q_standard_item_model.cpp \
#    q_abstract_item_view.cpp
#    q_table_view.cpp
    file_info_list_model.cpp

contains(DEFINES, TEST_MODEL) {
    build_pass:!message(Perform the model testing)
    include(./modeltest/modeltest.pri)
}

HEADERS += \
    main_window.h \
    dir_tree.h \
    file_system_custom_model.h \
    lines_counter.h \
    file_info_list_model.h \
    common_defines.h \
    files_info_data_impl.h

FORMS += \
    main_window.ui

RESOURCES += \
    images.qrc

RC_FILE += icoset.rc
