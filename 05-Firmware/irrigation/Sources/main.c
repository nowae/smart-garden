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

#include "application.h"

static int i = 0;

int main(void)
{
    System_Errors errors = ERRORS_NO_ERROR;

    Clock_Config clockConfig =
    {
        .source         = CLOCK_CRYSTAL,
        .fext           = 8000000,
        .foutSys        = 40000000,
        .busDivider     = 2,
    };

    // Enable Clock
    errors = Clock_setDividers(clockConfig.busDivider, 0,0);
    System_Errors  error = Clock_Init(&clockConfig);

    // Enable all ports
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK |
                 SIM_SCGC5_PORTB_MASK |
                 SIM_SCGC5_PORTC_MASK |
                 SIM_SCGC5_PORTD_MASK |
                 SIM_SCGC5_PORTE_MASK;

    if (Board_init() != BOARDERRORS_OK)
    {
//        Board_systemStatus.flags.error = 1;
        /* FIXME: reboot, retry configuration, or what? */
        while(1){};
    }

    // Timer initialization
    Timer_init();

    App_init();

    for (;;)
    {
        App_update();
        i++;
    }
    /* Never leave main */
    return 0;
}

