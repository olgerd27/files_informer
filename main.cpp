#include <QApplication>
#include "main_window.h"

#ifndef TEST_MODEL
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
#endif
