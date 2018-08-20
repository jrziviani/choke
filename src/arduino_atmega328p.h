#ifndef ATMEGA328_H
#define ATMEGA328_H

#include "serial.h"
//#include "events.h"

#define GET_PTR(field) pgm_read_ptr(&(arduino_pins[pin].field))

//                  +-----+
//            PC6  1|o    |28  PC5 (AI 5)
//      (D 0) PD0  2|     |27  PC4 (AI 4)
//      (D 1) PD1  3|     |26  PC3 (AI 3)
//      (D 2) PD2  4|     |25  PC2 (AI 2)
// PWM+ (D 3) PD3  5|     |24  PC1 (AI 1)
//      (D 4) PD4  6|     |23  PC0 (AI 0)
//            VCC  7|     |22  GND
//            GND  8|     |21  AREF
//            PB6  9|     |20  AVCC
//            PB7 10|     |19  PB5 (D 13)
// PWM+ (D 5) PD5 11|     |18  PB4 (D 12)
// PWM+ (D 6) PD6 12|     |17  PB3 (D 11) PWM
//      (D 7) PD7 13|     |16  PB2 (D 10) PWM
//      (D 8) PB0 14|     |15  PB1 (D 9) PWM
//                  +-----+


namespace soc
{
    const uint8_t A0 = 14;
    const uint8_t A1 = 15;
    const uint8_t A2 = 16;
    const uint8_t A3 = 17;
    const uint8_t A4 = 18;
    const uint8_t A5 = 19;

    enum class REFERENCE
    {
        DEFAULT,    // 5V or 3V3
        EXTERNAL,   // voltage applied to AREF (0 or 5) is used as ref.
        RESERVED,
        INTERNAL,   // 1V1 on atmega328
    };

    struct interrupt
    {
        uint16_t addr;
        const char *name;
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

    const interrupt interrupts[] PROGMEM = {
        {0xffff, "NULL"},
        {0x0000, "RESET"},
        {0x0002, "INT0"},
        {0x0004, "INT1"},
        {0x0006, "PCINT0"},
        {0x0008, "PCINT1"},
        {0x000a, "PCINT2"},
        {0x000c, "WDT"},
        {0x000e, "TIMER2_COMPA"},
        {0x0010, "TIMER2_COMPB"},
        {0x0012, "TIMER2_OVF"},
        {0x0014, "TIMER1_CAPT"},
        {0x0016, "TIMER1_COMPA"},
        {0x0018, "TIMER1_COMPB"},
        {0x001a, "TIMER1_OVF"},
        {0x001c, "TIMER0_COMPA"},
        {0x001e, "TIMER0_COMPB"},
        {0x0020, "TIMER0_OVF"},
        {0x0022, "SPI STC"},
        {0x0024, "USART_RX"},
        {0x0026, "USART_UDRE"},
        {0x0028, "USART_TX"},
        {0x002a, "ADC"},
        {0x002c, "EE READY"},
        {0x002e, "ANALOG COMP"},
        {0x0030, "TWI"},
        {0x0032, "SPM READY"},
    };

    const avrpin arduino_pins[] PROGMEM = {
        {_BV(0), &PIND, &PORTD, &DDRD, nullptr},
        {_BV(1), &PIND, &PORTD, &DDRD, nullptr},
        {_BV(2), &PIND, &PORTD, &DDRD, nullptr},
        {_BV(3), &PIND, &PORTD, &DDRD, &OCR2B},
        {_BV(4), &PIND, &PORTD, &DDRD, nullptr},
        {_BV(5), &PIND, &PORTD, &DDRD, &OCR0B},
        {_BV(6), &PIND, &PORTD, &DDRD, &OCR0A},
        {_BV(7), &PIND, &PORTD, &DDRD, nullptr},
        {_BV(0), &PINB, &PORTB, &DDRB, nullptr},
        {_BV(1), &PINB, &PORTB, &DDRB, &OCR1A},
        {_BV(2), &PINB, &PORTB, &DDRB, &OCR1B},
        {_BV(3), &PINB, &PORTB, &DDRB, &OCR2A},
        {_BV(4), &PINB, &PORTB, &DDRB, nullptr},
        {_BV(5), &PINB, &PORTB, &DDRB, nullptr},
        {_BV(0), &PINC, &PORTC, &DDRC, nullptr},
        {_BV(1), &PINC, &PORTC, &DDRC, nullptr},
        {_BV(2), &PINC, &PORTC, &DDRC, nullptr},
        {_BV(3), &PINC, &PORTC, &DDRC, nullptr},
        {_BV(4), &PINC, &PORTC, &DDRC, nullptr},
        {_BV(5), &PINC, &PORTC, &DDRC, nullptr},
    };

