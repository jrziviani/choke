#ifndef ATMEGA328_H
#define ATMEGA328_H

#include "event_handler.h"

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

namespace choke
{
    class atmega328p : public arduino
    {
        private:
            event_handler events_;

        protected:
            atmega328p();
            atmega328p(uint16_t baud);

        public:
            void set_pin_mode(uint8_t pin, MODE mode);
            MODE get_pin_mode(uint8_t pin);

            void write_digital(uint8_t pin, bool data);
            bool read_digital(uint8_t pin);

            void write_analog(uint8_t pin, uint16_t data);
            uint16_t read_analog(uint8_t pin);

            void handle_timer0_compa(const event_t&);

        public:
            static atmega328p &instance()
            {
                static atmega328p inst;
                return inst;
            }
    };
}

#endif // ATMEGA328_H
