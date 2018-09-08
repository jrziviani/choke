#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "clib.h"
#include "arduino.h"

#ifndef FCPU
#define FCPU 16000000
#endif

namespace soc
{
    class event_t
    {
    };

    enum class event_types : uint8_t
    {
        RESET,
        INTERRUPT,
        WATCHDOG,
        TIMER,
        USART,
        ANALOG_COMP,
    };

    template <uint16_t event>
    constexpr event_types get_type()
    {
        if constexpr(event == 0)
            return event_types::RESET;

        else if constexpr(event >= 1 && event <= 5)
            return event_types::INTERRUPT;

        else if constexpr(event == 6)
            return event_types::WATCHDOG;

        else if constexpr(event >= 7 && event <= 16)
            return event_types::TIMER;

        else if constexpr(event >= 18 && event <= 20)
            return event_types::USART;

        else if constexpr(event == 23)
            return event_types::ANALOG_COMP;

        // this won't be even compiled EXCEPT if none of the
        // other cases is true
        else
            static_assert(event == 999999);
    }

    using handler_t = void (arduino::*)(const event_t&);
    class event_handler
    {
    public:
        event_handler() = default;
        ~event_handler() = default;
        event_handler(const event_handler&) = delete;
        event_handler(event_handler&&) = delete;

        event_handler &operator=(const event_handler&) = delete;
        event_handler &operator=(event_handler&&) = delete;

    public:
        template <uint16_t event>
        void register_event(handler_t handler,
                            uint16_t timer_n,
                            uint16_t compare,
                            uint16_t arg)
        {
            switch (get_type<event>()) {
                case event_types::RESET:
                    break;

                case event_types::INTERRUPT:
                    break;

                case event_types::TIMER:
                    set_timer(timer_n, compare, arg);
                    break;

                case event_types::USART:
                    break;

                default:
                    break;
            }
        }

        void set_timer(uint16_t timer_n, uint16_t compare, uint16_t arg);
    };
}

#endif // EVENT_HANDLER_H
