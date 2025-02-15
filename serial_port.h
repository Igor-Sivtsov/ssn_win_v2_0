#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <QSerialPort>
#include <QSerialPortInfo>

class serial_port : public QSerialPort, public QSerialPortInfo
{
public:
            serial_port();
            ~serial_port();

    void    device_detection(const unsigned int vid, const unsigned int pid);
    bool    open_();
    bool    close_();
    void    resive_data();
    void    send_data();

private:

    QString port;
    QString serial_number;

};

#endif // SERIAL_PORT_H
