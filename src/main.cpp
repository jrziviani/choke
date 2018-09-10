#include "car.h"
#include "driver.h"

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

enum pins : uint8_t
{
    MOTOR_B = 5,
    MOTOR_B_FW,
    MOTOR_B_BW,
    MOTOR_A_BW,
    MOTOR_A_FW,
    MOTOR_A,

    ECHO = 11,
    TRIGGER,
};

int main()
{
    driver &pilot = driver::instance();

    engine left = {1, 0, pins::MOTOR_A_FW,
                         pins::MOTOR_A_BW,
                         pins::MOTOR_A};
    engine right = {1, 0, pins::MOTOR_B_FW,
                          pins::MOTOR_B_BW,
                          pins::MOTOR_B};

    car ferrari(left, right, pilot.get_arduino());
    pilot.set_car(&ferrari);

#ifdef DEBUG_CAR
    a = pilot.get_state();
    b = pilot.get_obstacle();
#endif

    while (true) {
        pilot.go();
    }

    return 0;
}
