#include "atmega328p.h"

void* operator new(size_t size_,void *ptr_)
{
    return ptr_;
}

void operator delete(void *ptr_, size_t size_)
{
    if (ptr_ != nullptr)
        free(ptr_);
}

namespace soc
{
    enum ANALOGIC : uint8_t
    {
        A0 = 14, A1, A2, A3, A4, A5,
    };

    enum PWM_STATE : bool
    {
        ON = true, OFF = false,
    };

    void switch_pwm(volatile uint16_t *timer, PWM_STATE state)
    {
        uint8_t bit = (state == ON) ? 1 : 0;

        if (timer == &OCR1A) {
            TCCR1A |= (bit << COM1A1);
            TCCR1A |= (bit << WGM11) | (bit << WGM10);
        }
        else if (timer == &OCR1B) {
            TCCR1A |= (bit << COM1B1);
            TCCR1A |= (bit << WGM11) | (bit << WGM10);
        }
        else if (timer == reinterpret_cast<volatile uint16_t*>(&OCR0A)) {
            TCCR0A |= (bit << COM0A1);
            TCCR0A |= (bit << WGM01) | (bit << WGM00);
        }
        else if (timer == reinterpret_cast<volatile uint16_t*>(&OCR0B)) {
            TCCR0A |= (bit << COM0B1);
            TCCR0A |= (bit << WGM01) | (bit << WGM00);
        }
        else if (timer == reinterpret_cast<volatile uint16_t*>(&OCR2A)) {
            TCCR2A |= (bit << COM2A1);
            TCCR2A |= (bit << WGM21) | (bit << WGM20);
        }
        else if (timer == reinterpret_cast<volatile uint16_t*>(&OCR2B)) {
            TCCR2A |= (bit << COM2B1);
            TCCR2A |= (bit << WGM21) | (bit << WGM20);
        }
    }

    atmega328p::atmega328p()
    {
        events_.register_event<TIMER0_COMPA_vect_num>(
                static_cast<handler_t>(&atmega328p::handle_timer0_compa),
                0, 0, 10);
    }

    atmega328p::atmega328p(uint16_t baud)
    {
        serial_.init(baud);
    }

    void atmega328p::set_pin_mode(uint8_t pin, MODE mode)
    {
        auto ddr = static_cast<volatile uint16_t*>(GET_PTR(ddr));
        auto port = static_cast<volatile uint16_t*>(GET_PTR(port));
        auto mask = pgm_read_byte(&(arduino_pins[pin].mask));

        uint8_t old_sreg = SREG;
        cli();

        if (mode == MODE::INPUT) {
            *ddr &= ~mask;
            *port &= ~mask;
        }
        else {
            *ddr |= mask;
        }
        SREG = old_sreg;
    }

    MODE atmega328p::get_pin_mode(uint8_t pin)
    {
        auto ddr = static_cast<volatile uint16_t*>(GET_PTR(ddr));
        auto mask = pgm_read_byte(&(arduino_pins[pin].mask));

        if ((mask & *ddr) == mask)
            return MODE::OUTPUT;

        return MODE::INPUT;
    }

    void atmega328p::write_digital(uint8_t pin, bool data)
    {
        auto timer = static_cast<volatile uint16_t*>(GET_PTR(timer));
        auto port = static_cast<volatile uint16_t*>(GET_PTR(port));
        auto mask = pgm_read_byte(&(arduino_pins[pin].mask));

        switch_pwm(timer, OFF);

        if (data)
            *port |= mask;
        else
            *port &= ~mask;
    }

    bool atmega328p::read_digital(uint8_t pin)
    {
        auto timer = static_cast<volatile uint16_t*>(GET_PTR(timer));
        auto in = static_cast<volatile uint16_t*>(GET_PTR(pin));
        auto mask = pgm_read_byte(&(arduino_pins[pin].mask));

        switch_pwm(timer, OFF);

        return (*in & mask) ? true : false;
    }

    void atmega328p::write_analog(uint8_t pin, uint16_t data)
    {
        auto timer = static_cast<volatile uint16_t*>(GET_PTR(timer));
        if (timer == nullptr) {
            return;
        }

        set_pin_mode(pin, MODE::OUTPUT);
        if (data == 0x0 || data == 0xffff) {
            write_digital(pin, static_cast<bool>(data));
            return;
        }

        switch_pwm(timer, ON);

        *timer = data;
    }

    uint16_t atmega328p::read_analog(uint8_t pin)
    {
        if (pin < A0 || pin > A5)
            return 0;

        ADMUX = (reference_ << REFS0) | ((pin - A0) & 0x7);
        ADCSRA = (1 << ADEN)  | (1 << ADIE) | (1 << ADPS2) |
                 (1 << ADPS1) | (1 << ADPS0);

        ADCSRA |= (1 << ADSC);

        // flag ADSC is set to 0 when the conversion finishes
        while (ADCSRA & (1 << ADSC));

        return ADC;
    }

    void atmega328p::handle_timer0_compa(const event_t&)
    {
        serial_.print(";-(\n");
    }
}
