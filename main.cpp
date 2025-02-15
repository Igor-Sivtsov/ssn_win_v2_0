#include <QCoreApplication>
#include <QtSerialPort/QSerialPortInfo>
#include "serial_port.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
    serial_port port;
}
