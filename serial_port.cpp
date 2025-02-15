#include "serial_port.h"

serial_port::serial_port()  {}
serial_port::~serial_port() {}

bool serial_port::device_detection(const unsigned int vid, const unsigned int pid)
{
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if(info.vendorIdentifier() == vid and info.productIdentifier()== pid)
        {
            this->name          = info.portName();
            this->serial_number = info.serialNumber();
            return true;
        }
    }
    return false;
}

bool serial_port::open_()
{
    this->setPortName(this->name);
    this->setBaudRate(QSerialPort::Baud115200);

    if(!this->open(QIODevice::ReadWrite))
        return false;
    else
    {
        this->readAll();
        return true;
    }
}

bool serial_port::close_()
{
    this->close();

    if(this->isOpen() == false)
        return true;

    return false;
}

void serial_port::send_data(const char *buf, unsigned int len)
{
    this->write(buf, len);
}

QString serial_port::get_serial_number() const
{
    return this->serial_number;
}
