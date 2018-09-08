#include "event_handler.h"

namespace choke
{
    void event_handler::set_timer(uint16_t timer_n, uint16_t compare, uint16_t arg)
    {
        if (timer_n > 2) {
            return;
        }

        // 16ms is the maximum timer0|2 can reach
        if ((timer_n == 0 || timer_n == 2) && arg > 16384) {
            return;
        }

        uint8_t prescaler;
        uint16_t val = (arg / (1.0 / F_CPU)) - 1;
        if (val <= 256) {
            prescaler = 0x1;
        }
        else if (val <= 2048) {
            prescaler = 0x2;
        }
        else if (val <= 16384) {
            prescaler = 0x3;
        }
        else if (val <= 131072) {
            prescaler = 0x4;
        }
        else if (val <= 1048576) {
            prescaler = 0x5;
        }
        else {
            return;
        }

        cli();
        if (timer_n == 0) {
            TCCR0A |= prescaler;
            TCCR0A |= (compare != 0) ? (1 << WGM01) : 0;
            TIMSK0 |= (compare != 0) ? (1 << OCIE0A) : (1 << TOIE0);
            TCNT0 = 0;
        }
        else if (timer_n == 1) {
            TCCR1A |= 0;
            TCCR1B |= prescaler;
            TCCR1B |= (compare != 0) ? (1 << WGM12) : 0;
            TIMSK0 |= (compare != 0) ? (1 << OCIE1A) : (1 << TOIE1);
            TCNT1  = 0;
            OCR1A  = val;
        }
        else if (timer_n == 2) {
            TCCR2A |= prescaler;
            TCCR2A |= (compare != 0) ? (1 << WGM21) : 0;
            TIMSK0 |= (compare != 0) ? (1 << OCIE2A) : (1 << TOIE2);
            TCNT2 = 0;
        }
        else {

        }
        sei();
    }
}
