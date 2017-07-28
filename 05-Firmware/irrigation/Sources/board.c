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

#include "board.h"

union Board_TaskStatusType Board_taskStatus;
union Board_SystemStatusType Board_systemStatus;

static Spi_Config Board_spiConfig =
{
    .soutPin            = SPI_PINS_PTD3O,
    .sinPin             = SPI_PINS_PTD2I,
    .sckPin             = SPI_PINS_PTD1,
    .pcs0Pin            = SPI_PINS_PCSNONE,
    .pcs1Pin            = SPI_PINS_PCSNONE,
    .pcs2Pin            = SPI_PINS_PCSNONE,
    .pcs3Pin            = SPI_PINS_PCSNONE,
    .pcs4Pin            = SPI_PINS_PCSNONE,

    .devType            = SPI_MASTER_MODE,
    .baudrate           = 300000,

    .sckPolarity        = SPI_SCK_INACTIVE_STATE_HIGH,
    .sckPhase           = SPI_SCK_LEADING_EDGE_DATA_CHANGED,
};

uint32_t count = 0;

static Rtc_Config Board_rtcConfig =
{
    .clockSource    = RTC_CLOCK_CLKIN,

    .alarm          = 0,
    .callbackAlarm  = 0, // No callback for alarm interrupt

    .callbackSecond = 0, // Callback initialization inside App module
};

/**
 * This function initialize the mux of digitals pins of the microcontroller.
 *
 * @return The results of initialization
 */
static Board_Errors Board_setDigitalPinout (void)
{
    System_Errors error = Gpio_config(RELAY_OUT1_PIN,RELAY_OUT1_CONFIG)           |
                          Gpio_config(RELAY_OUT2_PIN,RELAY_OUT2_CONFIG);

    // Set OFF conditions for all relays
    RELAY_OUT1_DIS();
    RELAY_OUT2_DIS();

    return BOARDERRORS_OK;
}

Board_Errors Board_init (void)
{
    System_Errors error;
    Board_Errors bError;

    // Enable digital pins function
    bError = Board_setDigitalPinout();
    if(bError != BOARDERRORS_OK)
        return bError;

    // Enable spi device for SDCard
    error = Spi_init(OB_SPI0, &Board_spiConfig);

    // Enable RTC
    error = Rtc_init (OB_RTC0, &Board_rtcConfig);
    if (error != ERRORS_NO_ERROR)
        return BOARDERRORS_INIT_RTC;
    Rtc_setTime(OB_RTC0,FW_TIME_VERSION);

    return BOARDERRORS_OK;
}
