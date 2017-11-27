/******************************************************************************
 * Copyright (C) 2017 NOWAE.IT
 *
 * Authors:
 *   Marco Giammarini <m.giammarini@warcomeb.it>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

/**
 * @file Includes/board.h
 * @author Marco Giammarini <m.giammarini@warcomeb.it>
 * @brief Board utility defines.
 */

#ifndef __BOARD_H
#define __BOARD_H

/* OHIBoard Library (MIT License) */
#include "libohiboard.h"

#define FW_MAJOR_VERSION         0
#define FW_MINOR_VERSION         0
#define FW_VERSION_STRING        "0.0"
/**
 *  Time stamp of current firmware version (-5GMT + DST).
 *  http://www.currenttimestamp.com/
 */
#define FW_TIME_VERSION          1448901002
/**
 * Macro for board version.
 *
 * 0: FRDM-K64F
 */
#define PCB_VERSION              0
#define PCB_VERSION_STRING       "0"

#define PROJECT_NAME             "Smart Garden - Irrigation"
#define PROJECT_COPYRIGTH        "(C) 2017 NOWAE.IT"

#define CONFIGFILE_PATH          "CONF/CONFIG.INI"
#define CONFIGFILE_DIR           "CONF"

#if (PCB_VERSION == 0)

// CLI parameters
#define LOCCIONI_CLI_DEV         OB_UART0
#define LOCCIONI_CLI_RX_PIN      UART_PINS_PTA1
#define LOCCIONI_CLI_TX_PIN      UART_PINS_PTA2
#define LOCCIONI_CLI_BAUDRATE    9600

// Basetime of 1ms
#define WARCOMEB_TIMER_NUMBER        0
#define WARCOMEB_TIMER_FREQUENCY     1000
#define WARCOMEB_TIMER_CALLBACK      5

// Configuration file
#define PARSEINI_PATH_LENGTH_FILE    100
#define PARSEINI_PATH_LENGTH_DIR     120

// Output signal
#define RELAY_OUT1_PIN       GPIO_PINS_PTC7
#define RELAY_OUT1_NUMBER    GPIO_PIN(7)
#define RELAY_OUT1_CONFIG    GPIO_PINS_OUTPUT
#define RELAY_OUT1_EN()      (PTC_BASE_PTR->PSOR = RELAY_OUT1_NUMBER)
#define RELAY_OUT1_DIS()     (PTC_BASE_PTR->PCOR = RELAY_OUT1_NUMBER)

#define RELAY_OUT2_PIN       GPIO_PINS_PTC0
#define RELAY_OUT2_NUMBER    GPIO_PIN(0)
#define RELAY_OUT2_CONFIG    GPIO_PINS_OUTPUT
#define RELAY_OUT2_EN()      (PTC_BASE_PTR->PSOR = RELAY_OUT2_NUMBER)
#define RELAY_OUT2_DIS()     (PTC_BASE_PTR->PCOR = RELAY_OUT2_NUMBER)

#define RELAY_OUT3_PIN       GPIO_PINS_PTC3
#define RELAY_OUT3_NUMBER    GPIO_PIN(3)
#define RELAY_OUT3_CONFIG    GPIO_PINS_OUTPUT
#define RELAY_OUT3_EN()      (PTC_BASE_PTR->PSOR = RELAY_OUT3_NUMBER)
#define RELAY_OUT3_DIS()     (PTC_BASE_PTR->PCOR = RELAY_OUT3_NUMBER)

#define RELAY_OUT4_PIN       GPIO_PINS_PTC4
#define RELAY_OUT4_NUMBER    GPIO_PIN(4)
#define RELAY_OUT4_CONFIG    GPIO_PINS_OUTPUT
#define RELAY_OUT4_EN()      (PTC_BASE_PTR->PSOR = RELAY_OUT4_NUMBER)
#define RELAY_OUT4_DIS()     (PTC_BASE_PTR->PCOR = RELAY_OUT4_NUMBER)

#endif // PCB_VERSION

typedef enum _Board_Errors
{
    // System Error
    /**< No Errors */
    BOARDERRORS_OK                      = 0x00,

	// Init error
    /**< Clock initialization fail */
    BOARDERRORS_INIT_CLOCK              = 0x11,
    BOARDERRORS_INIT_DIO                = 0x12,
    BOARDERRORS_INIT_DIO_INTERRUPT      = 0x13,
    BOARDERRORS_INIT_DAC                = 0x14,
    BOARDERRORS_INIT_RTC                = 0x15,
} Board_Errors;

union Board_TaskStatusType
{
    uint16_t requestPending;

    struct
    {
        uint8_t notUsed              :8;
    } flags;

} extern Board_taskStatus;

union Board_SystemStatusType
{
    uint8_t actualStatus;

    struct
    {
        uint8_t notUsed              :8;
    } flags;

} extern Board_systemStatus;

/**
 * This function initialize the low level peripheral.
 *
 * @return The results of initialization
 */
Board_Errors Board_init (void);

#endif /* __BOARD_H */
