#ifndef ARDUINO_H
#define ARDUINO_H

#include "clib.h"
#include "serial.h"

namespace choke
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
