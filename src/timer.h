#ifndef TIMER_H
#define TIMER_H

#include "clib.h"

#include <util/delay.h>

namespace choke
{
    inline void delay_us(double usec)
    {
        return _delay_us(usec);
    }

    inline void delay_ms(double usec)
    {
        return _delay_ms(usec);
    }
}

#endif // TIMER_H
