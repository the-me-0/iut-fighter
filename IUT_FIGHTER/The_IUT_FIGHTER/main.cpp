#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    w.setWindowState(Qt::WindowFullScreen); // Displaying game in full screen
    return a.exec();
}
