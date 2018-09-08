#ifndef ARDUINO_H
#define ARDUINO_H

#include "clib.h"
#include "serial.h"

namespace soc
{
    enum REFERENCE : uint8_t
    {
        DEFAULT,    // 5V or 3V3
        EXTERNAL,   // voltage applied to AREF (0 or 5) is used as ref.
        RESERVED,
        INTERNAL,   // 1V1 on atmega328
    };

    struct avrpin
    {
        uint8_t mask;
        volatile void *pin;
        volatile void *port;
        volatile void *ddr;
        volatile void *timer;
    };

    enum class MODE
    {
        INPUT,
        OUTPUT,
    };

    const avrpin arduino_pins[] PROGMEM = {
        {_BV(0), &PIND, &PORTD, &DDRD, nullptr},
        {_BV(1), &PIND, &PORTD, &DDRD, nullptr},
        {_BV(2), &PIND, &PORTD, &DDRD, nullptr},
        {_BV(3), &PIND, &PORTD, &DDRD, &OCR2B},
        {_BV(4), &PIND, &PORTD, &DDRD, nullptr},
        {_BV(5), &PIND, &PORTD, &DDRD, &OCR0B},
        {_BV(6), &PIND, &PORTD, &DDRD, &OCR0A},
        {_BV(7), &PIND, &PORTD, &DDRD, nullptr},
        {_BV(0), &PINB, &PORTB, &DDRB, nullptr},
        {_BV(1), &PINB, &PORTB, &DDRB, &OCR1A},
        {_BV(2), &PINB, &PORTB, &DDRB, &OCR1B},
        {_BV(3), &PINB, &PORTB, &DDRB, &OCR2A},
        {_BV(4), &PINB, &PORTB, &DDRB, nullptr},
        {_BV(5), &PINB, &PORTB, &DDRB, nullptr},
        {_BV(0), &PINC, &PORTC, &DDRC, nullptr},
        {_BV(1), &PINC, &PORTC, &DDRC, nullptr},
        {_BV(2), &PINC, &PORTC, &DDRC, nullptr},
        {_BV(3), &PINC, &PORTC, &DDRC, nullptr},
        {_BV(4), &PINC, &PORTC, &DDRC, nullptr},
        {_BV(5), &PINC, &PORTC, &DDRC, nullptr},
    };

    class arduino
    {
    protected:
        REFERENCE reference_;
        const usart serial_;

    public:
        virtual ~arduino() = default;
        arduino(const arduino&) = delete;
        arduino(arduino&&) = delete;

        arduino &operator=(const arduino&) = delete;
        arduino &operator=(arduino&&) = delete;

    public:
        arduino() :
            reference_(DEFAULT),
            serial_(9600)
        {
        }

        const usart &serial() const
        {
            return serial_;
        }

        void set_analog_reference(REFERENCE mode)
        {
            reference_ = mode;
        }
    };
}

#endif // ARDUINO_H
