#include <QApplication>
#include <QFont>
#include "HNetMonitor.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font("宋体",12);
    QApplication::setFont(font);
    HNetMonitor w;
    w.show();

    return a.exec();
}
