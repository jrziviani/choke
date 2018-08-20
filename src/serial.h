#ifndef SERIAL_H
#define SERIAL_H

#include "clib.h"

#include <iterator>
#include <string>

namespace soc
{
    class usart
    {
    private:

    public:
        usart(uint16_t baud)
        {
            init(baud);
        }

        void init(uint16_t baud) const
        {
            auto pbaud = (F_CPU / (baud * 16UL)) - 1;
            UBRR0H = static_cast<uint8_t>(pbaud >> 8);
            UBRR0L = static_cast<uint8_t>(pbaud & 0xff);
            UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
            UCSR0C |= (3 << UCSZ00);
        }

        void write(unsigned char byte) const
        {
            while (!(UCSR0A & (1 << UDRE0)));
            UDR0 = byte;
        }

        unsigned char read() const
        {
            while (UCSR0A & (0 << UDRE0));
            return UDR0;
        }

        void print(const std::string &str) const
        {
            for (auto c : str)
                write(c);
        }

        std::string get_line(char eol = '\n') const
        {
            std::string ret;
            unsigned char c;
            while ((c = read()) != eol)
                ret += c;
        }
    };
}

#endif
