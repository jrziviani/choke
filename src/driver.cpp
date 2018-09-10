#include "driver.h"
#include "timer.h"

driver::driver(uint8_t echo, uint8_t trigger) :
    pin_echo_(echo),
    pin_trigger_(trigger),
    current_state_(nm_states::STOP),
    last_block_turn_(200),
    time_(0),
    timer_(false),
    car_(nullptr)
{
}

driver::driver() :
    driver(0, 0)
{
}

void driver::init()
{
    arduino_.set_pin_mode(pin_trigger_, choke::MODE::OUTPUT);
    arduino_.set_pin_mode(pin_echo_, choke::MODE::INPUT);
    srand(arduino_.read_analog(0));

    register_event<TIMER0_COMPA_vect_num>(
        &driver::handle_timer0_compa,
        0, 0, 10);

    register_event<TIMER0_OVF_vect_num>(
        &driver::handle_timer0_ovf,
        0, 0, 10);

    // setup timer0 counter
    //    takes 16.384ms to overflow timer0 8bit counter
    //    ovf = 1 / (16Mhz/1024) * 256
    //    with a 16-bit integer it counts up to 18 min
    //    total = (ovf * 2^16) / 60 => 17:53
    // after 17:53 min the car state is reseted but I don't
    // think I'm able to play with this thing for more that 10
    // minutes in a row (neither the battery I'm using).
    // note: using a 32-bit integer it can reach almost 68 years :-D
    ms_ = prec_ = 0;
    cli();
    TCCR0B = 0x5; // 1024 pre-scaller
    TCCR0B |= (1 << WGM01);
    TIMSK0 |= (1 << TOIE0);
    TCNT0 = 0;
    sei();
}

choke::atmega328p &driver::get_arduino()
{
    return arduino_;
}

void driver::set_car(car *const c)
{
    car_ = c;
}

void driver::set_echo_trigger(uint8_t echo, uint8_t trigger)
{
    pin_trigger_ = trigger;
    pin_echo_ = echo;
}

obstacle driver::get_obstacle()
{
    unsigned long distance;

    arduino_.write_digital(pin_echo_, false);
    choke::delay_us(2);

    arduino_.write_digital(pin_trigger_, true);
    choke::delay_us(10);

    arduino_.write_digital(pin_trigger_, false);
    //distance = pulseIn(_pin_echo, HIGH);

    if (distance >= 2800) { // about 50 cm
        return obstacle::FAR;
    }
    else if (distance >= 2000 && distance < 2800) { // 35 - 50 cm
        return obstacle::MEDIUM;
    }
    else if (distance >= 1000 && distance < 2000) { // 17 - 35cm
        return obstacle::CLOSE;
    }
    else {
        return obstacle::BLOCK;
    }
}

void driver::go()
{
    int rnd;
    uint8_t obstacle;

    switch (current_state_) {
        case nm_states::STOP:
            current_state_ = nm_states::START;
            (car_->*states[nm_states::START].method)();
            break;

        case nm_states::START:
            obstacle = get_obstacle();
            if (obstacle == obstacle::MEDIUM) {
                if (last_block_turn_ == nm_states::LEFT) {
                    current_state_ = nm_states::RIGHT;
                }
                else if (last_block_turn_ == nm_states::RIGHT) {
                    current_state_ = nm_states::LEFT;
                }
                else {
                    rnd = rand() % (nm_states::RIGHT + 1 -
                                    nm_states::LEFT);
                    current_state_ = rnd + nm_states::LEFT;
                }
                time_ = ms();
                (car_->*states[current_state_].method)();
            }
            else if (obstacle == obstacle::BLOCK) {
                current_state_ = nm_states::BACKWARD;
                time_ = ms();
                (car_->*states[nm_states::BACKWARD].method)();
            }
            break;

        case nm_states::LEFT:
        case nm_states::RIGHT:
            while (ms() <= time_ + 1000) {
                return;
            }

            obstacle = get_obstacle();
            if (obstacle == obstacle::FAR) {
                current_state_ = nm_states::START;
                (car_->*states[nm_states::START].method)();
            }
            else if (obstacle == obstacle::BLOCK) {
                last_block_turn_ = current_state_;
                current_state_ = nm_states::START;
                (car_->*states[nm_states::START].method)();
            }
            break;

        case nm_states::BACKWARD:
            while (ms() <= time_ + 500) {
                return;
            }

            obstacle = get_obstacle();
            if (obstacle == obstacle::FAR) {
                timer_ = false;
                current_state_ = nm_states::START;
                (car_->*states[nm_states::BACKWARD].method)();
            }
            else if (obstacle == obstacle::BLOCK) {
                if (!timer_) {
                    time_ = ms();
                    timer_ = true;
                }
                else {
                    if (ms() > time_ + 10000) {
                        current_state_ = 200;
                        timer_ = false;
                    }
                }
            }
            break;

        default:
            current_state_ = nm_states::STOP;
            (car_->*states[nm_states::STOP].method)();
            choke::delay_ms(10000);
            break;
    }
}

uint32_t driver::ms()
{
    uint32_t ret;
    cli();
    ret = ms_ << 4;
    sei();
    return ret;
}

void driver::handle_timer0_compa(const choke::event_t &e)
{

}

void driver::handle_timer0_ovf(const choke::event_t &e)
{
    ms_++;
    prec_ += 384;
    if (prec_ > 1000) {
        prec_ -= 1000;
        ms_++;
    }
}

const char *driver::state_to_str()
{
    switch (current_state_) {
        case nm_states::STOP:
            return "car stopped";

        case nm_states::START:
            return "moving forward";

        case nm_states::LEFT:
            return "turning left";

        case nm_states::RIGHT:
            return "turning right";

        case nm_states::BACKWARD:
            return "backward";

        default:
            return "hmmmm";
    }
}

const char *driver::obstacle_to_str()
{
    switch(get_obstacle()) {
        case obstacle::FAR:
            return "far";

        case obstacle::CLOSE:
            return "close";

        case obstacle::BLOCK:
            return "blocked";

        case obstacle::MEDIUM:
            return "medium";

        default:
            return "noooo";
    }
}
