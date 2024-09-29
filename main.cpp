#include "mathanalygui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mathanalygui w;
    w.show();
    return a.exec();
}
