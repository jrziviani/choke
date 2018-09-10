#ifndef CAR_H
#define CAR_H

#include "atmega328p.h"
#include "event_handler.h"

struct engine
{
    uint8_t forward;
    uint8_t speed;
    uint8_t pin_forward;
    uint8_t pin_backward;
    uint8_t pin_speed;
};

class car
{
    mutable engine left_;
    mutable engine right_;
    choke::atmega328p &arduino_;

public:
    car(engine left, engine right, choke::atmega328p &arduino);
    car(choke::atmega328p &arduino);
    ~car() = default;

    car() = delete;
    car(const car&) = delete;
    car(car&&) = delete;
    car &operator=(const car&) = delete;
    car &operator=(car&&) = delete;

    void turn_left() const;
    void turn_right() const;
    void stop() const;
    void start() const;
    void reverse() const;
    void accelerate(uint8_t spped) const;
};

#endif // CAR_H
