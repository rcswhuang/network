#include <QApplication>
#include <QFont>
#include "HNetMonitor.h"
int main(int argc, char *argv[])
{
   // char temp[4] = {'\0','\0','\0','5'};
  //  int n = atoi(temp);

    QApplication a(argc, argv);
    QFont font("宋体",12);
    QApplication::setFont(font);
    HNetMonitor w;
    w.show();

    return a.exec();
}
