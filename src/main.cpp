#include "events.h"
#include "atmega328p.h"

#include <util/delay.h>

/*
extern "C" {
#include "avr_mcu_section.h"

AVR_MCU(F_CPU, "atmega328p");

const struct avr_mmcu_vcd_trace_t _mytrace[] _MMCU_ = {
    { AVR_MMCU_TAG_VCD_TRACE,
      sizeof(struct avr_mmcu_vcd_trace_t) - 2,
      1 << 5,
      (void*)&PORTB,
      "PORTB"
    },

    { AVR_MMCU_TAG_VCD_TRACE,
      sizeof(struct avr_mmcu_vcd_trace_t) - 2,
      1 << 5,
      (void*)&DDRB,
      "DDRB"
    },

    { AVR_MMCU_TAG_VCD_TRACE,
      sizeof(struct avr_mmcu_vcd_trace_t) - 2,
      1 << 5,
      (void*)&PINB,
      "PINB"
    },

    { AVR_MMCU_TAG_VCD_TRACE,
      sizeof(struct avr_mmcu_vcd_trace_t) - 2,
      0,
      (void*)&PORTD,
      "PORTD"
    },

    { AVR_MMCU_TAG_VCD_TRACE,
      sizeof(struct avr_mmcu_vcd_trace_t) - 2,
      0,
      (void*)&ADC,
      "ADC"
    },
};
}
*/

ISR(ADC_vect)
{
    // user code here
}

int main()
{
    auto &board = soc::atmega328p::instance();

    board.set_pin_mode(13, soc::MODE::OUTPUT);
    board.write_digital(13, false);

    bool on = false;
    int r = 0;
    char buf[32];

    while (true) {

        board.write_digital(13, !on);
        on = !on;

        /*
        if (on)
            board.serial().print("13: on\r\n");
        else
            board.serial().print("13: off\r\n");
*/
        /*
        r = board.read_analog(15);
        itoa(r, buf, 10);
        board.serial().print("analog: " + std::string(buf) + "\r\n");
        board.serial().print(buf);

        uint16_t tmp = board.read_analog(14);
        board.write_analog(14, 80);
        board.read_digital(14);
        tmp = board.read_analog(14);
*/
        _delay_ms(500);
    }

    return 0;
}