    class arduino_atmega328p
    {
        private:
            REFERENCE reference_;
            const usart serial_;

        public:
            arduino_atmega328p() :
                reference_(REFERENCE::DEFAULT),
                serial_(9600) { serial_.init(9600); }

            arduino_atmega328p(uint16_t baud) :
                reference_(REFERENCE::DEFAULT),
                serial_(baud) { serial_.init(baud); }

        public:
            const usart &serial() const
            {
                return serial_;
            }

            void set_pin_mode(uint8_t pin, MODE mode)
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

            MODE get_pin_mode(uint8_t pin)
            {
                auto ddr = static_cast<volatile uint16_t*>(GET_PTR(ddr));
                auto mask = pgm_read_byte(&(arduino_pins[pin].mask));

                if ((mask & *ddr) == mask)
                    return MODE::OUTPUT;

                return MODE::INPUT;
            }

            void write_digital(uint8_t pin, bool data)
            {
                auto timer = static_cast<volatile uint16_t*>(GET_PTR(timer));
                auto port = static_cast<volatile uint16_t*>(GET_PTR(port));
                auto mask = pgm_read_byte(&(arduino_pins[pin].mask));

                //if (timer != nullptr)
                    //set_duty_cycle(&timer, 0);

                if (data)
                    *port |= mask;
                else
                    *port &= ~mask;
            }

            bool read_digital(uint8_t pin)
            {
                auto timer = static_cast<volatile uint16_t*>(GET_PTR(timer));
                auto in = static_cast<volatile uint16_t*>(GET_PTR(pin));
                auto mask = pgm_read_byte(&(arduino_pins[pin].mask));

                //if (timer != nullptr)
                    //set_duty_cycle(&timer, 0);

                return (*in & mask) ? true : false;
            }

            void set_analog_reference(REFERENCE mode)
            {
                reference_ = mode;
            }

            void write_analog(uint8_t pin, uint16_t data)
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

                if (timer == &OCR1A)
                    TCCR1A |= (1 << COM1A1);
                else if (timer == &OCR1B)
                    TCCR1A |= (1 << COM1B1);
                else if (timer == reinterpret_cast<volatile uint16_t*>(&OCR0A))
                    TCCR0A |= (1 << COM0A1);
                else if (timer == reinterpret_cast<volatile uint16_t*>(&OCR0B))
                    TCCR0A |= (1 << COM0B1);
                else if (timer == reinterpret_cast<volatile uint16_t*>(&OCR2A))
                    TCCR2A |= (1 << COM2A1);
                else if (timer == reinterpret_cast<volatile uint16_t*>(&OCR2B))
                    TCCR2A |= (1 << COM2B1);

                *timer = data;
            }

            uint16_t read_analog(uint8_t pin)
            {
                if (pin < A0 || pin > A5)
                    return 0;

                ADMUX = (/*reference_*/1 << REFS0) | ((pin - A0) & 0x7);
                ADCSRA = (1 << ADEN)  | (1 << ADIE) | (1 << ADPS2) |
                         (1 << ADPS1) | (1 << ADPS0);

                ADCSRA |= (1 << ADSC);
                // flag ADSC is set to 0 when the conversion finishes
                while (ADCSRA & (1 << ADSC));

                return ADC;
            }

            void handle_timer0_compa()
            {

            }

            void handle_timer1_compa()
            {

            }
    };
}

#endif // ATMEGA328_H
