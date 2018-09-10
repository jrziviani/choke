#ifndef DRIVER_H
#define DRIVER_H

#include "car.h"
#include "board.h"

enum obstacle : uint8_t
{
    FAR,
    MEDIUM,
    CLOSE,
    BLOCK,
};

enum nm_states : uint8_t
{
    STOP,
    START,
    LEFT,
    RIGHT,
    BACKWARD,
};

struct fn_states
{
    nm_states state;
    void (car::*method)() const;
};

const fn_states states[] = {
    { nm_states::STOP,     &car::stop },
    { nm_states::START,    &car::start },
    { nm_states::LEFT,     &car::turn_left },
    { nm_states::RIGHT,    &car::turn_right },
    { nm_states::BACKWARD, &car::reverse }
};

class driver : public choke::board
{
    uint8_t pin_echo_;
    uint8_t pin_trigger_;
    uint8_t current_state_;
    uint8_t last_block_turn_;
    uint32_t time_;
    bool timer_;
    volatile uint8_t prec_;
    volatile uint16_t ms_;
    car *car_;
    choke::atmega328p arduino_;

private:
    driver(uint8_t echo, uint8_t trigger);
    driver();
    void init();
    uint32_t ms();

public:
    ~driver() = default;

    driver(const driver&) = delete;
    driver(driver&&) = delete;
    driver &operator=(const driver&) = delete;
    driver &operator=(driver&&) = delete;

    void set_car(car *const c);
    void set_echo_trigger(uint8_t echo, uint8_t trigger);
    choke::atmega328p &get_arduino();

    obstacle get_obstacle();
    void go();

    const char *state_to_str();
    const char *obstacle_to_str();

    void handle_timer0_compa(const choke::event_t &e);
    void handle_timer0_ovf(const choke::event_t &e);

/*static*/ public:
    static driver &instance()
    {
        static driver inst;
        return inst;
    }
};

#endif // DRIVER_H
