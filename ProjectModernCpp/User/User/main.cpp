#include "User.h"
#include <QtWidgets/QApplication>
#include <Windows.h>
#include <mmsystem.h>
#include <iostream>

#pragma comment(lib, "winmm.lib")
int main(int argc, char *argv[])
{
    PlaySound(TEXT("Super.wav"), NULL, SND_ASYNC);
    QApplication a(argc, argv);
    User w;
    w.show();
    return a.exec();
}
