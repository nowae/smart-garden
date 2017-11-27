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

#include "application.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli/cli.h"
#include "timer/timer.h"

// Include for file systems
#include "fat-mla/fat-mla.h"
#include "fat-mla/ff.h"

// Include for parsing configuration file
#include "parse-ini/parse-ini.h"

FATMLA_Config App_fsConfig =
{
    .csPin       = GPIO_PINS_PTD5,

    .cpPin       = GPIO_PINS_PTD0,
    .cpType      = SDCARD_PRESENTTYPE_HIGH,

    .delayTime   = Timer_delay,
    .currentTime = Timer_currentTick,
};

ParseINI_File App_configFile =
{
    .file     = 0,
    .filePath = CONFIGFILE_PATH,
    .dirPath  = CONFIGFILE_DIR,
};

static uint8_t App_secondCounter = 0;

static void App_cliParser(void* dev, int argc, char argv[][LOCCIONI_CLI_BUFFER_SIZE]);
static void App_rtcTimeUpdate (void);

Board_Errors App_init (void)
{
    FIL file;
    DIR dir;
    FATFS fat;
    FRESULT fr;

    // Init CLI
    Cli_init();
    Cli_addModule("APP","Application Control",0,App_cliParser);

    // Init RTC with second interrupt
    Rtc_enableSecond(OB_RTC0,App_rtcTimeUpdate);

    // File system initialization
    App_fsConfig.device = OB_SPI0;
    FATMLA_init(&App_fsConfig);
    fr = f_mount(&fat,"0:",0);

    // Open config file
//    App_configFile.fat = &fat;
//    ParseINI_open(&App_configFile);
//    ParseINI_close(&App_configFile);
}

void App_update (void)
{
    Cli_check();
}

static void App_rtcTimeUpdate (void)
{
    App_secondCounter++;
    if (App_secondCounter == 60)
    {
        // TODO: do something
        App_secondCounter = 0;
    }
}

/**************************** CLI Section *************************************/

static void App_cliHelp()
{
    Cli_sendHelpString("help","Print help information");
    Cli_sendHelpString("status","Print status information");

    // RTC informations
    Cli_sendHelpString("RTC get","Get current time");
    Cli_sendHelpString("RTC gets","Get current timein string");
    Cli_sendHelpString("RTC set <val>","Set current time");

    Cli_sendHelpString("OUT on <val>","Switch ON selected relay <1-4>");
    Cli_sendHelpString("OUT off <val>","Switch OFF selected relay <1-4>");

}

static void App_cliParser(void* dev, int argc, char argv[][LOCCIONI_CLI_BUFFER_SIZE])
{
    char dateString[26];
    Time_UnixTime time = 0;

    if ((argc == 1) || ((argc == 2) && (strcmp(argv[1],"help") == 0)))
    {
        App_cliHelp();
        return;
    }

    if (strcmp(argv[1],"RTC") == 0)
    {
        if ((argc == 3) && (strcmp(argv[2],"get") == 0))
        {
            time = (Time_UnixTime)Rtc_getTime(OB_RTC0);
            u32td(dateString,time);
            Cli_sendStatusString("RTC",dateString,"[ms]");
            return;
        }
        else if ((argc == 3) && (strcmp(argv[2],"gets") == 0))
        {
            time = (Time_UnixTime)Rtc_getTime(OB_RTC0);
            Time_unixtimeToString(time,dateString);
            Cli_sendStatusString("RTC",dateString,"");
            return;
        }
        else if ((argc == 4) && (strcmp(argv[2],"set") == 0))
        {
            dtu32(&argv[3],&time,strlen(&argv[3]));
            Rtc_setTime(OB_RTC0,time);
            Cli_sendString("The value is setted!");
            return;
        }
        else
        {
            Cli_sendString("Wrong request!");
            return;
        }
    }

    if (strcmp(argv[1],"OUT") == 0)
    {
        if ((argc == 4) && (strcmp(argv[2],"on") == 0))
        {
            uint8_t outNumber = 0;
            dtu8(&argv[3],&outNumber,strlen(&argv[3]));
            if (outNumber > 4)
            {
                Cli_sendString("Wrong request!");
                return;
            }
            switch (outNumber)
            {
            case 1:
                RELAY_OUT1_EN();
                break;
            case 2:
                RELAY_OUT2_EN();
                break;
            case 3:
                RELAY_OUT3_EN();
                break;
            case 4:
                RELAY_OUT4_EN();
                break;
            }
            Cli_sendString("The relay is ON!");
            return;
        }
        else if ((argc == 4) && (strcmp(argv[2],"off") == 0))
        {
            uint8_t outNumber = 0;
            dtu8(&argv[3],&outNumber,strlen(&argv[3]));
            if (outNumber > 4)
            {
                Cli_sendString("Wrong request!");
                return;
            }
            switch (outNumber)
            {
            case 1:
                RELAY_OUT1_DIS();
                break;
            case 2:
                RELAY_OUT2_DIS();
                break;
            case 3:
                RELAY_OUT3_DIS();
                break;
            case 4:
                RELAY_OUT4_DIS();
                break;
            }
            Cli_sendString("The relay is OFF!");
            return;
        }
    }

    Cli_sendString("APP: No command found!");
}
