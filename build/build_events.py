#!/usr/bin/env python3

import sys
import re

events_head_h = """#ifndef EVENTS_H
#define EVENTS_H

#include "arduino.h"
#include "atmega328p.h"

namespace soc
{"""

events_tail_h = """}

#endif // EVENTS_H"""

event_table = {
    "handle_adc":              "ADC_vect",
    "handle_analog_comp_0":    "ANALOG_COMP_0_vect",
    "handle_analog_comp_1":    "ANALOG_COMP_1_vect",
    "handle_analog_comp_2":    "ANALOG_COMP_2_vect",
    "handle_analog_comp":      "ANALOG_COMP_vect",
    "handle_ana_comp":         "ANA_COMP_vect",
    "handle_canit":            "CANIT_vect",
    "handle_eeprom_ready":     "EEPROM_READY_vect",
    "handle_ee_rdy":           "EE_RDY_vect",
    "handle_ee_ready":         "EE_READY_vect",
    "handle_ext_int0":         "EXT_INT0_vect",
    "handle_int0":             "INT0_vect",
    "handle_int1":             "INT1_vect",
    "handle_int2":             "INT2_vect",
    "handle_int3":             "INT3_vect",
    "handle_int4":             "INT4_vect",
    "handle_int5":             "INT5_vect",
    "handle_int6":             "INT6_vect",
    "handle_int7":             "INT7_vect",
    "handle_io_pins":          "IO_PINS_vect",
    "handle_lcd":              "LCD_vect",
    "handle_lowlevel_io_pins": "LOWLEVEL_IO_PINS_vect",
    "handle_ovrit":            "OVRIT_vect",
    "handle_pcint0":           "PCINT0_vect",
    "handle_pcint1":           "PCINT1_vect",
    "handle_pcint2":           "PCINT2_vect",
    "handle_pcint3":           "PCINT3_vect",
    "handle_pcint":            "PCINT_vect",
    "handle_psc0_capt":        "PSC0_CAPT_vect",
    "handle_psc0_ec":          "PSC0_EC_vect",
    "handle_psc1_capt":        "PSC1_CAPT_vect",
    "handle_psc1_ec":          "PSC1_EC_vect",
    "handle_psc2_capt":        "PSC2_CAPT_vect",
    "handle_psc2_ec":          "PSC2_EC_vect",
    "handle_spi_stc":          "SPI_STC_vect",
    "handle_spm_rdy":          "SPM_RDY_vect",
    "handle_spm_ready":        "SPM_READY_vect",
    "handle_tim0_compa":       "TIM0_COMPA_vect",
    "handle_tim0_compb":       "TIM0_COMPB_vect",
    "handle_tim0_ovf":         "TIM0_OVF_vect",
    "handle_tim1_capt":        "TIM1_CAPT_vect",
    "handle_tim1_compa":       "TIM1_COMPA_vect",
    "handle_tim1_compb":       "TIM1_COMPB_vect",
    "handle_tim1_ovf":         "TIM1_OVF_vect",
    "handle_timer0_capt":      "TIMER0_CAPT_vect",
    "handle_timer0_compa":     "TIMER0_COMPA_vect",
    "handle_timer0_compb":     "TIMER0_COMPB_vect",
    "handle_timer0_comp_a":    "TIMER0_COMP_A_vect",
    "handle_timer0_comp":      "TIMER0_COMP_vect",
    "handle_timer0_ovf0":      "TIMER0_OVF0_vect",
    "handle_timer0_ovf":       "TIMER0_OVF_vect",
    "handle_timer1_capt1":     "TIMER1_CAPT1_vect",
    "handle_timer1_capt":      "TIMER1_CAPT_vect",
    "handle_timer1_cmpa":      "TIMER1_CMPA_vect",
    "handle_timer1_cmpb":      "TIMER1_CMPB_vect",
    "handle_timer1_comp1":     "TIMER1_COMP1_vect",
    "handle_timer1_compa":     "TIMER1_COMPA_vect",
    "handle_timer1_compb":     "TIMER1_COMPB_vect",
    "handle_timer1_compc":     "TIMER1_COMPC_vect",
    "handle_timer1_compd":     "TIMER1_COMPD_vect",
    "handle_timer1_comp":      "TIMER1_COMP_vect",
    "handle_timer1_ovf1":      "TIMER1_OVF1_vect",
    "handle_timer1_ovf":       "TIMER1_OVF_vect",
    "handle_timer2_compa":     "TIMER2_COMPA_vect",
    "handle_timer2_compb":     "TIMER2_COMPB_vect",
    "handle_timer2_comp":      "TIMER2_COMP_vect",
    "handle_timer2_ovf":       "TIMER2_OVF_vect",
    "handle_timer3_capt":      "TIMER3_CAPT_vect",
    "handle_timer3_compa":     "TIMER3_COMPA_vect",
    "handle_timer3_compb":     "TIMER3_COMPB_vect",
    "handle_timer3_compc":     "TIMER3_COMPC_vect",
    "handle_timer3_ovf":       "TIMER3_OVF_vect",
    "handle_timer4_capt":      "TIMER4_CAPT_vect",
    "handle_timer4_compa":     "TIMER4_COMPA_vect",
    "handle_timer4_compb":     "TIMER4_COMPB_vect",
    "handle_timer4_compc":     "TIMER4_COMPC_vect",
    "handle_timer4_ovf":       "TIMER4_OVF_vect",
    "handle_timer5_capt":      "TIMER5_CAPT_vect",
    "handle_timer5_compa":     "TIMER5_COMPA_vect",
    "handle_timer5_compb":     "TIMER5_COMPB_vect",
    "handle_timer5_compc":     "TIMER5_COMPC_vect",
    "handle_timer5_ovf":       "TIMER5_OVF_vect",
    "handle_twi":              "TWI_vect",
    "handle_txdone":           "TXDONE_vect",
    "handle_txempty":          "TXEMPTY_vect",
    "handle_uart0_rx":         "UART0_RX_vect",
    "handle_uart0_tx":         "UART0_TX_vect",
    "handle_uart0_udre":       "UART0_UDRE_vect",
    "handle_uart1_rx":         "UART1_RX_vect",
    "handle_uart1_tx":         "UART1_TX_vect",
    "handle_uart1_udre":       "UART1_UDRE_vect",
    "handle_uart_rx":          "UART_RX_vect",
    "handle_uart_tx":          "UART_TX_vect",
    "handle_uart_udre":        "UART_UDRE_vect",
    "handle_usart0_rxc":       "USART0_RXC_vect",
    "handle_usart0_rx":        "USART0_RX_vect",
    "handle_usart0_txc":       "USART0_TXC_vect",
    "handle_usart0_tx":        "USART0_TX_vect",
    "handle_usart0_udre":      "USART0_UDRE_vect",
    "handle_usart1_rxc":       "USART1_RXC_vect",
    "handle_usart1_rx":        "USART1_RX_vect",
    "handle_usart1_txc":       "USART1_TXC_vect",
    "handle_usart1_tx":        "USART1_TX_vect",
    "handle_usart1_udre":      "USART1_UDRE_vect",
    "handle_usart2_rx":        "USART2_RX_vect",
    "handle_usart2_tx":        "USART2_TX_vect",
    "handle_usart2_udre":      "USART2_UDRE_vect",
    "handle_usart3_rx":        "USART3_RX_vect",
    "handle_usart3_tx":        "USART3_TX_vect",
    "handle_usart3_udre":      "USART3_UDRE_vect",
    "handle_usart_rxc":        "USART_RXC_vect",
    "handle_usart_rx":         "USART_RX_vect",
    "handle_usart_txc":        "USART_TXC_vect",
    "handle_usart_tx":         "USART_TX_vect",
    "handle_usart_udre":       "USART_UDRE_vect",
    "handle_usi_overflow":     "USI_OVERFLOW_vect",
    "handle_usi_ovf":          "USI_OVF_vect",
    "handle_usi_start":        "USI_START_vect",
    "handle_usi_strt":         "USI_STRT_vect",
    "handle_usi_str":          "USI_STR_vect",
    "handle_watchdog":         "WATCHDOG_vect",
    "handle_wdt_overflow":     "WDT_OVERFLOW_vect",
    "handle_wdt":              "WDT_vect"}

def extract_event(line):
    rline = re.search('void handle_[a-z0-9_]+\(.+\)', line)

    if rline is None:
        return None

    for event in event_table.keys():
        if event == rline.group(0)[5:].split('(')[0]:
            return event

    return None

def set_events(filename_in, filename_out):
    events_to_handle = []

    with open(filename_in, mode='r') as file:
        for line in file:
            event = extract_event(line)
            if event is None:
                continue
            events_to_handle.append(event)

    with open(filename_out, mode='w') as file:
        file.write(events_head_h);

        for event in events_to_handle:
            file.write("\n")
            file.write("    ISR(" + event_table[event] + ") {\n")
            file.write("        event_t e;\n");
            file.write("        atmega328p::instance()." + event + "(e);\n")
            file.write("    }\n")

        file.write(events_tail_h)

def main(fin, fout):
    set_events(fin, fout)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: ./{0} <input.cpp> <output.h>".format(sys.argv[0]))
        sys.exit(1)

    main(sys.argv[1], sys.argv[2])
