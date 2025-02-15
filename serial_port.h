#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <QSerialPort>
#include <QSerialPortInfo>

class serial_port : public QSerialPort, public QSerialPortInfo
{
public:
                    serial_port();
                    ~serial_port();

    bool            device_detection(const unsigned int vid, const unsigned int pid);
    bool            open_();
    bool            close_();
    void            send_data(const char *buf, unsigned int len);

    QString         get_serial_number() const;

private:

    QString         name;
    QString         serial_number;
};

#endif // SERIAL_PORT_H
