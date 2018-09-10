#ifndef BOARD_H
#define BOARD_H

#include "clib.h"
#include "event_handler.h"

namespace choke
{
    class board
    {
    private:
        event_handler events_;

    protected:
        board()
        {
        }

        ~board() = default;

        template <uint16_t event_n, class C>
        void register_event(void (C::*fn)(const event_t&),
                            uint16_t timer_n,
                            uint16_t compare,
                            uint16_t arg)
        {
            events_.register_event<event_n>(fn, timer_n, compare, arg);
        }
    };
}

#endif // BOARD_H
