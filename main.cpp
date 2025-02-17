#include <QByteArray>
#include <QCoreApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>

#include <algorithm>
#include <conio.h>
#include <iostream>
#include <functional>
#include <regex>

#include "console.h"

enum class text_color
{
    blue    = 1,
    green   = 2,
    red     = 4,
    white   = 15
};

const unsigned int  vid = 0x03EB;
const unsigned int  pid = 0x6115;

const unsigned char magic_combination[32] = { 0xdc, 0x0f, 0xb8, 0xe9, 0x83, 0x57, 0x90, 0x19,
                                              0x5b, 0xf4, 0xa8, 0xe5, 0xe1, 0x22, 0xfe, 0x60,
                                              0x8e, 0x54, 0x8f, 0x46, 0xf8, 0x84, 0x10, 0xcc,
                                              0x67, 0x92, 0x92, 0x7b, 0xed, 0xbb, 0x6d, 0x55 };

const std::regex    pattern(R"(^\d{2}-\d{3}-\d{3}-\d{2}-\d{3})");

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    console     cons(CP_UTF8);

    QSerialPort port;
    QString     old_serial_number;

    const auto color_print = [&](const std::string& text, text_color color) -> void
    {
        SetConsoleTextAttribute(cons.output(), static_cast<unsigned short>(color));
        std::cout << text;
        SetConsoleTextAttribute(cons.output(), static_cast<unsigned short>(text_color::white));
    };

    const auto exit_fun = [&]() -> void
    {
        port.close();
        color_print("\n" "Для завершения нажмите любую клавишу... ", text_color::white);
        char ch = getch();
    };

    const auto is_match = [&](const std::string& str) -> bool
    {
        if(not std::regex_match(str, pattern))
        {
            color_print("Неверный формат серийного номера, попробуйте еще раз.\n", text_color::red);
            return false;
        }
        return true;
    };

    const auto device_detection = [&](const std::function<bool(const QString&, const QString&)>& func, const QString& arg1, const QString& arg2) ->bool
    {
        QString port_name;
        QString serial_number;

        foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            if(info.vendorIdentifier() == vid and info.productIdentifier()== pid)
            {
                port_name           = info.portName();
                serial_number       = info.serialNumber();
            }
        }

        if(port_name == "")
        {
            color_print("Устройство не найдено\n", text_color::red);
            return false;
        }

        if(not func(port_name, serial_number))
            return false;

        return true;
    };

    if(not device_detection([&](const QString& arg1, const QString& arg2) ->bool
        {
            port.setPortName(arg1);
            port.setBaudRate(QSerialPort::Baud115200);
            port.setDataBits(QSerialPort::Data8);
            port.setParity(QSerialPort::NoParity);
            port.setStopBits(QSerialPort::OneStop);
            port.setFlowControl(QSerialPort::NoFlowControl);

            if(not port.open(QIODevice::ReadWrite))
            {
                color_print("Устройство недоступно", text_color::red);
                return false;
            }
            else
            {
                old_serial_number = arg2;
                port.readAll();
                return true;
            }
        }, "", ""))
    {
        exit_fun();
        return EXIT_SUCCESS;
    }

    color_print("Программа записи серийного номера для преобразователя USB-RS485 \n", text_color::white);

    std::string new_serial_number;

    do
    {
        color_print("\nСейчас серийный номер:        ", text_color::green);
        color_print(old_serial_number.toStdString() + "\n", text_color::white);

        color_print("Введите новый серийный номер: ", text_color::green);
        color_print("FORWARD_DYN_", text_color::white);
        std::cin >> new_serial_number;
    }
    while(not is_match(new_serial_number));

    QString ckeck_string = "FORWARD_DYN_" + QString::fromStdString(new_serial_number);

    new_serial_number.erase(std::remove(new_serial_number.begin(), new_serial_number.end(), '-'), new_serial_number.end());

    QByteArray message;
    for(unsigned char byte : magic_combination)
        message.append(byte);

    QByteArray serial_number = QByteArray::fromStdString(new_serial_number);
    for(char byte : serial_number)
        message.append(byte);

    port.write(message);
    port.waitForBytesWritten(100);

    color_print("Новый серийный номер записан \n", text_color::white);
    color_print("Ожидание перезагрузки ", text_color::white);

    for(int i = 0; i < 37; i++)
    {
        color_print(".", text_color::blue);
        Sleep(270);
    }

    color_print("\nПроверка...", text_color::white);

    if(not device_detection([&]([[maybe_unused]] const QString& arg1, const QString& arg2) ->bool
        {
            color_print("\nНовый серийный номер:         ", text_color::green);
            color_print(arg2.toStdString() + "\n",  text_color::white);

            if(arg2 == ckeck_string)
            {
                color_print("Запись успешно завершена.\n", text_color::white);
                return true;
            }

            color_print("Серийный номер не совпадает!\n", text_color::red);
            return false;
        }, "", ""))
    {
        color_print("Ошибка! Что-то пошло не так...\n", text_color::red);
    }

    exit_fun();

    return EXIT_SUCCESS;
}
