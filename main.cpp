#include <QString>

#include <conio.h>
#include <iostream>
#include <regex>

#include "console.h"
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
    console cons(CP_UTF8);

    serial_port port;

    const auto color_print = [&](const std::string& text, text_color color) -> void
    {
        SetConsoleTextAttribute(cons.output(), static_cast<unsigned short>(color));
        std::cout << text;
        SetConsoleTextAttribute(cons.output(), static_cast<unsigned short>(text_color::white));
    };

    const auto exit_fun = [&]() -> void
    {
        port.close_();
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
}
