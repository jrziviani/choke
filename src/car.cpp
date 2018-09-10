#include "car.h"

const uint8_t INITIAL_SPEED = 200;

car::car(choke::atmega328p &arduino) :
    arduino_(arduino)
{
}

car::car(engine left, engine right, choke::atmega328p &arduino) :
    left_(left),
    right_(right),
    arduino_(arduino)
{
    arduino_.set_pin_mode(left.pin_speed, choke::MODE::OUTPUT);
    arduino_.set_pin_mode(left.pin_forward, choke::MODE::OUTPUT);
    arduino_.set_pin_mode(left.pin_backward, choke::MODE::OUTPUT);

    arduino_.set_pin_mode(right.pin_speed, choke::MODE::OUTPUT);
    arduino_.set_pin_mode(right.pin_forward, choke::MODE::OUTPUT);
    arduino_.set_pin_mode(right.pin_backward, choke::MODE::OUTPUT);

    arduino_.write_digital(left.pin_backward, false);
    arduino_.write_digital(right.pin_backward, false);
    arduino_.write_digital(left.pin_forward, true);
    arduino_.write_digital(right.pin_forward, true);

    left.forward = right.forward = 1;
}

void car::turn_right() const
{
    arduino_.write_analog(left_.pin_speed, 200);
    arduino_.write_analog(right_.pin_speed, 0);
}

void car::turn_left() const
{
    arduino_.write_analog(right_.pin_speed, 200);
    arduino_.write_analog(left_.pin_speed, 0);
}

void car::stop() const
{
    arduino_.write_analog(left_.pin_speed, 0);
    arduino_.write_analog(right_.pin_speed, 0);
}

void car::start() const
{
    accelerate(INITIAL_SPEED);
}

void car::reverse() const
{
    if (left_.forward == 1 && right_.forward == 1) {
        arduino_.write_digital(left_.pin_forward, false);
        arduino_.write_digital(right_.pin_forward, false);
        arduino_.write_digital(left_.pin_backward, true);
        arduino_.write_digital(right_.pin_backward, true);
        left_.forward = right_.forward = 0;
    }
    else if (left_.forward == 0 && right_.forward == 0) {
        arduino_.write_digital(left_.pin_backward, false);
        arduino_.write_digital(right_.pin_backward, false);
        arduino_.write_digital(left_.pin_forward, true);
        arduino_.write_digital(right_.pin_forward, true);
        left_.forward = right_.forward = 1;
    }
    else {
        stop();
    }
}

void car::accelerate(uint8_t speed) const
{
    arduino_.write_analog(left_.pin_speed, speed);
    arduino_.write_analog(right_.pin_speed, speed);
    right_.speed = left_.speed = speed;
}
