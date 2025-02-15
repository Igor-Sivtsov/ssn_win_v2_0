#include <QCoreApplication>
#include <QtSerialPort/QSerialPortInfo>
#include "serial_port.h"

enum class text_color
{
    blue    = 1,
    green   = 2,
    red     = 4,
    white   = 15
};
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
    serial_port port;
}
